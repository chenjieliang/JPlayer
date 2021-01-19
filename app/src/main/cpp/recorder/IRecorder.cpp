//
// Created by chenjieliang on 2020/1/14.
//

#include "IRecorder.h"
#include "../Xlog.h"


IRecorder *IRecorder::Get()
{
    static IRecorder p;
    return &p;
}

void IRecorder::createVideoEncoder(VideoEncoderCreateParam *createParam,stream_video_encode_callback cb,void *userdata)
{
    if(!videoRecorder){
        XLOGE("videoRecorder not init ");
        return;
    }
    videoRecorder->create(createParam);
    if(!vencoder || !vencoder->Open(videoRecorder->getParameter(),isHardDecode)){
        XLOGE("vencoder->Open failed!");
    }
    vencoder->setVideoStreamCallback(cb,userdata);
}

void IRecorder::createAudioEncoder(AudioEncoderCreateParam *createParam,
                                   stream_audio_encode_callback cb, void *userdata)
{
    if(!audioEncodeRecorder){
        XLOGE("audioEncodeRecorder not init ");
        return;
    }
    audioEncodeRecorder->create(createParam);
    if(!aencoder || !aencoder->Open(audioEncodeRecorder->getParameter(),isHardDecode)){
        XLOGE("aencoder->Open failed!");
    }
    aencoder->setAudioStreamCallback(cb,userdata);
}

void IRecorder::videoEncode(VideoEncodeParam *encodeParam)
{
    if(!videoRecorder){
        XLOGE("videoEncode not init ");
        return;
    }
    videoRecorder->encode(encodeParam);
}

void IRecorder::audioEncode(AudioEncodeParam *encodeParam)
{
    if(!audioEncodeRecorder){
        XLOGE("videoEncode not init ");
        return;
    }
    audioEncodeRecorder->encode(encodeParam);
}

bool IRecorder::Start()
{
    mux.lock();
    if (vencoder) {
        vencoder->Start();
    }
    if (aencoder) {
        aencoder->Start();
    }
    mux.unlock();
    return true;
}

void IRecorder::Close()
{
    mux.lock();
    if (vencoder) {
        vencoder->Stop();
    }
    if (aencoder) {
        aencoder->Stop();
    }
    if (vencoder) {
        vencoder->Clear();
    }
    if (aencoder) {
        aencoder->Clear();
    }
    if (aencoder) {
        aencoder->Close();
    }
    if (vencoder) {
        vencoder->Close();
    }
    mux.unlock();
}