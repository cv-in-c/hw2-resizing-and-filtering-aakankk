#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    float total = 0;
    for (int c = 0; c < im.c; c++) 
    {
        for (int h = 0; h < im.h; h++) 
        {
            for (int w = 0; w < im.w; w++) 
            {
                total += get_pixel(im, w, h, c);
            }
        }
    }

    for (int c = 0; c < im.c; c++) 
    {
        for (int h = 0; h < im.h; h++) 
        {
            for (int w = 0; w < im.w; w++) 
            {
                float temp = get_pixel(im, w, h, c) / total;
                set_pixel(im, w, h, c, temp);
            }
        }
    }
}

image make_box_filter(int w)
{
    image newimage = make_image(w, w, 1);

    for (int h = 0; h < w; h++) 
    {
        for (int wi = 0; wi < w; wi++) 
        {
            set_pixel(newimage, wi, h, 0, 1);
        }
    }

    l1_normalize(newimage);

    return newimage;
}

image convolve_image(image im, image filter, int preserve)
{
    assert(filter.c == im.c || filter.c == 1);

    int ch = preserve ? im.c : 1;
    image newimage = make_image(im.w, im.h, ch);

    for (int c = 0; c < ch; c++) 
    {
        for (int h = 0; h < im.h; h++) 
        {
            for (int w = 0; w < im.w; w++) 
            {

                
                float total = 0;
                for (int fc = 0; fc < filter.c; fc++) 
                {
                    for (int fh = 0; fh < filter.h; fh++) 
                    {
                        for (int fw = 0; fw < filter.w; fw++) 
                        {
                            int im_c = preserve ? c : 0;
                            int im_h = h + fh - filter.h / 2;
                            int im_w = w + fw - filter.w / 2;
                            
                            float im_val = get_pixel(im, im_w, im_h, im_c);
                            float filter_val = get_pixel(filter, fw, fh, fc);
                            
                            total = total + (im_val * filter_val);
                        }
                    }
                }
                
                set_pixel(newimage, w, h, c, total);

                
            }
        }
    }

    return newimage;
}

image make_highpass_filter()
{
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);

    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 4);
    set_pixel(filter, 2, 1, 0, -1);

    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);

    return filter;
}

image make_sharpen_filter()
{
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, 0);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);

    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 5);
    set_pixel(filter, 2, 1, 0, -1);

    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, -1);
    set_pixel(filter, 2, 2, 0, 0);

    return filter;
}

image make_emboss_filter()
{
    image filter = make_image(3, 3, 1);

    set_pixel(filter, 0, 0, 0, -2);
    set_pixel(filter, 1, 0, 0, -1);
    set_pixel(filter, 2, 0, 0, 0);

    set_pixel(filter, 0, 1, 0, -1);
    set_pixel(filter, 1, 1, 0, 1);
    set_pixel(filter, 2, 1, 0, 1);

    set_pixel(filter, 0, 2, 0, 0);
    set_pixel(filter, 1, 2, 0, 1);
    set_pixel(filter, 2, 2, 0, 2);

    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    return make_image(1,1,1);
}

image add_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image sub_image(image a, image b)
{
    // TODO
    return make_image(1,1,1);
}

image make_gx_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_gy_filter()
{
    // TODO
    return make_image(1,1,1);
}

void feature_normalize(image im)
{
    // TODO
}

image *sobel_image(image im)
{
    // TODO
    return calloc(2, sizeof(image));
}

image colorize_sobel(image im)
{
    // TODO
    return make_image(1,1,1);
}
