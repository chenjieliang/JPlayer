//
// Created by chenjieliang on 2019/8/29.
//

#ifndef VIDEOPLAY_FFTSVIDEO_H
#define VIDEOPLAY_FFTSVIDEO_H


#include "ITSVideo.h"

class FFTSVideo : public ITSVideo {

public:
    virtual void init(int width, int height);

    virtual XParameter getParameter();

    virtual void receive(int type,              //I、P、A
                         int camType,
                         int width,
                         int height,
                         int fps,              //帧率
                         int64_t pts,             //时间戳
                         unsigned char *dataArray,     //unint_8 、unsigned char
                         long size);

private:
    XParameter param;
    long gopIndex = 0;
};


#endif //VIDEOPLAY_FFTSVIDEO_H
