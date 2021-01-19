//
// Created by chenjieliang on 2019/8/28.
//

#include "ITSVideo.h"

void ITSVideo::send(XData data)
{
    if (data.data) {
        Notify(data);
    }
};

void ITSVideo::setSkipFrame(int skipNum) {
    this->skipNum = skipNum;
}