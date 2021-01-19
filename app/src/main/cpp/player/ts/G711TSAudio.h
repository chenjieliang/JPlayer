//
// Created by chenjieliang on 2020/3/6.
//

#ifndef JPLAYER_G711TSAUDIO_H
#define JPLAYER_G711TSAUDIO_H


#include "ITSAudio.h"

class G711TSAudio : public ITSAudio{

public:
    virtual void init();

    virtual XParameter getParameter();

    virtual void receive(
            int type,             //I、P、A
            int channels,         //声道数
            int sps,              //采样率
            int bps,              //
            int64_t pts,             //时间戳
            unsigned char * dataArray,     //unint_8 、unsigned char
            long size);

private:
    XParameter param;
};


#endif //JPLAYER_G711TSAUDIO_H
