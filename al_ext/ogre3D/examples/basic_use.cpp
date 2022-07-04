
#include "al/app/al_DistributedApp.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/ui/al_ControlGUI.hpp"

#include "OgreMaterialManager.h"
#include "OgreResourceManager.h"

#include "al_ext/ogre3D/al_Ogre3D.hpp"

#include "OgreHlmsPbs.h"
#include "OgreHlmsPbsDatablock.h"
#include "OgreItem.h"
#include "OgreMeshManager2.h"
#include "OgreTextureGpuManager.h"

using namespace al;

// Based in part in https://macoy.me/blog/programming/Ogre2Setup

struct MyDistributedApp : public App {
  std::shared_ptr<Ogre3DDomain> ogreDomain;
  Ogre::SceneNode *ogreNode;

  Parameter size{"size", "", 1.0f, 0.1f, 10.f};
  ControlGUI gui;

  void onInit() override {
    // Prepend the ogre domain so Ogre draws before allolib
    ogreDomain = defaultWindowDomain()->newSubDomain<Ogre3DDomain>(true);

    // Define and init GUI
    gui << size;
    gui.init();
  }

  void onCreate() override {
    // Call function in this class to create ogre scene
    createOgreScene();
  }

  void onAnimate(double dt) override {
    // Set scale of the Ogre node from the gui slider.
    ogreNode->setScale(size * 0.01f, size * 0.01f, size * 0.01f);
  }

  void onDraw(Graphics &g) override { gui.draw(g); }

  bool onKeyDown(Keyboard const &k) override { return true; }

  void onExit() override { gui.cleanup(); }

  void createOgreScene() {

    // Define paths to resources.
    // This example requires copying the Media folder frm Ogre to al_ext/ogre3d
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        "..\\..\\Media\\models\\", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        "..\\..\\Media\\materials\\scripts\\", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        "..\\..\\Media\\2.0\\scripts\\materials\\", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        "..\\..\\data\\", "FileSystem", "General");

    // The scene manager is where we create new nodes
    Ogre::SceneManager *sceneManager = ogreDomain->getSceneManager();

    {
      // To add a mesh, first load it into an "Entity"
      Ogre::v1::Entity *ogreEntity = sceneManager->createEntity("knot.mesh");
      // Then create a node for the scene graph
      ogreNode = ogreDomain->getSceneManager()
                     ->getRootSceneNode()
                     ->createChildSceneNode();
      // And attach the mesh entity to the node
      ogreNode->attachObject(ogreEntity);
      ogreNode->translate(0, 0, -4);
    }

    {
      // Create lights
      Ogre::SceneNode *newLightNode = nullptr;
      Ogre::SceneNode *rootSceneNode = sceneManager->getRootSceneNode();
      if (rootSceneNode) {
        Ogre::Light *light = sceneManager->createLight();
        Ogre::SceneNode *lightNode =
            ((Ogre::SceneNode *)rootSceneNode->createChild());
        newLightNode = lightNode;
        lightNode->attachObject(light);
        light->setPowerScale(1.0f);
        light->setType(Ogre::Light::LT_DIRECTIONAL);
        light->setDirection(Ogre::Vector3(-1, -1, -1).normalisedCopy());
        sceneManager->setAmbientLight(
            (Ogre::ColourValue(0.3f, 0.5f, 0.7f) * 0.1f * 0.75f),
            (Ogre::ColourValue(0.6f, 0.45f, 0.3f) * 0.065f * 0.75f),
            ((-Ogre::Vector3(-1, -1, -1).normalisedCopy()) +
             (Ogre::Vector3::UNIT_Y * 0.2f)));
      }
    }
  }
};

int main() {

  MyDistributedApp app;

  app.start();
  return 0;
}
