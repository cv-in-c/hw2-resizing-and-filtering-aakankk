#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    if(((x<0||x>=im.w) || (y<0||y>=im.h))|| (c<0||c>=im.c))
    {
        return 0;
    }    
    else
    {
    float *value= im.data + (c * im.w * im.h)+(y * im.w)+ x;
    return *value;
    }
}

void set_pixel(image im, int x, int y, int c, float v)
{ 
    if(((x<0||x>=im.w) || (y<0||y>=im.h))|| (c<0||c>=im.c))
    {
        ;
    }
    else
    {
    im.data[(c * im.w * im.h)+(y * im.w)+ x]=v;
    }
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    for(int i=0;i<((im.c)*(im.w)*(im.h));i++)
    {
       copy.data[i]=im.data[i];       
    }
        
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    
     for(int i=0;i<((im.w)*(im.h));i++)
    {
       float luma_value= (0.299*im.data[i]) +  (0.587*im.data[i+((im.w)*(im.h))]) + (0.114*im.data[i+2*((im.w)*(im.h))]);
       gray.data[i]=luma_value;       
    }
    
    return gray;
}

void shift_image(image im, int c, float v)
{
    if(c<0 || c>=im.c)
    {
        ;
    }
    else
    {
    for(int i=0;i<((im.w)*(im.h));i++)
    {
        im.data[i+(c*((im.w)*(im.h)))]+=v;
    }
    }
}

void clamp_image(image im)
{
    for(int i=0;i<((im.c)*(im.w)*(im.h));i++)
    {
       if(im.data[i] <= 0.0f)
       {
           im.data[i]=0.0f;
       }
       else if(im.data[i] >= 1.0f)
       {
           im.data[i]=1.0f;
       } 
       else
       {
           ;
       }    
        
    }
    
}


float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    
    for(int i=0;i<((im.w)*(im.h));i++)
    {
        float red= im.data[i];
        float green= im.data[i+((im.w)*(im.h))];
        float blue= im.data[i+2*((im.w)*(im.h))];

        float Value=three_way_max(red,green,blue);

        float min=three_way_min(red,green,blue);
        float C=Value-min;
        float Saturation;
        if((red==0.0f && green==0.0f )&& blue==0.0f)
        {
            Saturation=0.0f;
        }                   
        else
        {
            Saturation= C/Value;
        }

        float Hue,Hue_dash;
        if (C == 0)
        {
            Hue = 0;
        }
        else if (Value == red)
        {
          Hue_dash = (green - blue) / C;
        }
        else if (Value == green)
        {
           Hue_dash = ((blue - red) / C) + 2;
        }       
        else if (Value == blue)
        {
          Hue_dash = ((red - green) / C) + 4;
        }
        else
        {
           ;
        } 

        if(C!=0)  
        {
        if(Hue_dash<0)
        {
            Hue= (Hue_dash/6) + 1 ;
        }
        else
        {
            Hue= (Hue_dash/6) ;
        }
        }                                                            

        im.data[i]=Hue;
        im.data[i+((im.w)*(im.h))]=Saturation;
        im.data[i+2*((im.w)*(im.h))]=Value;                    
                            
                            
    }
}

void hsv_to_rgb(image im)
{
    for (int h = 0; h < im.h; h++) 
    {
        for (int w = 0; w < im.w; w++) 
        {
            float h_val = get_pixel(im, w, h, 0);
            float s_val = get_pixel(im, w, h, 1);
            float v_val = get_pixel(im, w, h, 2);

            float c = s_val * v_val;
            float x = c * (1 - fabsf(fmodf(h_val / 60, 2) - 1));
            float m = v_val - c;

            float r, g, b;
            if (h_val < 60) {
                r = c;
                g = x;
                b = 0;
            } else if (h_val < 120) {
                r = x;
                g = c;
                b = 0;
            } else if (h_val < 180) {
                r = 0;
                g = c;
                b = x;
            } else if (h_val < 240) {
                r = 0;
                g = x;
                b = c;
            } else if (h_val < 300) {
                r = x;
                g = 0;
                b = c;
            } else {
                r = c;
                g = 0;
                b = x;
            }

            set_pixel(im, w, h, 0, r + m);
            set_pixel(im, w, h, 1, g + m);
            set_pixel(im, w, h, 2, b + m);
        }
    }


}
