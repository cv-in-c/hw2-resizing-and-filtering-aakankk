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
    int size = (int)ceilf(6 * sigma);
    
    if (size % 2 == 0)
    {
        size++; 
    }

    image filter = make_image(size, size, 1);

    float total = 0;
    int mid = size / 2;
    
    for (int h = -mid; h <= mid; h++) 
    {
        for (int w = -mid; w <= mid; w++) 
        {
            float x = w;
            float y = h;
            
            float temp = expf(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * 3.141 * sigma * sigma);
            
            total += temp;
            
            set_pixel(filter, w + mid, h + mid, 0, temp);
        }
    }

    l1_normalize(filter);

    return filter;
}

image add_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);

    image newimage = make_image(a.w, a.h, a.c);

    for (int c = 0; c < a.c; c++) 
    {
        for (int h = 0; h < a.h; h++) 
        {
            for (int w = 0; w < a.w; w++) 
            {
                float value1 = get_pixel(a, w, h, c);
                float value2 = get_pixel(b, w, h, c);
                set_pixel(newimage, w, h, c, value1 + value2);
            }
        }
    }

    return newimage;
}

image sub_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);

    image newimage = make_image(a.w, a.h, a.c);

    for (int c = 0; c < a.c; c++) 
    {
        for (int h = 0; h < a.h; h++) 
        {
            for (int w = 0; w < a.w; w++) 
            {
                float value1 = get_pixel(a, w, h, c);
                float value2 = get_pixel(b, w, h, c);
                set_pixel(newimage, w, h, c, value1 - value2);
            }
        }
    }

    return newimage;
}

image make_gx_filter()
{
    image filter = make_image(3, 3, 1);
    
    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, 0);
    set_pixel(filter, 2, 0, 0, 1);
    
    set_pixel(filter, 0, 1, 0, -2);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 2, 1, 0, 2);
    
    set_pixel(filter, 0, 2, 0, -1);
    set_pixel(filter, 1, 2, 0, 0);
    set_pixel(filter, 2, 2, 0, 1);
    
    return filter;
}

image make_gy_filter()
{
    image filter = make_image(3, 3, 1);
    
    set_pixel(filter, 0, 0, 0, -1);
    set_pixel(filter, 1, 0, 0, -2);
    set_pixel(filter, 2, 0, 0, -1);
    
    set_pixel(filter, 0, 1, 0, 0);
    set_pixel(filter, 1, 1, 0, 0);
    set_pixel(filter, 2, 1, 0, 0);
    
    set_pixel(filter, 0, 2, 0, 1);
    set_pixel(filter, 1, 2, 0, 2);
    set_pixel(filter, 2, 2, 0, 1);
    
    return filter;
}

void feature_normalize(image im)
{
    float minval = INFINITY;
    float maxval = -INFINITY;

    for (int c = 0; c < im.c; c++) 
    {
        for (int h = 0; h < im.h; h++) 
        {
            for (int w = 0; w < im.w; w++) 
            {
                float temp = get_pixel(im, w, h, c);
                
                if (temp < minval)
                {
                    minval = temp;
                }
                
                if (temp > maxval)
                {
                    maxval = temp;
                }
            }
        }
    }

    float range = maxval - minval;

    if (range == 0) 
    {
        for (int c = 0; c < im.c; c++) 
        {
        for (int h = 0; h < im.h; h++) 
        {
            for (int w = 0; w < im.w; w++) 
            {
                set_pixel(im, w, h, c, 0);
            }
        }
        }
    }
    else
    {
    for (int c = 0; c < im.c; c++) 
    {
        for (int h = 0; h < im.h; h++) 
        {
            for (int w = 0; w < im.w; w++) 
            {
                float temp = get_pixel(im, w, h, c);
                
                temp = (temp - minval) / range;
                
                set_pixel(im, w, h, c, temp);
            }
        }
    }
    }
}

image *sobel_image(image im)
{
   image *newimage = calloc(2, sizeof(image));
    newimage[0] = make_image(im.w, im.h, 1); 
    newimage[1] = make_image(im.w, im.h, 1); 

    image gx_filter = make_gx_filter();
    image gy_filter = make_gy_filter();

    image gx = convolve_image(im, gx_filter, 0);
    image gy = convolve_image(im, gy_filter, 0);

    for (int h = 0; h < im.h; h++) 
    {
        for (int w = 0; w < im.w; w++) 
        {
            float gx_val = get_pixel(gx, w, h, 0);
            float gy_val = get_pixel(gy, w, h, 0);
            
            float mag = sqrtf(gx_val * gx_val + gy_val * gy_val);
            float dir = atan2f(gy_val, gx_val);
            
            set_pixel(new_image[0], w, h, 0, mag);
            set_pixel(new_image[1], w, h, 0, dir);
        }
    }

    feature_normalize(newimage[0]);

    free_image(gx);
    free_image(gy);
    free_image(gx_filter);
    free_image(gy_filter);

    return newimage;
}

image colorize_sobel(image im)
{
    image *grad = sobel_image(im);
    image magnitude = copy_image(grad[0]);

    feature_normalize(magnitude);

    for (int h = 0; h < im.h; ++h) 
    {
        for (int w = 0; w < im.w; ++w) 
        {
            float magval = get_pixel(magnitude, w, h, 0);
            float hue = (1 - magval) * 240;
            set_pixel(magnitude, w, h, 0, hue);
        }
    }

    hsv_to_rgb(magnitude);

    image res = make_image(im.w, im.h, im.c);

    for (int c = 0; c < im.c; c++)
     {
        for (int h = 0; h < im.h; h++) 
        {
            for (int w = 0; w < im.w; w++) 
            {
                float hueval = get_pixel(magnitude, w, h, 0);
                float imval = get_pixel(im, w, h, c);
                set_pixel(res, w, h, c, hueval * imval);
            }
        }
    }

    free_image(grad[0]);
    free_image(grad[1]);
    free(grad);

    return res;
}
