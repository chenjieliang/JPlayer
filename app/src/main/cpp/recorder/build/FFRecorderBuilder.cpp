//
// Created by chenjieliang on 2020/1/15.
//

#include "FFRecorderBuilder.h"
#include "../encode/FFEncode.h"
#include "../video/FFVideoRecorder.h"
#include "../../Xlog.h"
#include "../audio/FFAudioEncodeRecorder.h"

extern "C"{
#include <libavformat/avformat.h>
}

FFRecorderBuilder::FFRecorderBuilder()
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
        XLOGI("register ffmpeg! %u\n ", avcodec_version());
        XLOGI("avcodec_configuration %s\n ", avcodec_configuration());
    }
}

IRecorder *FFRecorderBuilder::createrRecorder()
{
    IRecorder *recorder = new IRecorder();
    return recorder;
}

IEncode *FFRecorderBuilder::createEncode()
{
    IEncode *encode = new FFEncode();
    return encode;
}

IVideoRecorder *FFRecorderBuilder::createrVideoRecorder()
{
    IVideoRecorder *videoRecorder = new FFVideoRecorder();
    return videoRecorder;
}

IAudioEncodeRecorder *FFRecorderBuilder::createrAudioEncodeRecorder()
{
    IAudioEncodeRecorder *audioEncodeRecorder= new FFAudioEncodeRecorder();
    return audioEncodeRecorder;
}

void FFRecorderBuilder::InitHard(void *vm)
{
    FFEncode::InitHard(vm);
}