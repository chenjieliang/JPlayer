//
// Created by chenjieliang on 2020/3/6.
//

#include "G711TSAudio.h"
#include "../../Xlog.h"

extern "C" {
#include <libavcodec/avcodec.h>
}

void G711TSAudio::init()
{
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
    param.streamType = AudioStreamTypeG711a;

}

void G711TSAudio::receive(int type, int channels, int sps, int bps, int64_t pts,
                           unsigned char *dataArray, long size)
{
    XLOGI("G711NetAudio receive size %d",size);
    XData d;
    d.type = UCHAR_TYPE;
    d.size = size;
    d.data = dataArray;
    d.bytes_per_sample = bps;
    d.pts = pts;
    d.isAudio = true;
    send(d);
}

XParameter G711TSAudio :: getParameter()
{
    return param;
}