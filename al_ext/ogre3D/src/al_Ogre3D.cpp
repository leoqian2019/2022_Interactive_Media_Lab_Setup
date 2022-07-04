
#include "al_ext/ogre3D/al_Ogre3D.hpp"
#include "al/app/al_OpenGLGraphicsDomain.hpp"

#include "OgreArchiveManager.h"
#include "OgreCamera.h"
#include "OgreConfigFile.h"
#include "OgreRoot.h"
#include "OgreWindow.h"

#include "OGRE/Hlms/Pbs/OgreHlmsPbs.h"
#include "OGRE/Hlms/Unlit/OgreHlmsUnlit.h"
#include "OgreHlmsManager.h"

#include "Compositor/OgreCompositorManager2.h"

#include "OgreEntity.h"
#include "OgreMaterialManager.h"
#include "OgreMesh2.h"
#include "OgreMesh2Serializer.h"
#include "OgreMeshManager.h"
#include "OgreMeshManager2.h"
#include "OgreMeshSerializer.h"
#include "OgreWindowEventUtilities.h"
//#include "OgreResourceManager.h"

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"
#undef GLFW_EXPOSE_NATIVE_WIN32
#undef GLFW_EXPOSE_NATIVE_WGL

#include <iostream>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include "OSX/macUtils.h"
#endif
static void registerHlms(void) {
  using namespace Ogre;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
  // Note:  macBundlePath works for iOS too. It's misnamed.
  const String resourcePath = Ogre::macResourcesPath();
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
  const String resourcePath = Ogre::macBundlePath() + "/";
#else
  String resourcePath = "";
#endif

  ConfigFile cf;
  cf.load(resourcePath + "resources2.cfg");

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE ||                                    \
    OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
  String rootHlmsFolder =
      macBundlePath() + '/' + cf.getSetting("DoNotUseAsResource", "Hlms", "");
#else
  String rootHlmsFolder =
      resourcePath + cf.getSetting("DoNotUseAsResource", "Hlms", "");
#endif

  if (rootHlmsFolder.empty())
    rootHlmsFolder = "./";
  else if (*(rootHlmsFolder.end() - 1) != '/')
    rootHlmsFolder += "/";

  // At this point rootHlmsFolder should be a valid path to the Hlms data folder

  HlmsUnlit *hlmsUnlit = 0;
  HlmsPbs *hlmsPbs = 0;

  // For retrieval of the paths to the different folders needed
  String mainFolderPath;
  StringVector libraryFoldersPaths;
  StringVector::const_iterator libraryFolderPathIt;
  StringVector::const_iterator libraryFolderPathEn;

  ArchiveManager &archiveManager = ArchiveManager::getSingleton();

  {
    // Create & Register HlmsUnlit
    // Get the path to all the subdirectories used by HlmsUnlit
    HlmsUnlit::getDefaultPaths(mainFolderPath, libraryFoldersPaths);
    Archive *archiveUnlit = archiveManager.load(rootHlmsFolder + mainFolderPath,
                                                "FileSystem", true);
    ArchiveVec archiveUnlitLibraryFolders;
    libraryFolderPathIt = libraryFoldersPaths.begin();
    libraryFolderPathEn = libraryFoldersPaths.end();
    while (libraryFolderPathIt != libraryFolderPathEn) {
      Archive *archiveLibrary = archiveManager.load(
          rootHlmsFolder + *libraryFolderPathIt, "FileSystem", true);
      archiveUnlitLibraryFolders.push_back(archiveLibrary);
      ++libraryFolderPathIt;
    }

    // Create and register the unlit Hlms
    hlmsUnlit = OGRE_NEW HlmsUnlit(archiveUnlit, &archiveUnlitLibraryFolders);
    Root::getSingleton().getHlmsManager()->registerHlms(hlmsUnlit);
  }

  {
    // Create & Register HlmsPbs
    // Do the same for HlmsPbs:
    HlmsPbs::getDefaultPaths(mainFolderPath, libraryFoldersPaths);
    Archive *archivePbs = archiveManager.load(rootHlmsFolder + mainFolderPath,
                                              "FileSystem", true);

    // Get the library archive(s)
    ArchiveVec archivePbsLibraryFolders;
    libraryFolderPathIt = libraryFoldersPaths.begin();
    libraryFolderPathEn = libraryFoldersPaths.end();
    while (libraryFolderPathIt != libraryFolderPathEn) {
      Archive *archiveLibrary = archiveManager.load(
          rootHlmsFolder + *libraryFolderPathIt, "FileSystem", true);
      archivePbsLibraryFolders.push_back(archiveLibrary);
      ++libraryFolderPathIt;
    }

    // Create and register
    hlmsPbs = OGRE_NEW HlmsPbs(archivePbs, &archivePbsLibraryFolders);
    Root::getSingleton().getHlmsManager()->registerHlms(hlmsPbs);
  }

  RenderSystem *renderSystem = Root::getSingletonPtr()->getRenderSystem();
  if (renderSystem->getName() == "Direct3D11 Rendering Subsystem") {
    // Set lower limits 512kb instead of the default 4MB per Hlms in D3D 11.0
    // and below to avoid saturating AMD's discard limit (8MB) or
    // saturate the PCIE bus in some low end machines.
    bool supportsNoOverwriteOnTextureBuffers;
    renderSystem->getCustomAttribute("MapNoOverwriteOnDynamicBufferSRV",
                                     &supportsNoOverwriteOnTextureBuffers);

    if (!supportsNoOverwriteOnTextureBuffers) {
      hlmsPbs->setTextureBufferDefaultSize(512 * 1024);
      hlmsUnlit->setTextureBufferDefaultSize(512 * 1024);
    }
  }
}

bool al::Ogre3DDomain::init(ComputationDomain *parent) {

  if (mInitialized) {
    return true;
  }
  using namespace Ogre;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
  const String pluginsFolder = macResourcesPath();
  const String writeAccessFolder = macLogPath();
#else
  const String pluginsFolder = "./";
  const String writeAccessFolder = pluginsFolder;
#endif

#ifndef OGRE_STATIC_LIB
#if OGRE_DEBUG_MODE && !((OGRE_PLATFORM == OGRE_PLATFORM_APPLE) ||             \
                         (OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS))
  const char *pluginsFile = "plugins_d.cfg";
#else
  const char *pluginsFile = "plugins.cfg";
#endif
#endif
  mOgreRoot = OGRE_NEW Root(pluginsFolder + pluginsFile,    //
                            writeAccessFolder + "ogre.cfg", //
                            writeAccessFolder + "Ogre.log");
  Ogre::RenderSystem *renderSystem =
      mOgreRoot->getRenderSystemByName("OpenGL 3+ Rendering Subsystem");
  if (!(renderSystem)) {
    printf("Render system not found!\n");
    return false;
  }
  renderSystem->setConfigOption("sRGB Gamma Conversion", "Yes");

  mOgreRoot->setRenderSystem(renderSystem);

  // This can be used to display the OGRE configuration panel.
  //  if (!root->restoreConfig()) {
  //    if (!root->showConfigDialog())
  //      return -1;
  //  }

  // Initialize Root
  mOgreRoot->initialise(false);

  //  ogreWindow->setVisible(true);

  Ogre::NameValuePairList misc;
  // misc["currentGLContext"] = Ogre::String("true");

  mOpenGLWindowParent = dynamic_cast<GLFWOpenGLWindowDomain *>(parent);
  misc["externalGLControl"] = Ogre::StringConverter::toString((true));
// expose NSGL stuff for ogre
// See for details:
// https://www.ogre3d.org/docs/api/1.9/class_ogre_1_1_render_system.html#aaf156b9f935396e6c17f532f6e6c847e
#ifdef AL_MAC_OS
  misc["macAPI"] = Ogre::String("cocoa");
  NSWindow *nsWindow = glfwGetCocoaWindow(window);
  NSView *view = nsWindow.contentView;

  misc["macAPICocoaUseNSView"] = Ogre::String("true");
  misc["externalWindowHandle"] =
      Ogre::StringConverter::toString((unsigned long)view);
  misc["externalGLContext"] = Ogre::StringConverter::toString(
      (unsigned long)glfwGetNSGLContext(window));

#elif defined(AL_WINDOWS)
  misc["externalWindowHandle"] = Ogre::StringConverter::toString(
      (unsigned long long)glfwGetWin32Window(static_cast<GLFWwindow *>(
          mOpenGLWindowParent->window().windowHandle())));
  misc["externalGLContext"] = Ogre::StringConverter::toString(
      (unsigned long long)glfwGetWGLContext(static_cast<GLFWwindow *>(
          mOpenGLWindowParent->window().windowHandle())));

#else
  misc["externalWindowHandle"] =
      Ogre::StringConverter::toString((unsigned long)view);
  misc["externalGLContext"] = Ogre::StringConverter::toString(
      (unsigned long)glfwGetNSGLContext(window));

#endif

  ogreWindow =
      mOgreRoot->createRenderWindow("AlloOgre", 800, 600, false, &misc);

  registerHlms();

  const size_t numThreads = 1u;
  mSceneManager = mOgreRoot->createSceneManager(ST_GENERIC, numThreads,
                                                "AlloOgreSceneManager");

  // Create & setup camera
  camera = mSceneManager->createCamera("Main Camera");

  // Position it at 500 in Z direction
  camera->setPosition(Vector3(0, 5, 15));
  // Look back along -Z
  camera->lookAt(Vector3(0, 0, 0));
  camera->setNearClipDistance(0.2f);
  camera->setFarClipDistance(10000.0f);
  camera->setAutoAspectRatio(true);

  // Setup a basic compositor with a blue clear colour
  mCompositorManager = mOgreRoot->getCompositorManager2();
  const ColourValue backgroundColour(0.2f, 0.4f, 0.6f);
  mCompositorManager->createBasicWorkspaceDef(workspaceName, backgroundColour,
                                              IdString());
  mCompositorManager->addWorkspace(mSceneManager, ogreWindow->getTexture(),
                                   camera, workspaceName, true);

  //  WindowEventUtilities::addWindowEventListener(ogreWindow,
  //  &myWindowEventListener); root->addFrameListener(this);

  //  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
  //      "Media/2.0/scripts/materials/PbsMaterials", "Filesystem");

  //  { // Materials and HLMS

  //    Ogre::ResourceGroupManager &resourceGroupManager =
  //        Ogre::ResourceGroupManager::getSingleton();
  //    resourceGroupManager.addResourceLocation("data/Models", "FileSystem",
  //                                             "Models");
  //    resourceGroupManager.addResourceLocation("data/Materials/Textures",
  //                                             "FileSystem", "Textures");

  //    {
  //      resourceGroupManager.createResourceGroup("Materials");
  //      resourceGroupManager.addResourceLocation("data/Materials",
  //      "FileSystem",
  //                                               "Materials");
  //      resourceGroupManager.initialiseResourceGroup("Materials", true);
  //      resourceGroupManager.loadResourceGroup("Materials");
  //    }
  //  }
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups(true);

  { // List materials
    int i = 0;
    auto materialIterator =
        Ogre::MaterialManager::getSingleton().getResourceIterator();
    while (materialIterator.hasMoreElements()) {
      std::cout << materialIterator.peekNextValue()->getName() << std::endl;
      materialIterator.moveNext();
      i++;
    }
  }
  mInitialized = true;
  return true;
}

bool al::Ogre3DDomain::tick() {
  auto pos = mOpenGLWindowParent->nav().pos();
  camera->setPosition(Ogre::Vector3(pos.x, pos.y, pos.z));
  // Should we find a reactive way to set resolution?
  ogreWindow->requestResolution(mOpenGLWindowParent->window().width(),
                                mOpenGLWindowParent->window().height());
  // FIXME set camera orientation, this is not quite right
  // Look back along -Z
  auto lookAtVector = mOpenGLWindowParent->nav().quat().normalize().rotate(
      Vec3d(pos.x, pos.y, pos.z - 1));
  camera->lookAt(Ogre::Vector3(lookAtVector.x, lookAtVector.y, lookAtVector.z));
  Ogre::WindowEventUtilities::messagePump();
  bool ret = mOgreRoot->renderOneFrame();
  return ret;
}

Ogre::SceneManager *al::Ogre3DDomain::getSceneManager() {
  return mSceneManager;
}

Ogre::Root *al::Ogre3DDomain::getOgreRoot() { return mOgreRoot; }

bool al::Ogre3DDomain::cleanup(ComputationDomain *parent) {
  // FIXME what is needed here? How do we cleanup Ogre?

  mInitialized = false;
  return true;
}
