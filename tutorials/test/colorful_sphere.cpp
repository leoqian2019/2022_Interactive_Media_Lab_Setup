/*
Allocore Example: Shape Gallery

Description:
This shows some of the shape primitives provided by AlloCore. Many of the
shapes have parameters that can be adjusted to produce a continuum of related
shapes. Moving the mouse will adjust some of these parameters.

Author(s):
Lance Putnam, 2011, 2015
*/

#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include <cstdlib>

using namespace al;

struct MyApp : public App {
    static const int Nm = 9;
    Mesh sphere;
    double angle = 0;
    float mx = 0.5, my = 0.5;
    bool wireframe = false;
    bool vertexLight = false;

    void onCreate() override {
        // Create a sphere
        addSphere(sphere,
                  2,   // radius
                  500,  // slices
                  500   // stacks
        );

        sphere.scale(0.4);

        int Nv = sphere.vertices().size();
        float rand_R, rand_G, rand_B = 0;
        for(int i = 0; i < Nv; i++){
            rand_R = (float) rand()/RAND_MAX;
            rand_G = (float) rand()/RAND_MAX;
            rand_B = (float) rand()/RAND_MAX;
            sphere.color(rand_R, rand_G, rand_B);
//            sphere.color(1, 1, 1);
            }
//        sphere.color(HSV(1, 1, 1));
        if (!vertexLight && sphere.primitive() == Mesh::TRIANGLES) {
            sphere.decompress();
        }

        sphere.generateNormals();

        nav().pullBack(5.5);
    }

    void onAnimate(double dt) override {
        angle += 2 * dt;
        if (angle > 360) angle -= 360;

//        sphere.reset();

        int S = 0;
    }

    void onDraw(Graphics &g) override {
        static Light light;

        g.clear(0);
        g.polygonMode(wireframe ? GL_LINE : GL_FILL);
        light.pos(1, 4, 1);

        gl::depthTesting(true);
        g.lighting(true);
        g.light(light);

//        sphere.color(HSV(150, 1, 1));
        g.meshColor();

//        for (int i = 0; i < Nm; ++i) {
//            g.pushMatrix();
//            float x = float(i % 3) / 2 * 2 - 1;
//            float y = float(i / 3) / 2 * 2 - 1;
//            g.translate(x, -y, 0);
            g.pushMatrix();
            g.translate(0, 0, 0);
            g.rotate(angle * 13, 0, 0, 1);
            g.rotate(angle * 17, 0, 1, 0);
//            g.color(.5,.5,.5);
            g.draw(sphere);
            g.popMatrix();
//        }
    }

//    bool onKeyDown(const Keyboard &k) override {
//        switch (k.key()) {
//            case 'f':
//                wireframe ^= true;
//                break;
//            case 'l':
//                vertexLight ^= true;
//                break;
//        }
//        return true;
//    }
//
//    bool onMouseMove(const Mouse &m) override {
//        mx = float(m.x()) / defaultWindow().width();
//        my = float(m.y()) / defaultWindow().height();
//        return true;
//    }
};

int main() {
    MyApp app;
    app.dimensions(600, 600);
    app.title("Colorful Sphere");
    app.start();
    return 0;
}
