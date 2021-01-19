//
// Created by chenjieliang on 2019/8/29.
//

#include "ITSAudio.h"

void ITSAudio::send(XData data)
{
    if (data.data) {
        Notify(data);
    }
};