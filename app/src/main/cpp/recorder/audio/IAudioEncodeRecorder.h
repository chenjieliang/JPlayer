//
// Created by chenjieliang on 2020/3/5.
//

#ifndef JPLAYER_IAUDIOENCODERECORDER_H
#define JPLAYER_IAUDIOENCODERECORDER_H


#include "../../thread/IObserver.h"
#include "../../XParameter.h"
#include "../../sccodec/SCCodecParam.h"

class IAudioEncodeRecorder : public IObserver
{
public:
    void send(XData data);
    virtual XParameter getParameter() = 0;
    virtual void create(AudioEncoderCreateParam *createParam) = 0;
    virtual void encode(AudioEncodeParam *encodeParam) = 0;
};


#endif //JPLAYER_IAUDIOENCODERECORDER_H
