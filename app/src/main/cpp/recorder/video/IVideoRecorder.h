//
// Created by chenjieliang on 2020/1/14.
//

#ifndef JPLAYER_IVIDEORECORDER_H
#define JPLAYER_IVIDEORECORDER_H


#include "../../thread/IObserver.h"
#include "../../XParameter.h"
#include "../../sccodec/SCCodecParam.h"


class IVideoRecorder : public IObserver
{
public:
    void send(XData data);
    virtual XParameter getParameter() = 0;
    virtual void create(VideoEncoderCreateParam *createParam) = 0;
    virtual void encode(VideoEncodeParam *encodeParam) = 0;
};


#endif //JPLAYER_IVIDEORECORDER_H
