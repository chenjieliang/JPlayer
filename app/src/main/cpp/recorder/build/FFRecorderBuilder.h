//
// Created by chenjieliang on 2020/1/15.
//

#ifndef JPLAYER_FFRECORDERBUILDER_H
#define JPLAYER_FFRECORDERBUILDER_H


#include "IRecorderBuilder.h"

class FFRecorderBuilder : public IRecorderBuilder
{
public:
    FFRecorderBuilder();
    static void InitHard(void *vm);
    static FFRecorderBuilder *Get()
    {
        static FFRecorderBuilder ff;
        return &ff;
    }
protected:
    virtual IRecorder *createrRecorder();
    virtual IVideoRecorder *createrVideoRecorder();
    virtual IAudioEncodeRecorder *createrAudioEncodeRecorder();
    virtual IEncode *createEncode();
};


#endif //JPLAYER_FFRECORDERBUILDER_H
