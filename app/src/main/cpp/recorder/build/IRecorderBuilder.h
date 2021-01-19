//
// Created by chenjieliang on 2020/1/15.
//

#ifndef JPLAYER_IRECORDERBUILDER_H
#define JPLAYER_IRECORDERBUILDER_H


#include "../IRecorder.h"
#include "../audio/IAudioEncodeRecorder.h"

class IRecorderBuilder {

public:
    virtual IRecorder *BuilderRecorder();

protected:
    virtual IRecorder *createrRecorder() = 0;
    virtual IVideoRecorder *createrVideoRecorder() = 0;
    virtual IAudioEncodeRecorder *createrAudioEncodeRecorder() = 0;
    virtual IEncode *createEncode() = 0;

};


#endif //JPLAYER_IRECORDERBUILDER_H
