//
// Created by chenjieliang on 2020/1/14.
//

#ifndef JPLAYER_IRECORDER_H
#define JPLAYER_IRECORDER_H


#include "video/IVideoRecorder.h"
#include "encode/IEncode.h"
#include "../sdk.h"
#include "audio/IAudioEncodeRecorder.h"

class IRecorder {
public:
    IRecorder(){};
    static IRecorder *Get();
    virtual void createVideoEncoder(VideoEncoderCreateParam *createParam, stream_video_encode_callback cb,void *userdata);
    virtual void createAudioEncoder(AudioEncoderCreateParam *createParam, stream_audio_encode_callback cb,void *userdata);
    virtual bool Start();
    virtual void videoEncode(VideoEncodeParam *encodeParam);
    virtual void audioEncode(AudioEncodeParam *encodeParam);
    virtual void Close();

    //是否视频硬解码
    bool isHardDecode = true;

    IVideoRecorder *videoRecorder = 0;
    IAudioEncodeRecorder *audioEncodeRecorder = 0;
    IEncode *vencoder = 0;
    IEncode *aencoder = 0;

protected:
    std::mutex mux;
};


#endif //JPLAYER_IRECORDER_H
