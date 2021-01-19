//
// Created by chenjieliang on 2020/1/14.
//

#ifndef JPLAYER_FFVIDEORECORDER_H
#define JPLAYER_FFVIDEORECORDER_H


#include <libavutil/frame.h>
#include "IVideoRecorder.h"

class FFVideoRecorder  : public IVideoRecorder
{
public:
    virtual XParameter getParameter();
    virtual void create(VideoEncoderCreateParam *createParam);
    virtual void encode(VideoEncodeParam *encodeParam);
private:
    XParameter param;
};


#endif //JPLAYER_FFVIDEORECORDER_H
