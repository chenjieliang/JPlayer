//
// Created by chenjieliang on 2019/2/28.
//

#ifndef VIDEOPLAY_SLAUDIOPLAY_H
#define VIDEOPLAY_SLAUDIOPLAY_H

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "IAudioPlay.h"

class SLAudioPlay: public IAudioPlay
{
public:
    virtual bool StartPlay(XParameter out);

    bool setSpeedRate(signed short rate);
    virtual bool setVolume(int percent);
    void PlayCall(void *bufq);
    void Close();

    SLAudioPlay();
    virtual ~SLAudioPlay();

protected:
    unsigned char *buf = 0;
    std::mutex mux;

private:

    /* SLEngineItf eng = NULL;
     SLObjectItf mix = NULL;
     SLObjectItf player = NULL;
     SLPlayItf iplayer = NULL;
     SLAndroidSimpleBufferQueueItf pcmQue = NULL;

     SLDynamicInterfaceManagementItf dynamicInterfaceManagementItf;
     SLPlaybackRateItf uriPlaybackRate;
     // playback rate (default 1x:1000)
     SLpermille playbackMinRate = 500;
     SLpermille playbackMaxRate = 2000;
     SLpermille playbackRateStepSize;

     SLVolumeItf playbackVolume = NULL;*/
};
#endif //VIDEOPLAY_SLAUDIOPLAY_H
