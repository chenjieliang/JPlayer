//
// Created by chenjieliang on 2019/1/27.
//

#include "XData.h"
#include "Xlog.h"

extern "C"{
#include <libavformat/avformat.h>
}

bool XData::Alloc(int size, const char *d)
{
    Drop();
    type = UCHAR_TYPE;
    XLOGI("XData::Alloc size %d " , size);
    if(size <= 0){
        return false;
    }

    this->data = new unsigned char[size];
    if(!this->data){
        return false;
    }

    if(d)
    {
        XLOGI("XData::Alloc memcpy ");
        memcpy(this->data, d, size);
    }
    this->size = size;
    return true;
}
void XData::Drop()
{
    if(!data){
        return;
    }
    if(type == AVPACKET_TYPE)
    {
        av_packet_free((AVPacket **)&data);
    } else if(type == AVFRAME_TYPE)
    {
        av_frame_free((AVFrame **)&data);

    } else if(type == UCHAR_TYPE)
    {
        delete []data;

    } else
    {
        delete data;
    }

    data = 0;
    size = 0;
}