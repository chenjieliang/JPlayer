//
// Created by chenjieliang on 2020/1/15.
//

#include "IRecorderProxy.h"
#include "build/FFRecorderBuilder.h"


void IRecorderProxy::Init(void *vm)
{
    mux.lock();
    if(vm)
    {
        FFRecorderBuilder::InitHard(vm);
    }

    if(!recorder)
    {
        FFRecorderBuilder *recorderBuilder = new FFRecorderBuilder();
        recorder = recorderBuilder->BuilderRecorder();
    }
    mux.unlock();
}

void IRecorderProxy::createVideoEncoder(VideoEncoderCreateParam *createParam,stream_video_encode_callback cb,void *userdata)
{
    mux.lock();
    if(recorder)
    {
        recorder->isHardDecode = isHardDecode;
        recorder->createVideoEncoder(createParam,cb,userdata);
    }
    mux.unlock();
}

void IRecorderProxy::createAudioEncoder(AudioEncoderCreateParam *createParam,
                                        stream_audio_encode_callback cb, void *userdata)
{
    mux.lock();
    if(recorder)
    {
        recorder->isHardDecode = isHardDecode;
        recorder->createAudioEncoder(createParam,cb,userdata);
    }
    mux.unlock();
}

void IRecorderProxy::videoEncode(VideoEncodeParam *encodeParam)
{
    mux.lock();
    if(recorder)
    {
        recorder->videoEncode(encodeParam);
    }
    mux.unlock();
}

void IRecorderProxy::audioEncode(AudioEncodeParam *encodeParam)
{
    mux.lock();
    if(recorder)
    {
        recorder->audioEncode(encodeParam);
    }
    mux.unlock();
}

bool IRecorderProxy::Start()
{
    bool re = false;
    mux.lock();
    if(recorder)
    {
        re = recorder->Start();
    }
    mux.unlock();
    return re;
}

void IRecorderProxy::Close()
{
    mux.lock();
    if(recorder) {
        recorder->Close();
    }
    mux.unlock();
}
