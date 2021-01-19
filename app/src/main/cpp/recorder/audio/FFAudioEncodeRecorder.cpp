//
// Created by chenjieliang on 2020/3/5.
//

extern "C" {
#include <libavcodec/avcodec.h>
}
#include "FFAudioEncodeRecorder.h"
#include "../RecordParam.h"

XParameter FFAudioEncodeRecorder::getParameter()
{
    return this->param;
}

void FFAudioEncodeRecorder ::create(AudioEncoderCreateParam *createParam)
{
    AVCodecParameters *avPara = avcodec_parameters_alloc();
    avPara->codec_type = AVMEDIA_TYPE_AUDIO;
    //avPara->codec_id = AV_CODEC_ID_PCM_ALAW; //G711A
    switch (createParam->bitstreamType) {
        case AudioEncoderBitstreamTypeG711a :
            avPara->codec_id = AV_CODEC_ID_PCM_ALAW;
            break;
        case AudioEncoderBitstreamTypeG711u :
            avPara->codec_id = AV_CODEC_ID_PCM_MULAW;
            break;
        case AudioEncoderBitstreamTypeAAC :
            avPara->codec_id = AV_CODEC_ID_AAC;
            break;
        case AudioEncoderBitstreamTypeUnknown :
            avPara->codec_id = AV_CODEC_ID_PCM_ALAW;
            break;
    }
    avPara->format = AV_SAMPLE_FMT_S16;
    avPara->channels = createParam->channels;
    avPara->sample_rate = createParam->sampleRate;
    avPara->bit_rate = createParam->bitRate;

    param.para = avPara;
    param.channels = createParam->channels;
    param.sample_rate = createParam->sampleRate;
}

void FFAudioEncodeRecorder :: encode(AudioEncodeParam *encodeParam)
{
    XData d;
   /* AVPacket *pkt = av_packet_alloc();
    //av_init_packet(pkt);
    pkt->data = encodeParam->bitstream;
    pkt->size = encodeParam->bitstreamLength;
    pkt->pts = encodeParam->pts;

    d.size = encodeParam->bitstreamLength;
    d.data = (unsigned char*)pkt;
    d.type = AVPACKET_TYPE;
    //d.bytes_per_sample = bps;
    d.pts = (int)pkt->pts;
    d.isAudio = true;*/

    d.data = (unsigned char*)encodeParam->bitstream;
    d.pts = encodeParam->pts;
    d.isAudio = true;
    d.size = encodeParam->bitstreamLength;
    d.type = UCHAR_TYPE;

    send(d);
}
