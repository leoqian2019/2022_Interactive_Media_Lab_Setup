#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"

/*
 * This code creates a rotating blue cube
 */
using namespace al;

class SphereApp : public App {
public:
    // mesh to store the points we're rendering
    Mesh box;

    // angle of rotation
    double angle1, angle2;


    void onCreate() {
        angle1 = angle2 = 0;

        // creating cube
        addCube(box);


        box.decompress();
        box.generateNormals();

        nav().pullBack(4);
    }


    void onAnimate(double dt) {
        // declaring angles of rotation
        angle1 += 1. / 3;
        angle2 += M_PI / 3;

    }


    void onDraw(Graphics &g) {
        // Initialize the color setting
        g.clear(0, 0, 0);

        // Enable light reflection
        g.depthTesting(true);
        g.lighting(true);

        float angPos = 2 * M_PI / 5;
        float R = 3.0;

        // draw the cube
        g.pushMatrix();
        g.rotate(angle1, 0, 1, 0);
        g.rotate(angle2, 1, 0, 0);
        g.color(0, 0, 1);
        g.polygonFill();
        g.draw(box);

        g.popMatrix();
    }
};


int main() {
    SphereApp app;
    app.start();
}
