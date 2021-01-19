//
// Created by chenjieliang on 2020/1/15.
//

#ifndef JPLAYER_RECORDPARAM_H
#define JPLAYER_RECORDPARAM_H

#include <cstdint>
#include "../sdk.h"
/*

#define ROTATE_0_CROP_LT 0
*/
/**
 * 旋转90度剪裁左上
 *//*

#define ROTATE_90_CROP_LT 1
*/
/**
 * 暂时没处理
 *//*

#define ROTATE_180 2
*/
/**
 * 旋转270(-90)裁剪左上，左右镜像
 *//*

#define ROTATE_270_CROP_LT_MIRROR_LR 3

*/
/*
 * videoEncoder
 *//*

typedef struct {
    int bitstreamType = 1;
    int bitrate = 0;
    int fps = 0;
    int width = 0;
    int height = 0;
    int gop = 0;
    int is_yuv420p = 0;
    int filterFormat = 0;
} VideoEncoderCreateParam;

typedef struct {
    uint8_t *bitstream;
    int bitstreamLength;
    long pts;
    int width;
    int height;
    int filterFormat;
} VideoEncodeParam;

typedef enum {
    AudioEncoderBitstreamTypeUnknown,
    AudioEncoderBitstreamTypeAAC,
    AudioEncoderBitstreamTypeG711a,
    AudioEncoderBitstreamTypeG711u,
    AudioEncoderBitstreamTypeADPCM_G726,
} AudioEncoderBitstreamType;

typedef struct {
    AudioEncoderBitstreamType bitstreamType;
    uint32_t sampleRate;
    uint32_t channels;
    uint32_t bitRate;
    uint32_t frameSize;
} AudioEncoderCreateParam;

typedef struct {
    uint8_t *frameBuffer;
    uint32_t bufferSize;
    uint8_t *bitstream;
    uint32_t bitstreamLength;
    long pts;
} AudioEncodeParam;

typedef struct {
    MediaStreamType streamType;
    uint32_t channels;
    uint32_t sampleRate;
    uint32_t sampleSize;
    uint32_t bitRate;
} AudioDecoderCreateParam;

typedef struct {
    uint8_t *bitstream;
    uint32_t bitstreamLength;
    uint8_t *frameBuffer;
    uint32_t bufferSize;
} AudioDecodeParam;
*/

class RecordParam {

};


#endif //JPLAYER_RECORDPARAM_H
