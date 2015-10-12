//
//  main.cpp
//  sinespace
//
//  Created by Mateusz Bugaj on 09.10.2015.
//  Copyright © 2015 Ocean Planet Studios. All rights reserved.
//

#include <iostream>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include "buffers.hpp"

int main(int argc, const char * argv[]) {
    ALCdevice *dev;
    ALCcontext *ctx;
    struct stat statbuf;
    
    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s <imgfile>\n", argv[0]);
        return 0;
    }
    
    /* First the standard open-device, create-context, set-context.. */
    dev = alcOpenDevice(NULL);
    if(!dev)
    {
        fprintf(stderr, "Oops\n");
        return 1;
    }
    ctx = alcCreateContext(dev, NULL);
    alcMakeContextCurrent(ctx);
    if(!ctx)
    {
        fprintf(stderr, "Oops2\n");
        return 1;
    }
    
    {
        /* The number of buffers and bytes-per-buffer for our stream are set
         * here. The number of buffers should be two or more, and the buffer
         * size should be a multiple of the frame size (by default, OpenAL's
         * largest frame size is 4, however extensions that can add more formats
         * may be larger). Slower systems may need more buffers/larger buffer
         * sizes. */
#define NUM_BUFFERS 1

        /* These are what we'll use for OpenAL playback */
        ALuint source, buffers[NUM_BUFFERS];
 

       
        
        /* Generate the buffers and sources */
        alGenBuffers(NUM_BUFFERS, buffers);
        alGenSources(1, &source);
        if(alGetError() != AL_NO_ERROR)
        {
            fprintf(stderr, "Error generating :(\n");
            return 1;
        }
      
        auto buff=load_audio_buffer(argv[1]);
 
            alBufferData(buffers[0], AL_FORMAT_MONO16, buff.data(), (int)buff.size()*sizeof(int16_t), 48000);
            alSourceQueueBuffers(source, NUM_BUFFERS, buffers);
        alSourcei(source, AL_LOOPING, AL_TRUE);
            alSourcePlay(source);
        
            if(alGetError() != AL_NO_ERROR)
            {
                fprintf(stderr, "Error starting :(\n");
                return 1;
            }
 
        {
            ALint val;
            /* Although mplayer is done giving us data, OpenAL may still be
             * playing the remaining buffers. Wait until it stops. */
            do {
                alGetSourcei(source, AL_SOURCE_STATE, &val);
            } while(val == AL_PLAYING);
        }
        
        /* Done playing. Delete the source and buffers */
        alDeleteSources(1, &source);
        alDeleteBuffers(NUM_BUFFERS, buffers);
    }
    
    /* All done. Close OpenAL and exit. */
    alcMakeContextCurrent(NULL);
    alcDestroyContext(ctx);
    alcCloseDevice(dev);
    
    return 0;
}
