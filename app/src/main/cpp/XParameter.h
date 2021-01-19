//
// Created by chenjieliang on 2019/2/12.
//

#ifndef VIDEOPLAY_XPARAMETER_H
#define VIDEOPLAY_XPARAMETER_H

#include "sdk.h"

struct AVCodecParameters;
struct AVRational;
class XParameter
{
public:
    MediaStreamType streamType;
    AVCodecParameters *para = 0;

    int channels = 2;
    int sample_rate = 0;//44100;
    AVRational *time_base;

    int fps = 25;
    /**
     * the number of pictures in a group of pictures, or 0 for intra_only
     * - encoding: Set by user.
     * - decoding: unused
     */
    int gop_size = 0;

    /**
     * Video only. The aspect ratio (width / height) which a single pixel
     * should have when displayed.
     *
     * When the aspect ratio is unknown / undefined, the numerator should be
     * set to 0 (the denominator may have any value).
     */
    AVRational *sample_aspect_ratio;
};
#endif //VIDEOPLAY_XPARAMETER_H
