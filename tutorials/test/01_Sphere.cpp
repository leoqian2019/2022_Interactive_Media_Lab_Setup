#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/ui/al_Parameter.hpp"
#include "al/math/al_Random.hpp"
#include <librealsense2/rs.hpp>
#include <iostream>  

using namespace al;
// using namespace rs2;

class SphereApp : public App {
public:
// scale of the vector field
float zpos;

// mesh to store the points we're rendering
Mesh sphere;
Mesh wall[2];

double angle1, angle2;

// Create a Pipeline - this serves as a top-level API for streaming and processing frames
rs2::pipeline p;
float dist_to_center;

SphereApp() {
    zpos = 10.0;
    dist_to_center = 0.0;
    
}

void onCreate() {
    angle1 = angle2 = 0;
    int Nv;
    // Set the camera to view the scene
    // nav().pos(0,0,zpos);
    // Prepare mesh to draw a cube
    Nv = addSphere(sphere,2,36);
    // mesh.primitive(Mesh::LINE_STRIP);
    // for(int i = 0; i < Nv; ++i){
    //     float f = float(i) /Nv;
    //     sphere.color(HSV(f*0.1+0.2,1,1));
    // }
    float f = 1.0;
    sphere.color(HSV(f*0.1+0.2,1,1));

    sphere.decompress();
    sphere.generateNormals();

    // add two rectangles

    Nv = addRect(wall[0],0,0,6,4);
    wall[0].color(HSV(f*0.1+0.2,1,1));
    wall[0].decompress();
    wall[0].generateNormals();

    Nv = addRect(wall[1],0,0,6,4);
    wall[1].color(HSV(f*0.1+0.2,1,1));
    wall[1].decompress();
    wall[1].generateNormals();

    nav().pullBack(16);

    
    // Configure and start the pipeline
    p.start();
    
    
}

void onAnimate(double dt) {
    angle1 += 1. / 3;
    angle2 += M_PI / 3;

    // Blo// ck program until frames arrive
    rs2::frameset frames = p.wait_for_frames();
// 
//     // Try to get a frame of a depth image
    rs2::depth_frame depth = frames.get_depth_frame();
// 
//     // Get the depth frame's dimensions
    auto width = depth.get_width();
    auto height = depth.get_height();
// 
//     // Query the distance from the camera to the object in the center of the image
    dist_to_center = depth.get_distance(width / 2, height / 2);
//     
    std::cout << "The camera is facing an object " << dist_to_center << " meters away \r";
}

void onDraw(Graphics &g) {
    g.clear(0,0,0);

    g.depthTesting(true);
    g.lighting(true);

    float angPos = 2 * M_PI / 5;
    float R = 3.0;

    // draw the sphere
    g.pushMatrix();
    // g.translate(R * cos(1 * angPos), R * sin(1 * angPos), 0);
    g.rotate(angle1, 0, 1, 0);
    g.rotate(angle2, 1, 0, 0);
    g.color(0.5, 0.5, 0.5);
    g.polygonFill();
    g.draw(sphere);
    g.scale(1.01);
    g.color(0);
    g.polygonLine();
    g.draw(sphere);
    g.popMatrix();


    // draw the wall[0]
    g.pushMatrix();
    g.translate(3,-2,-1);
    // g.rotate(90,0,0,1);
    g.rotate(90,0,1,0);
    g.color(0.5, 0.5, 0.5);
    g.polygonFill();
    g.draw(wall[0]);
    g.scale(4);
    g.color(0);
    g.polygonLine();
    g.draw(wall[0]);
    g.popMatrix();

    // draw the wall[1]
    g.pushMatrix();
    g.translate(-3,-2,-1);
    // g.rotate(90,0,0,1);
    g.rotate(90,0,1,0);
    g.color(0.5, 0.5, 0.5);
    g.polygonFill();
    g.draw(wall[1]);
    g.scale(4);
    g.color(0);
    g.polygonLine();
    g.draw(wall[1]);
    g.popMatrix();

}
};

int main() {
SphereApp app;
app.start();
}

// test changes