//
// Created by chenjieliang on 2019/8/29.
//

extern "C" {
#include <libavcodec/avcodec.h>
}
#include "FFTSAudio.h"
#include "../../Xlog.h"

void FFTSAudio :: init()
{
    XLOGI("FFNetAudio init codec id  %d ", AV_CODEC_ID_PCM_ALAW);
    AVCodecParameters *avPara = avcodec_parameters_alloc();
    avPara->codec_type = AVMEDIA_TYPE_AUDIO;
    avPara->codec_id = AV_CODEC_ID_PCM_ALAW; //G711A
    avPara->format = AV_SAMPLE_FMT_S16;
    avPara->channels = 1;
    avPara->sample_rate = 8000;
    //avPara->channel_layout = 1;

    param.para = avPara;
    param.channels = 1;
    param.sample_rate = 8000;
    AVRational time_base;
    time_base.num = 1;
    time_base.den = 1000;
    param.time_base = &time_base;
}

XParameter FFTSAudio :: getParameter()
{
    return param;
}

void FFTSAudio :: receive(
        int type,             //I、P、A
        int channels,         //声道数
        int sps,              //采样率
        int bps,              //
        int64_t


        pts,             //时间戳
        unsigned char * dataArray,     //unint_8 、unsigned char
        long size)
{
    XData d;
    /*
    AVPacket *pkt = av_packet_alloc();
    pkt->data = dataArray;
    pkt->size = size;
    pkt->pts = pts;
    */
    d.Alloc(size,(const char *)dataArray);
    d.bytes_per_sample = bps;
    d.pts = pts;
    d.isAudio = true;

    send(d);
}