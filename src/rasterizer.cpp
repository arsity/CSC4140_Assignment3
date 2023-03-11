#include <cmath>
#include "rasterizer.h"

using namespace std;

namespace CGL
{

    RasterizerImp::RasterizerImp(PixelSampleMethod psm, LevelSampleMethod lsm,
        size_t width, size_t height,
        unsigned int sample_rate)
    {
        this->psm = psm;
        this->lsm = lsm;
        this->width = width;
        this->height = height;
        this->sample_rate = sample_rate;

        sample_buffer.resize(width * height * sample_rate, Color::White);
    }

    // Used by rasterize_point and rasterize_line
    void RasterizerImp::fill_pixel(size_t x, size_t y, Color c, bool augmented = false)
    {
        // TODO: Task 2: You might need to update this function to fix points and lines (such as the black rectangle border in test4.svg)
        // NOTE: You are not required to implement proper super-sampling for points and lines
        // It is sufficient to use the same color for all super-samples of a pixel for points and lines (not triangles)
        if (augmented)
        {
            sample_buffer[y * width + x] = c;
        }
        else
        {
            auto multiple = ::sqrt(get_sample_rate());
            sample_buffer[y * width * multiple + x * multiple] = c;
        }
    }

    // Rasterize a point: simple example to help you start familiarizing yourself with the starter code.
    //
    void RasterizerImp::rasterize_point(float x, float y, Color color)
    {
        // fill in the nearest pixel
        int sx = (int)floor(x);
        int sy = (int)floor(y);

        // check bounds
        if (sx < 0 || sx >= width) return;
        if (sy < 0 || sy >= height) return;

        fill_pixel(sx, sy, color, false);
        return;
    }

    // Rasterize a line.
    void RasterizerImp::rasterize_line(float x0, float y0,
        float x1, float y1,
        Color color)
    {
        if (x0 > x1)
        {
            swap(x0, x1);
            swap(y0, y1);
        }

        float pt[] = { x0, y0 };
        float m = (y1 - y0) / (x1 - x0);
        float dpt[] = { 1, m };
        int steep = abs(m) > 1;
        if (steep)
        {
            dpt[0] = x1 == x0 ? 0 : 1 / abs(m);
            dpt[1] = x1 == x0 ? (y1 - y0) / abs(y1 - y0) : m / abs(m);
        }

        while (floor(pt[0]) <= floor(x1) && abs(pt[1] - y0) <= abs(y1 - y0))
        {
            rasterize_point(pt[0], pt[1], color);
            pt[0] += dpt[0];
            pt[1] += dpt[1];
        }
    }

    // Rasterize a triangle.
    void RasterizerImp::rasterize_triangle(float x0, float y0,
        float x1, float y1,
        float x2, float y2,
        Color color)
    {
        // TODO: Task 1: Implement basic triangle rasterization here, no supersampling
        // find out the bounding box
        int min_x = floor(min(x0, min(x1, x2)));
        int max_x = ceil(max(x0, max(x1, x2)));

        int min_y = floor(min(y0, min(y1, y2)));
        int max_y = floor(max(y0, max(y1, y2)));

        int sr = get_sample_rate();
        int multiplier = sqrt(sr);

        if (sr == 1)
        {
            // iterate super pixels in the bounding box
            for (int x = min_x; x < max_x; x++)
            {
                for (int y = min_y; y < max_y; y++)
                {
                    if (check_inside(x0, y0, x1, y1, x2, y2, x + 0.5, y + 0.5, Color()))
                    {
                        rasterize_point(x, y, color);
                    }
                }
            }
        }
            // TODO: Task 2: Update to implement super-sampled rasterization
        else
        {
            for (int x = min_x * multiplier; x < max_x * multiplier; x++)
            {
                for (int y = min_y * multiplier; y < max_y * multiplier; y++)
                {
                    if (check_inside(x0 * multiplier, y0 * multiplier,
                        x1 * multiplier, y1 * multiplier,
                        x2 * multiplier, y2 * multiplier,
                        x + 0.5, y + 0.5, color))
                    {

                        fill_pixel(x, y, color, true);

                    }
                }
            }
        }

    }

    void RasterizerImp::rasterize_interpolated_color_triangle(float x0, float y0, Color c0,
        float x1, float y1, Color c1,
        float x2, float y2, Color c2)
    {
        // TODO: Task 4: Rasterize the triangle, calculating barycentric coordinates and using them to interpolate vertex colors across the triangle
        // Hint: You can reuse code from rasterize_triangle



    }

    void RasterizerImp::rasterize_textured_triangle(float x0, float y0, float u0, float v0,
        float x1, float y1, float u1, float v1,
        float x2, float y2, float u2, float v2,
        Texture& tex)
    {
        // TODO: Task 5: Fill in the SampleParams struct and pass it to the tex.sample function.
        // TODO: Task 6: Set the correct barycentric differentials in the SampleParams struct.
        // Hint: You can reuse code from rasterize_triangle/rasterize_interpolated_color_triangle




    }

    void RasterizerImp::set_sample_rate(unsigned int rate)
    {
        // TODO: Task 2: You may want to update this function for super-sampling support

        this->sample_rate = rate;

        // resize the sample buffer to multiply the previous size with sqrt(sample_rate)
        int multiplier = sqrt(get_sample_rate());
        this->sample_buffer.resize((width * multiplier) * (height * multiplier), Color::White);
    }

    void RasterizerImp::set_framebuffer_target(unsigned char* rgb_framebuffer,
        size_t width, size_t height)
    {
        // TODO: Task 2: You may want to update this function for super-sampling support

        this->width = width;
        this->height = height;
        this->rgb_framebuffer_target = rgb_framebuffer;

        // resize the sample buffer to multiply the previous size with sqrt(sample_rate)
        int multiplier = sqrt(get_sample_rate());
        this->sample_buffer.resize((width * multiplier) * (height * multiplier), Color::White);
    }

    void RasterizerImp::clear_buffers()
    {
        std::fill(rgb_framebuffer_target, rgb_framebuffer_target + 3 * width * height, 255);
        std::fill(sample_buffer.begin(), sample_buffer.end(), Color::White);
    }

    // This function is called at the end of rasterizing all elements of the
    // SVG file.  If you use a supersample buffer to rasterize SVG elements
    // for antialising, you could use this call to fill the target framebuffer
    // pixels from the supersample buffer data.
    //
    void RasterizerImp::resolve_to_framebuffer()
    {
        // TODO: Task 2: You will likely want to update this function for super-sampling support
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                Color col = point_mapping(x, y);

                for (int k = 0; k < 3; k++)
                {
                    this->rgb_framebuffer_target[3 * (y * width + x) + k] = (&col.r)[k] * 255;
                }
            }
        }
    }

    Color RasterizerImp::point_mapping(float px, float py)
    {
        auto sr = get_sample_rate();
        if (sr == 1)
        {
            return sample_buffer[py * width + px];
        }

        auto multiple = ::sqrt(sr);
        auto sum = Color{};

        for (int i = 0; i < multiple; ++i)
        {
            for (int j = 0; j < multiple; ++j)
            {
                sum += sample_buffer[py * multiple * width + i + px * multiple + j];
            }
        }

        return sum * (1 / static_cast<float>(sr));
    }

    bool
    RasterizerImp::check_inside(float x0, float y0, float x1, float y1, float x2, float y2, float px, float py, Color color)
    {
        // compute all vectors
        float x_ab = x1 - x0;
        float y_ab = y1 - y0;

        float x_bc = x2 - x1;
        float y_bc = y2 - y1;

        float x_ca = x0 - x2;
        float y_ca = y0 - y2;

        float x_ap = px - x0;
        float y_ap = py - y0;

        float x_bp = px - x1;
        float y_bp = py - y1;

        float x_cp = px - x2;
        float y_cp = py - y2;

        // perform cross-product three times
        float state_1 = x_ab * y_ap - y_ab * x_ap;
        float state_2 = x_bc * y_bp - y_bc * x_bp;
        float state_3 = x_ca * y_cp - y_ca * x_cp;

        if ((state_1 >= 0 && state_2 >= 0 && state_3 >= 0) || (state_1 <= 0 && state_2 <= 0 && state_3 <= 0))
            return true;

        return false;
    }

    Rasterizer::~Rasterizer()
    = default;

}// CGL
