//
// Created by chenjieliang on 2020/3/5.
//

#ifndef JPLAYER_FFAUDIOENCODERECORDER_H
#define JPLAYER_FFAUDIOENCODERECORDER_H


#include "../../XParameter.h"
#include "../SCCodecParam.h"
#include "../SCCodec.h"
#include "encode/IEncode.h"

class FFAudioEncoder : public AudioEncoder
{
public:
    virtual void create(AudioEncoderCreateParam *createParam, stream_audio_encode_callback callback, void *userdata);
    virtual int encode(AudioEncodeParam *encodeParam);
    virtual void close();
    void send(XData data);

private:
    XParameter param;
    IEncode *aEncode;

};


#endif //JPLAYER_FFAUDIOENCODERECORDER_H
