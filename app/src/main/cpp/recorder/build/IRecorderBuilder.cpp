//
// Created by chenjieliang on 2020/1/15.
//

#include "IRecorderBuilder.h"

IRecorder *IRecorderBuilder::BuilderRecorder()
{
    IRecorder *recorder = createrRecorder();

    IEncode *vencode = createEncode();
    IEncode *aencode = createEncode();


    IVideoRecorder *videoRecorder = createrVideoRecorder();
    videoRecorder->AddObs(vencode);

    IAudioEncodeRecorder *audioEncodeRecorder = createrAudioEncodeRecorder();
    audioEncodeRecorder->AddObs(aencode);

    recorder->videoRecorder = videoRecorder;
    recorder->audioEncodeRecorder = audioEncodeRecorder;
    recorder->vencoder = vencode;
    recorder->aencoder = aencode;


    return recorder;

}