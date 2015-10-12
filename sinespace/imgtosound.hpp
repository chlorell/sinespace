//
//  imgtosound.hpp
//  sinespace
//
//  Created by Mateusz Bugaj on 09.10.2015.
//  Copyright © 2015 Ocean Planet Studios. All rights reserved.
//

#ifndef imgtosound_hpp
#define imgtosound_hpp

#include <cmath>
#include <tuple>

template <typename FISource, typename FIDestination, unsigned BandWidth, unsigned Scale>
class ImgToSound {
    struct oscilator_data_t
    {
        float phase, last_amp;
        oscilator_data_t():phase(0), last_amp(0){}
    };
    oscilator_data_t oscilators[BandWidth];
    double curr_time=0.0;
public:
    double base_freq=120.0;
    double max_freq=18000.0;
 
    double amp_scale=256.0*8.0;
    double dest_freq=48000.0;
    std::pair<FIDestination, FISource> process(FISource sbegin, FISource send, FIDestination dbegin, FIDestination dend)
    {
        FISource src_i=sbegin;
        FIDestination dst_i=dbegin;
        double time_step=1.0/dest_freq;
        double band_scale=std::log2(max_freq/base_freq);
        for (; dst_i!=dend && src_i!=send; ) {
            double sum[Scale];
            for (int i=0;i<Scale; ++i)sum[i]=0.0;
            
            for (int i=0; i<BandWidth && src_i!=send; ++i,++src_i) {
                
                
                float last_amp=oscilators[i].last_amp;
                float phase=oscilators[i].phase;
                double freq=base_freq*std::exp2(float(i)*band_scale/(BandWidth-1));
                float next_amp=float(*src_i)*amp_scale;
                float amp_diff=next_amp-last_amp;
                
                if(last_amp==0)oscilators[i].phase=freq*curr_time;
                
                double t=curr_time;
                for (int b=0; b<Scale; ++b) {
                    float amp=last_amp+amp_diff*float(b)/float(Scale-1);
                    sum[b]+=amp*sin((freq*t-phase)/M_2_PI);
                    t+=time_step;
                }
                oscilators[i].last_amp=next_amp;
            }
            curr_time+=double(Scale)*time_step;
            
            for (int b=0; b<Scale&&dst_i!=dend; ++b,++dst_i) {
                *dst_i=sum[b];
            }
            
        }
        return std::make_pair(dst_i, src_i);
    }
};



#endif /* imgtosound_hpp */
