#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/ui/al_Parameter.hpp"
#include "al/math/al_Random.hpp"
#include <librealsense2/rs.hpp>
// #include "example.hpp" 
#include <iostream>
#include "al/io/al_Window.hpp"

using namespace al;
// using namespace rs2;

class WallApp : public App {
public:

// mesh to store the points we're rendering
Mesh verts;

Window window;

al::Window::Dim dimension;

// Declare pointcloud object, for calculating pointclouds and texture mappings
rs2::pointcloud pc;
// We want the points object to be persistent so we can display the last cloud when a frame drops
rs2::points points;

// Create a Pipeline - this serves as a top-level API for streaming and processing frames
rs2::pipeline pipe;

Texture tex;

WallApp() {

}

void onCreate() {
	window.makeCurrent();
    

    nav().pullBack(16);

    
    // Configure and start the pipeline
    pipe.start();
}

void onAnimate(double dt) {
    dimension = window.dimensions();
    std::cout << dimension.h << " " << dimension.w << std::endl;

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
    for (int i = 0; i < points.size(); i++)
    {
        if (vertices[i].z)
        {
            // upload the point and texture coordinates only for points we have depth data for
            verts.vertex(vertices[i].x,vertices[i].y,vertices[i].z);
            // glTexCoord2fv(tex_coords[i]);
            // verts.texCoord(tex_coords[i].u,tex_coords[i].v);
            
        }
    }
}

void onDraw(Graphics &g) {
    g.clear(0,0,0);

    
    // draw the pointcloud
    g.pushMatrix();

    g.color(0.5, 0.5, 0.5);
    // tex.bind();
    // g.texture();
    g.scale(4);
    g.draw(verts);
    // tex.unbind();
    // g.color(0);
    g.polygonPoint();
    // g.draw(verts);
    g.popMatrix();



}
};

int main() {
WallApp app;
app.start();
}
