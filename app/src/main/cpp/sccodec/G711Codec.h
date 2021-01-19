#ifndef _G711_CODEC_H_
#define _G711_CODEC_H_


#include "SCCodec.h"

class G711aEncoder : public AudioEncoder
{
public:
	virtual ~G711aEncoder() {}
	virtual void create(AudioEncoderCreateParam *createParam, stream_audio_encode_callback callback, void *userdata);
	virtual int encode(AudioEncodeParam *encodeParam);
    virtual void close();
private:
	AudioEncoderCreateParam *encoderCreateParam ;
	stream_audio_encode_callback streamAudioEncodeCallback;
	void *userdata;
};

class G711aDecoder : public AudioDecoder
{
public:
	virtual ~G711aDecoder() {}
	virtual int decode(AudioDecodeParam *decodeParam);
};

class G711uEncoder : public AudioEncoder
{
public:
	virtual ~G711uEncoder() {}
	virtual void create(AudioEncoderCreateParam *createParam, stream_audio_encode_callback callback, void *userdata);
	virtual int encode(AudioEncodeParam *encodeParam);
    virtual void close();
private:
	AudioEncoderCreateParam *encoderCreateParam ;
	stream_audio_encode_callback streamAudioEncodeCallback;
	void *userdata;
};

class G711uDecoder : public AudioDecoder
{
public:
	virtual ~G711uDecoder() {}
	virtual int decode(AudioDecodeParam *decodeParam);
};

#endif//_G711_CODEC_H_
