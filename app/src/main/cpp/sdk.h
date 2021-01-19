//
// Created by chenjieliang on 2020/1/16.
//

#ifndef JPLAYER_SDK_H
#define JPLAYER_SDK_H

typedef void (*stream_encode_cb)(int width,int height,long pts,void *data, int size, void *userdata);

typedef void (*stream_video_encode_callback)(int width,int height,long pts,void *data, int size, void *userdata);

typedef void (*stream_audio_encode_callback)(int channels, int sps, int bps, long pts,void *data, int size, void *userdata);


typedef enum {
    AudioStreamTypeUnknown,
    AudioStreamTypeAAC,
    AudioStreamTypeG711a,
    AudioStreamTypeG711u,
    AudioStreamTypeADPCM_G726,
} MediaStreamType;



#endif //JPLAYER_SDK_H
