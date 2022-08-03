#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include <librealsense2/rs.hpp>

/*
 * This code uses intel realsense camera and draw the point cloud using Allolib
 */

using namespace al;

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
        // position the camera
        nav().pos(0, 0, 0);
        // positing the direction that the camera is facing
        nav().faceToward(Vec3d(0, 0, 1), Vec3d(0, -1, 0));

        // Configure and start the pipeline
        pipe.start();
    }


    void onAnimate(double dt) {
        // get frames
        auto frames = pipe.wait_for_frames();
        // get color frames
        auto color = frames.get_color_frame();

        // For cameras that don't have RGB sensor, we'll map the pointcloud to infrared instead of color
        if (!color)
            color = frames.get_infrared_frame();

        // Tell pointcloud object to map to this color frame
        pc.map_to(color);

        // get dept frame
        auto depth = frames.get_depth_frame();

        // Generate the pointcloud and texture mappings
        points = pc.calculate(depth);


        /* this segment actually prints the pointcloud */
        auto vertices = points.get_vertices();              // get vertices
        auto tex_coords = points.get_texture_coordinates(); // and texture coordinates

        // reset the mesh object every time when the frames updates
        verts.reset();

        for (int i = 0; i < points.size(); i++) {
            if (vertices[i].z) {
                // upload the point and texture coordinates only for points we have depth data for
                verts.vertex(vertices[i].x, vertices[i].y, vertices[i].z);
                verts.texCoord(tex_coords[i].u, tex_coords[i].v);

            }
        }

        // get the dimension of the color frame
        int xRes = color.get_width();
        int yRes = color.get_height();

        // set the filters for the texture. Default: NEAREST
        tex.filterMag(Texture::LINEAR);
        tex.filterMin(Texture::LINEAR);

        tex.wrapS(Texture::CLAMP_TO_EDGE);
        tex.wrapT(Texture::CLAMP_TO_EDGE);
        // create a texture unit on the GPU
        tex.create2D(xRes, yRes, Texture::RGB, Texture::RGB, Texture::UBYTE);
        // use the color pixel data as texture
        tex.submit(color.get_data());

    }

    void onDraw(Graphics &g) {
        // clear graphics object
        g.clear();

        // Draws the pointcloud
        g.pushMatrix();
        // prepare texture for binding
        g.texture();
        tex.bind();
        // draw the vertices as individual points
        g.polygonPoint();
        // change the point size to fit the window size
        g.pointSize(defaultWindow().width() / 640);
        g.draw(verts);
        tex.unbind();
        g.popMatrix();

    }
};

int main() {
    WallApp app;
    app.start();
}
