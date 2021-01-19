//
// Created by chenjieliang on 2020/1/14.
//
extern "C" {
#include <libavcodec/avcodec.h>
}
#include "FFVideoEncoder.h"
#include "../../Xlog.h"
#include "encode/FFEncode.h"


void FFVideoEncoder::create(VideoEncoderCreateParam *createParam, stream_video_encode_callback callback, void *userdata)
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

    vEncode = new FFEncode();
    AddObs(vEncode);

    vEncode->Open(param,true);
    vEncode->setVideoStreamCallback(callback,userdata);
    vEncode->Start();
}

int FFVideoEncoder::encode(VideoEncodeParam *encodeParam)
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

    return 0;
}

void FFVideoEncoder::send(XData data)
{
    if (data.data) {
        Notify(data);
    }
};


void FFVideoEncoder::close()
{
     if (vEncode) {
         vEncode->Stop();
         vEncode->Close();
     }
}