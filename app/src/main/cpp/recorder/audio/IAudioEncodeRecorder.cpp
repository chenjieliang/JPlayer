//
// Created by chenjieliang on 2020/3/5.
//

#include "IAudioEncodeRecorder.h"

void IAudioEncodeRecorder::send(XData data)
{
    if (data.data) {
        Notify(data);
    }
};