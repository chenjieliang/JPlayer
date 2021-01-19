//
// Created by chenjieliang on 2019/8/28.
//

#ifndef VIDEOPLAY_ITSVIDEO_H
#define VIDEOPLAY_ITSVIDEO_H


#include "../../thread/IObserver.h"
#include "../../XParameter.h"

class ITSVideo : public IObserver {

public:

    virtual void init(int width, int height) = 0;

    virtual XParameter getParameter() = 0;

    virtual void receive(int type,              //I、P、A
                   int camType,
                   int width,
                   int height,
                   int fps,              //帧率
                   int64_t pts,             //时间戳
                   unsigned char *dataArray,     //unint_8 、unsigned char
                   long size) = 0;

    void send(XData data);

    void setSkipFrame(int skipNum);
protected:
    int skipNum = 0;
};


#endif //VIDEOPLAY_ITSVIDEO_H
