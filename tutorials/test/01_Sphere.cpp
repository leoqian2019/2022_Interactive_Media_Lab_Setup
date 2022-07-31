#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/ui/al_Parameter.hpp"
#include "al/math/al_Random.hpp"

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

SphereApp() {
    zpos = 10.0;
}

void onCreate() {
    angle1 = angle2 = 0;
    int Nv;
    // Prepare mesh to draw a cube
    Nv = addSphere(sphere,2,36);

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

}

void onAnimate(double dt) {
    // Declaring the angles of rotation
    angle1 += 1. / 3;
    angle2 += M_PI / 3;

}

void onDraw(Graphics &g) {
    // Initialize the color setting
    g.clear(0,0,0);

    // Enable light reflection
    g.depthTesting(true);
    g.lighting(true);

    float angPos = 2 * M_PI / 5;
    float R = 3.0;

    // draw the sphere
    g.pushMatrix();
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
