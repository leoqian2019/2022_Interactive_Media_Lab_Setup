
#include "al/app/al_DistributedApp.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/ui/al_ControlGUI.hpp"

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
  std::vector<Ogre::SceneNode *> monkeyNodes;

  Parameter size{"size", "", 1.0f, 0.1f, 10.f};
  ControlGUI gui;

  void onInit() override {
    // Prepend the ogre domain so Ogre draws before allolib
    ogreDomain = defaultWindowDomain()->newSubDomain<Ogre3DDomain>(true);

    // Call function in this class to create ogre scene
    createOgreScene();

    // Define and init GUI
    gui << size;
    gui.init();
  }

  void onAnimate(double dt) override {
    // Resize all instances
    int i = 0;
    for (auto *node : monkeyNodes) {
      node->setScale(size * (1 + i / 20.0), size, size);
      i++;
    }
  }

  void onDraw(Graphics &g) override { gui.draw(g); }

  bool onKeyDown(Keyboard const &k) override { return true; }

  void onExit() override { gui.cleanup(); }

  void createOgreScene() {

    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        "..\\..\\Media\\models\\", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        "..\\..\\Media\\materials\\scripts\\", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        "..\\..\\Media\\2.0\\scripts\\materials\\", "FileSystem", "General");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        "..\\..\\data\\", "FileSystem", "General");

    Ogre::SceneManager *sceneManager = ogreDomain->getSceneManager();
    Ogre::Root *ogreRoot = ogreDomain->getOgreRoot();

    {
      // Instance rendering with Material
      int numSpheres = 0;
      Ogre::HlmsManager *hlmsManager = ogreRoot->getHlmsManager();

      assert(
          dynamic_cast<Ogre::HlmsPbs *>(hlmsManager->getHlms(Ogre::HLMS_PBS)));

      Ogre::HlmsPbs *hlmsPbs =
          static_cast<Ogre::HlmsPbs *>(hlmsManager->getHlms(Ogre::HLMS_PBS));

      const int numX = 10;
      const int numZ = 10;

      const float armsLength = 5;
      const float startX = (numX - 1) / 2.0f;
      const float startZ = (numZ - 1) / 2.0f;

      Ogre::TextureGpuManager *textureMgr =
          ogreRoot->getRenderSystem()->getTextureGpuManager();
      Ogre::SceneNode *rootSceneNode = sceneManager->getRootSceneNode();

      // const Ogre::String meshNameV2 = "Sphere.mesh";
      const Ogre::String meshNameV2 = "Suzanne.mesh";
      {
        Ogre::MeshPtr v2Mesh = Ogre::MeshManager::getSingleton().load(
            meshNameV2,
            Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
      }

      for (int x = 0; x < numX; ++x) {
        for (int z = 0; z < numZ; ++z) {
          Ogre::String datablockName =
              "Test" + Ogre::StringConverter::toString(numSpheres++);
          Ogre::HlmsPbsDatablock *datablock =
              static_cast<Ogre::HlmsPbsDatablock *>(hlmsPbs->createDatablock(
                  datablockName, datablockName, Ogre::HlmsMacroblock(),
                  Ogre::HlmsBlendblock(), Ogre::HlmsParamVec()));

          // Reflection texture. Uncomment once you've copied
          // SaintPetersBasilica.dds to Materials/Textures
          //          {
          //            Ogre::TextureGpu *texture =
          //            textureMgr->createOrRetrieveTexture(
          //                "SaintPetersBasilica.dds",
          //                Ogre::GpuPageOutStrategy::Discard,
          //                Ogre::TextureFlags::PrefersLoadingFromFileAsSRGB,
          //                Ogre::TextureTypes::TypeCube,
          //                Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
          //            //            std::cout << (int)texture->getNumMipmaps()
          //            <<
          //            //            std::endl;

          //            datablock->setTexture(Ogre::PBSM_REFLECTION, texture);
          //          }

          datablock->setDiffuse(Ogre::Vector3(0.0f, 1.0f, 0.0f));

          datablock->setRoughness(
              std::max(0.02f, x / std::max(1.0f, (float)(numX - 1))));
          datablock->setFresnel(
              Ogre::Vector3(z / std::max(1.0f, (float)(numZ - 1))), false);

          Ogre::Item *item = nullptr;
          {
            item = sceneManager->createItem(
                meshNameV2,
                Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                Ogre::SCENE_DYNAMIC);
          }

          // Example using a .material (must be hlms) instead of using manual
          // C++: item->setDatablock("MyMaterial");

          item->setDatablock(datablock);
          item->setVisibilityFlags(0x000000002);

          Ogre::SceneNode *sceneNode =
              ((Ogre::SceneNode *)rootSceneNode->createChild());
          if (!sceneNode)
            return;
          sceneNode->setPosition(Ogre::Vector3(armsLength * x - startX, -3.0f,
                                               -(armsLength * z - startZ)));
          sceneNode->attachObject(item);
          monkeyNodes.push_back(sceneNode);
        }
      }
    }

    {
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
            ((-light->getDirection()) + (Ogre::Vector3::UNIT_Y * 0.2f)));
      }
    }
  }
};

int main() {

  MyDistributedApp app;

  app.start();
  return 0;
}
