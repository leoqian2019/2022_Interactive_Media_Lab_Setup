#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"


using namespace al;


class SphereApp : public App {
public:

// mesh to store the points we're rendering
    Mesh sphere;

    double angle1, angle2;


    void onCreate() {
        angle1 = angle2 = 0;

        addCube(sphere);


        sphere.decompress();
        sphere.generateNormals();

        nav().pullBack(4);


    }

    void onAnimate(double dt) {
        angle1 += 1. / 3;
        angle2 += M_PI / 3;

    }

    void onDraw(Graphics &g) {
        g.clear(0, 0, 0);

        g.depthTesting(true);
        g.lighting(true);

        float angPos = 2 * M_PI / 5;
        float R = 3.0;

        // draw the sphere
        g.pushMatrix();
        // g.translate(R * cos(1 * angPos), R * sin(1 * angPos), 0);
        g.rotate(angle1, 0, 1, 0);
        g.rotate(angle2, 1, 0, 0);
        g.color(0, 0, 1);
        g.polygonFill();
        g.draw(sphere);

        g.popMatrix();

    }
};

int main() {
    SphereApp app;
    app.start();
}
