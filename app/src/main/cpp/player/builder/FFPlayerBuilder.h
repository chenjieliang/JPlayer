//
// Created by chenjieliang on 2019/3/4.
//

#ifndef VIDEOPLAY_FFPLAYERBUILDER_H
#define VIDEOPLAY_FFPLAYERBUILDER_H

#include "IPlayerBuilder.h"

class FFPlayerBuilder:public IPlayerBuilder
{
public:
    FFPlayerBuilder();
    static void InitHard(void *vm);
    static FFPlayerBuilder *Get()
    {
         static FFPlayerBuilder ff;
        return &ff;
    }

protected:
    virtual IDemux *CreateDemux();
    virtual ITSVideo *CreateTSVideo();
    virtual ITSAudio *CreateTSAudio();
    virtual IDecode *CreateVideoDecode();
    virtual IDecode *CreateAudioDecode();
    virtual IResample *CreateResample();
    virtual IVideoView *CreateVideoView();
    virtual IAudioPlay *CreateAudioPlay();
    virtual STFilter *CreateSTFilter();
    virtual IPlayer *CreatePlayer(unsigned char index = 0);
    
};
#endif //VIDEOPLAY_FFPLAYERBUILDER_H
