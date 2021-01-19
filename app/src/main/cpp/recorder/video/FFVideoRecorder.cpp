//
// Created by chenjieliang on 2020/1/14.
//
extern "C" {
#include <libavcodec/avcodec.h>
}
#include "FFVideoRecorder.h"
#include "IVideoRecorder.h"
#include "../../Xlog.h"
#include "../RecordParam.h"

XParameter FFVideoRecorder::getParameter()
{
    return this->param;
}

void FFVideoRecorder::create(VideoEncoderCreateParam *createParam)
{
    AVCodecParameters *avPara = avcodec_parameters_alloc();
    avPara->codec_type = AVMEDIA_TYPE_VIDEO;
    avPara->codec_id = AV_CODEC_ID_H264;
    avPara->format = AV_PIX_FMT_YUV420P;
    if (createParam->filterFormat == ROTATE_90_CROP_LT
        || createParam->filterFormat == ROTATE_270_CROP_LT_MIRROR_LR) {
        avPara->width = createParam->height;
        avPara->height = createParam->width;
    } else {
        avPara->width = createParam->width;
        avPara->height = createParam->height;
    }
    avPara->sample_aspect_ratio.num = 4;
    avPara->sample_aspect_ratio.den = 3;
    avPara->bit_rate = createParam->bitrate * 1000;
    param.para = avPara;
    param.gop_size = createParam->gop;
    param.fps = createParam->fps;
}

void FFVideoRecorder::encode(VideoEncodeParam *encodeParam)
{
    XData d;
    d.data = (unsigned char*)encodeParam->bitstream;
    d.format = AV_PIX_FMT_YUV420P;
    d.width = encodeParam->width;
    d.height = encodeParam->height;
    d.rotateFormat = encodeParam->filterFormat;
    d.pts = encodeParam->pts;
    d.isAudio = false;
    d.size = encodeParam->bitstreamLength;
    d.type = UCHAR_TYPE;
    send(d);
}
