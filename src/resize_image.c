#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int a)
{
    int near_x = (int)roundf(x);
    int near_y = (int)roundf(y);
    
    return get_pixel(im, near_x, near_y, a);
}

image nn_resize(image im, int w, int h)
{
    image newimage = make_image(w, h, im.c);

    float width_scale = (float)im.w / w;
    float height_scale = (float)im.h / h;

    for (int k = 0; k < im.c; k++) 
    {
        for (int j = 0; j < h; j++) 
        {
            for (int i = 0; i < w; i++) 
            {
                float x = (i + 0.5) * width_scale - 0.5;
                float y = (j + 0.5) * height_scale - 0.5;
                float temp = nn_interpolate(im, x, y, k);
                set_pixel(newimage, i, j, k, temp);
            }
        }
    }
    return newimage;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    return 0;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    return make_image(1,1,1);
}

