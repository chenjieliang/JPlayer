//
// Created by chenjieliang on 2020/1/14.
//

#ifndef JPLAYER_FFVIDEORECORDER_H
#define JPLAYER_FFVIDEORECORDER_H


#include <libavutil/frame.h>
#include "../SCCodec.h"
#include "../../XParameter.h"
#include "encode/IEncode.h"

class FFVideoEncoder  : public VideoEncoder
{
public:
    virtual void create(VideoEncoderCreateParam *createParam, stream_video_encode_callback callback, void *userdata);
    virtual int encode(VideoEncodeParam *encodeParam);
    virtual void close();

    void send(XData data);

private:
    XParameter param;
    IEncode *vEncode;
};


#endif //JPLAYER_FFVIDEORECORDER_H
