//
// Created by chenjieliang on 2020/1/15.
//

#ifndef JPLAYER_IRECORDERPROXY_H
#define JPLAYER_IRECORDERPROXY_H


#include "IRecorder.h"

class IRecorderProxy : public IRecorder{

public:
    IRecorderProxy(){}
    static IRecorderProxy*Get()
    {
        static IRecorderProxy px;
        return &px;
    }
    void Init(void *vm = 0);
    virtual void createVideoEncoder(VideoEncoderCreateParam *createParam,stream_video_encode_callback cb,void *userdata);
    virtual void createAudioEncoder(AudioEncoderCreateParam *createParam, stream_audio_encode_callback cb,void *userdata);
    virtual bool Start();
    virtual void videoEncode(VideoEncodeParam *encodeParam);
    virtual void audioEncode(AudioEncodeParam *encodeParam);
    virtual void Close();

protected:
    IRecorder *recorder = 0;
    std::mutex mux;
};


#endif //JPLAYER_IRECORDERPROXY_H
