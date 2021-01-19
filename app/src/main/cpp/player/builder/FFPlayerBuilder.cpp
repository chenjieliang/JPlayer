//
// Created by chenjieliang on 2019/3/4.
//
#include "FFPlayerBuilder.h"
#include "../demux/FFDemux.h"
#include "../decode/FFDecode.h"
#include "../resample/FFResample.h"
#include "../video/GLVideoView.h"
#include "../audio/SLAudioPlay.h"
#include "../ts/FFTSVideo.h"
#include "../ts/FFTSAudio.h"
#include "../ts/G711TSAudio.h"
#include "../decode/G711Decode.h"
#include "../../Xlog.h"
extern "C"{
#include <libavformat/avformat.h>
}

FFPlayerBuilder::FFPlayerBuilder()
{
    static bool isFirst = true;
    XLOGI("FFRecorderBuilder %d\n ", isFirst);
    if(isFirst)
    {
        isFirst = false;
        //注册所有封装器
        av_register_all();
        //注册所有的解码器
        avcodec_register_all();

        //初始化网络
        avformat_network_init();
        XLOGI("FFPlayerBuilder register ffmpeg! %u\n ", avcodec_version());
        XLOGI("FFPlayerBuilder avcodec_configuration %s\n ", avcodec_configuration());
    }
}

IDemux * FFPlayerBuilder::CreateDemux()
{
    IDemux *ffdemux = new FFDemux();
    return ffdemux;
};

ITSVideo * FFPlayerBuilder::CreateTSVideo()
{
    ITSVideo *ffTSVideo = new FFTSVideo();
    return ffTSVideo;
}

ITSAudio * FFPlayerBuilder::CreateTSAudio()
{
    ITSAudio *ffTSAudio = new FFTSAudio();
    //ITSAudio *ffTSAudio = new G711TSAudio();
    return ffTSAudio;
}

IDecode * FFPlayerBuilder::CreateVideoDecode()
{
    IDecode *ffdecode = new FFDecode();
    return ffdecode;
};

IDecode * FFPlayerBuilder::CreateAudioDecode()
{
    //IDecode *ffdecode = new G711Decode();
    IDecode *ffdecode = new FFDecode();
    return ffdecode;
};

IResample * FFPlayerBuilder::CreateResample()
{
    IResample *ffresample = new FFResample();
    return ffresample;
};

STFilter * FFPlayerBuilder::CreateSTFilter()
{
    STFilter *stFilter = new STFilter();
    return stFilter;
};

IVideoView * FFPlayerBuilder::CreateVideoView()
{
    IVideoView *glVideoView = new GLVideoView();
    return glVideoView;
};

IAudioPlay * FFPlayerBuilder::CreateAudioPlay()
{
    IAudioPlay *slAudioPlay = new SLAudioPlay();
    return slAudioPlay;
};

IPlayer *FFPlayerBuilder::CreatePlayer(unsigned char index)
{
    IPlayer *player = new IPlayer();
    return player;
    //return IPlayer::Get(index);
};

void FFPlayerBuilder::InitHard(void *vm)
{
    FFDecode::InitHard(vm);
}
