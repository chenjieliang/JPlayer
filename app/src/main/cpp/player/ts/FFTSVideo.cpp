//
// Created by chenjieliang on 2019/8/29.
//
extern "C" {
#include <libavcodec/avcodec.h>
}
#include "FFTSVideo.h"
#include "../../Xlog.h"

void FFTSVideo::init(int width, int height)
{
    AVCodecParameters *avPara = avcodec_parameters_alloc();
    avPara->codec_type = AVMEDIA_TYPE_VIDEO;
    avPara->codec_id = AV_CODEC_ID_H264;
    avPara->format = AV_PIX_FMT_YUV420P;
    avPara->width = width;
    avPara->height = height;
    avPara->sample_aspect_ratio.num = 4;
    avPara->sample_aspect_ratio.den = 3;

    param.para = avPara;
    AVRational time_base;
    time_base.num = 1;
   // time_base.den = 12800;
    time_base.den = 1000;
    param.time_base = &time_base;
}

XParameter FFTSVideo :: getParameter()
{
    return param;
}
void FFTSVideo::receive(int type, int camType, int width, int height, int fps, int64_t pts,
                         unsigned char *dataArray, long size)
{
    if (type == FRAME_I) {
        gopIndex++;
    }

    if (skipNum>0) {
        if (gopIndex != 1 && gopIndex % (skipNum + 1) != 0) {
            XLOGE("FFNetVideo current gopIndex %ld, skip frame!" , gopIndex);
            return;
        }
    }
    XData d;
    //XLOGE("av_packet_alloc 111, pts %lld: ", pts);
    /*
    AVPacket *pkt  = av_packet_alloc();
    pkt->data = dataArray;
    pkt->size = size;
    pkt->pts = pts;
    d.type = AVPACKET_TYPE;
    d.size = size;
    d.data = (unsigned char*)pkt;
    d.format = AV_PIX_FMT_YUV420P;
    d.pts = pkt->pts;
    d.isAudio = false;
    d.frameType = type;
    */
    //XLOGE("av_packet_alloc 222, pts  %lld: " ,d.pts);

    //优化内存
    d.Alloc(size,(const char *)dataArray);
    //d.type = UCHAR_TYPE;
    //d.size = size;
    //d.data = dataArray;
    d.format = AV_PIX_FMT_YUV420P;
    d.pts = pts;
    d.isAudio = false;
    d.frameType = type;

    send(d);
}
