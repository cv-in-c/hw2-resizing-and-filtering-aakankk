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
    // TODO Fill in (also fix that first line)
    return make_image(1,1,1);
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

