//
// Created by chenjieliang on 2019/8/29.
//

#ifndef VIDEOPLAY_ITSAUDIO_H
#define VIDEOPLAY_ITSAUDIO_H


#include "../../thread/IObserver.h"
#include "../../XParameter.h"

class ITSAudio : public IObserver {

public:
    virtual void init() = 0;

    virtual XParameter getParameter() = 0;

    virtual void receive(
                int type,             //I、P、A
                int channels,         //声道数
                int sps,              //采样率
                int bps,              //
                int64_t pts,             //时间戳
                unsigned char * dataArray,     //unint_8 、unsigned char
                long size) = 0;

    void send(XData data);
};


#endif //VIDEOPLAY_ITSAUDIO_H
