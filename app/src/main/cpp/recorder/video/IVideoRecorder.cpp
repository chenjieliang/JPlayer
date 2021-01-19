//
// Created by chenjieliang on 2020/1/14.
//

#include "IVideoRecorder.h"
#include "../../Xlog.h"

void IVideoRecorder::send(XData data)
{
    if (data.data) {
        Notify(data);
    }
};