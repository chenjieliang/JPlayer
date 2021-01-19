//
// Created by chenjieliang on 2020/1/14.
//

#ifndef JPLAYER_FFENCODE_H
#define JPLAYER_FFENCODE_H

extern "C"
{
 #include <libavcodec/avcodec.h>
}
#include <mutex>
#include "IEncode.h"

class FFEncode : public IEncode
{
public:
    static void InitHard(void *vm);

    virtual bool Open(XParameter para, bool isHard = false);
    virtual void Close();
    virtual void Clear();

    virtual bool SendFrame(XData pkt);

    virtual XData RecvPacket();

    virtual void rotateYUV420(const uint8_t *picture_buf,int in_width,int in_height,AVFrame *outFrame,int out_width, int out_height,
                              int in_y_size,
                              int format);
protected:
    AVCodecContext *codec = 0;
    AVFrame *avFrame = 0;
    AVPacket *avPacket = 0;
    int frameCount;//encoded frame count
    std::mutex mux;

};


#endif //JPLAYER_FFENCODE_H
