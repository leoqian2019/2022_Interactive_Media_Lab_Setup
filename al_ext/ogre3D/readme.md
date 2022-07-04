# Ogre3D rendering in allolib

This extension sets up Ogre3D rendering within al::App.

Tested with Ogre version 2.2.5 'Cerberus':
https://www.ogre3d.org/download/sdk/sdk-ogre-next

## Windows

  * Download the Ogre 2.2.5 SDK
  * Set OGRE_SDK for cmake to the location of the Ogre3D SDK
  * Copy the Media directory from the Ogre SDK to the ogre3D/examples/bin directory
  
This should be sufficient to build the al_ogre3d examples and should expose 
Ogre3D to cmake through the al_ogre3d extension.

