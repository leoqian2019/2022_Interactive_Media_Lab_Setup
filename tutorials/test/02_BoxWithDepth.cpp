#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/ui/al_Parameter.hpp"
#include "al/math/al_Random.hpp"
#include <librealsense2/rs.hpp>
// #include "example.hpp" 
#include <iostream>
#include <stdio.h>

using namespace al;
// using namespace rs2;

class WallApp : public App {
public:

// mesh to store the points we're rendering
    Mesh verts;

// Declare pointcloud object, for calculating pointclouds and texture mappings
    rs2::pointcloud pc;
// We want the points object to be persistent so we can display the last cloud when a frame drops
    rs2::points points;

// Create a Pipeline - this serves as a top-level API for streaming and processing frames
    rs2::pipeline pipe;

// std::vector to store the color of the vector field
    std::vector<Color> field;

// Texture to store the image
    Texture tex;


    WallApp() {

    }

    void onCreate() {
        nav().pos(0, 0, 0);
        nav().faceToward(Vec3d(0, 0, 1), Vec3d(0, -1, 0));
        nav().pullBack(16);


        // Configure and start the pipeline
        pipe.start();
    }

void onAnimate(double dt) {


    auto frames = pipe.wait_for_frames();

    auto color = frames.get_color_frame();

    // For cameras that don't have RGB sensor, we'll map the pointcloud to infrared instead of color
    if (!color)
        color = frames.get_infrared_frame();

    // Tell pointcloud object to map to this color frame
    pc.map_to(color);

    auto depth = frames.get_depth_frame();

    // Generate the pointcloud and texture mappings
    points = pc.calculate(depth);


    // std::cout << color.get_bytes_per_pixel() << std::endl;
    // tex.create2D(color.get_width(),color.get_height(),Texture::RGB8,Texture::RGB, Texture::UBYTE);
    // Upload the color frame to OpenGL
    // tex.submit(color.get_data());

    /* this segment actually prints the pointcloud */
    auto vertices = points.get_vertices();              // get vertices
    auto tex_coords = points.get_texture_coordinates(); // and texture coordinates

    verts.reset();
    for (int i = 0; i < points.size(); i++) {
        if (vertices[i].z) {
            // upload the point and texture coordinates only for points we have depth data for
            verts.vertex(vertices[i].x, vertices[i].y, vertices[i].z);
            verts.texCoord(tex_coords[i].u, tex_coords[i].v);

        }
    }

    int xRes = color.get_width();
    int yRes = color.get_height();
    std::cout << xRes << "," << yRes << std::endl;
    // field.resize(xRes * yRes);
    // create a texture unit on the GPU
    tex.create2D(xRes, yRes, Texture::RGB, Texture::RGB, Texture::UBYTE);

//    for (int j = 0; j < yRes; ++j) {
//        for (int i = 0; i < xRes; ++i) {
//
//            Color color = Color(0.5, 0.5, 0.5);
//
//            // store the color in the container
//            field[xRes * j + i] = color;
//
//        }
//
//    }
//    auto format = color.get_profile().format();
//    switch (format) {
//        case RS2_FORMAT_RGB8:
//            printf("1st \n");
//            break;
//        case RS2_FORMAT_RGBA8:
//            printf("2nd \n");
//            break;
//        case RS2_FORMAT_Y8:
//            printf("3rd \n");
//            break;
//        case RS2_FORMAT_Y10BPACK:
//            printf("4th \n");
//            break;
//        default:
//            throw std::runtime_error("The requested format is not supported by this demo!");
//    }
    tex.submit(color.get_data());
//    tex.submit(field.data());
}

void onDraw(Graphics &g) {
//    g.clear(0, 0, 0);

    g.clear();

    // draw the pointcloud
    g.pushMatrix();
    g.texture();
//    g.color(0.5, 0.5, 0.5);
    tex.bind();
    g.scale(4);
//    g.polygonPoint();
//    g.polygonLine()
    g.draw(verts);

    // g.color(0);

    tex.unbind();
//     g.draw(verts);

    g.popMatrix();


}
};

int main() {
WallApp app;
app.start();
}
