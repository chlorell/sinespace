//
//  buffers.cpp
//  sinespace
//
//  Created by Mateusz Bugaj on 09.10.2015.
//  Copyright © 2015 Ocean Planet Studios. All rights reserved.
//

#include "buffers.hpp"

#include "lodepng.h"
#include "imgtosound.hpp"



std::vector<int16_t> load_audio_buffer(const char * file_name)
{
    
    constexpr unsigned bandwidth=256;
    constexpr unsigned scale=256;
    
    
    std::vector<int16_t> dest;
    
    
  
    std::vector<unsigned char> image; //the raw pixels
    unsigned width, height;
    
    //decode
    unsigned error = lodepng::decode(image, width, height, file_name);
    

    if(!error&&width<=bandwidth)
    {
        std::vector<float> srcbuff;
        srcbuff.reserve(height*bandwidth);
        dest.resize(height*scale);
        
        for (int y=0; y<height; ++y)
            for (int x=0; x<bandwidth; ++x) {
                unsigned char r,g,b,a;
                unsigned offset=(y*width+x)*4;
                r=image[offset];
                g=image[offset+1];
                b=image[offset+2];
                a=image[offset+3];
                float amp=(float(r)+float(g)+float(b))*float(a)/(255.0*255.0*3.0);
                srcbuff.push_back(amp);
            }
        
        ImgToSound<std::vector<float>::iterator, std::vector<int16_t>::iterator, bandwidth, scale> converter;
        
        auto res=converter.process(srcbuff.begin(), srcbuff.end(), dest.begin(), dest.end());
        
        
    }

    
    
    return dest;
}