//
// Created by chenjieliang on 2019/3/4.
//
#include "IPlayerBuilder.h"

IPlayer *IPlayerBuilder::BuilderPlayer(unsigned char index)
{
    IPlayer *play = CreatePlayer(index);

    //解封装
    IDemux *demux = CreateDemux();

    //视频解码
    IDecode *vdecode = CreateVideoDecode();

    //音频解码
    IDecode *adecode = CreateAudioDecode();

    //解码器观察解封装
    demux->AddObs(vdecode);
    demux->AddObs(adecode);

    //显示观察者视频解码器
    IVideoView *view = CreateVideoView();
    vdecode->AddObs(view);

    //重采样观察音频解码器
    IResample *resample = CreateResample();
    adecode->AddObs(resample);

    STFilter *stFilter = CreateSTFilter();
    resample->AddObs(stFilter);

    //音频播放观重采样
    IAudioPlay *audioPlay = CreateAudioPlay();
    //resample->AddObs(audioPlay);
    stFilter->AddObs(audioPlay);

    play->demux = demux;
    play->adecode = adecode;
    play->vdecode = vdecode;
    play->videoView = view;
    play->resample = resample;
    play->stFilter = stFilter;
    play->audioPlay = audioPlay;

    return play;
}

IPlayer *IPlayerBuilder::BuilderNetPlayer(unsigned char index)
{
    IPlayer *play = CreatePlayer(index);

    //解封装
    IDemux *demux = CreateDemux();

    ITSVideo *tsVideo = CreateTSVideo();

    ITSAudio *tsAudio = CreateTSAudio();

    //视频解码
    IDecode *vdecode = CreateVideoDecode();

    //音频解码
    IDecode *adecode = CreateAudioDecode();

    //解码器观察解封装
    tsVideo->AddObs(vdecode);
    tsAudio->AddObs(adecode);

    //显示观察者视频解码器
    IVideoView *view = CreateVideoView();
    vdecode->AddObs(view);

    //重采样观察音频解码器
    IResample *resample = CreateResample();
    adecode->AddObs(resample);

    STFilter *stFilter = CreateSTFilter();
    resample->AddObs(stFilter);

    //音频播放观重采样
    IAudioPlay *audioPlay = CreateAudioPlay();
    stFilter->AddObs(audioPlay);

    //IAudioPlay *audioPlay = CreateAudioPlay();
    //adecode->AddObs(audioPlay);


    play->netVideo = tsVideo;
    play->netAudio = tsAudio;
    play->adecode = adecode;
    play->vdecode = vdecode;
    play->videoView = view;
    play->resample = resample;
    play->stFilter = stFilter;
    play->audioPlay = audioPlay;

    return play;
}