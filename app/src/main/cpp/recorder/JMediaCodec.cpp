//
// Created by chenjieliang on 2020/3/5.
//

#include "JMediaCodec.h"
#include "../sccodec/G711Codec.h"
#include "../Xlog.h"

EncodeCodecFactory *EncodeCodecFactory::_factory = NULL;

EncodeCodecFactory *EncodeCodecFactory::factory()
{
    if (NULL == _factory) {
        _factory = new EncodeCodecFactory;
    }

    return _factory;
}

VideoEncoder *EncodeCodecFactory::createVideoEncoder(VideoEncoderCreateParam *createParam, stream_video_encode_callback callback, void *userdata)
{
    VideoEncoder *encoder = NULL;//FFVideoEncoder::create(createParam);
    return encoder;
}

AudioEncoder *EncodeCodecFactory::createAudioEncoder(AudioEncoderCreateParam *createParam, stream_audio_encode_callback callback, void *userdata)
{
    AudioEncoder *encoder = NULL;

    switch (createParam->bitstreamType) {
        case AudioEncoderBitstreamTypeG711a:
            encoder = new G711aEncoder;
            XLOGD("createAudioEncoder G711aEncoder");
            break;
        case AudioEncoderBitstreamTypeG711u:
            encoder = new G711uEncoder;
            XLOGD("createAudioEncoder G711uEncoder");
            break;
        default:
            //encoder = FFAudioEncoder::create(createParam);
            break;
    }
    encoder->open(createParam,callback,userdata);
    return encoder;
}