//
// Created by chenjieliang on 2020/3/5.
//

#ifndef JPLAYER_FFAUDIOENCODERECORDER_H
#define JPLAYER_FFAUDIOENCODERECORDER_H


#include "IAudioEncodeRecorder.h"

class FFAudioEncodeRecorder : public IAudioEncodeRecorder
{
public:
    virtual XParameter getParameter();
    virtual void create(AudioEncoderCreateParam *createParam);
    virtual void encode(AudioEncodeParam *encodeParam);
private:
    XParameter param;
};


#endif //JPLAYER_FFAUDIOENCODERECORDER_H
