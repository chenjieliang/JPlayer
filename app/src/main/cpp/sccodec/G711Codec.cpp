#include "G711Codec.h"
#include "../g711/g711a.h"
#include "../g711/g711u.h"
#include "../recorder/RecordParam.h"
#include "../Xlog.h"

void G711aEncoder::create(AudioEncoderCreateParam *createParam,
						stream_audio_encode_callback callback, void *userdata) {
	this->encoderCreateParam = createParam;
	this->streamAudioEncodeCallback = callback;
	this->userdata = userdata;
}

int G711aEncoder::encode(AudioEncodeParam *encodeParam)
{
	if (encodeParam->bufferSize / 2 > encodeParam->bitstreamLength) {
		return -1;
	}
	g711a_Encode((char *)encodeParam->frameBuffer, 
		     (char *)encodeParam->bitstream, 
		     (int)encodeParam->bufferSize, 
		     (int *)&encodeParam->bitstreamLength);
	if (this->streamAudioEncodeCallback) {
		this->streamAudioEncodeCallback(encoderCreateParam->channels,encoderCreateParam->sampleRate,0,encodeParam->pts,encodeParam->bitstream,encodeParam->bitstreamLength,userdata);
	}
	return 0;
}

void G711aEncoder::close()
{

}

int G711aDecoder::decode(AudioDecodeParam *decodeParam)
{	
	if (decodeParam->bitstreamLength * 2 > decodeParam->bufferSize) {
		return -1;
	}

	g711a_Decode((char *)decodeParam->bitstream, 
		     (char *)decodeParam->frameBuffer, 
		     (int)decodeParam->bitstreamLength, 
		     (int *)&decodeParam->bufferSize);
	return 0;
}

void G711uEncoder::create(AudioEncoderCreateParam *createParam,
						stream_audio_encode_callback callback, void *userdata)
{
	this->encoderCreateParam = createParam;
	this->streamAudioEncodeCallback = callback;
	this->userdata = userdata;
}

int G711uEncoder::encode(AudioEncodeParam *encodeParam)
{
	g711u_Encode((char *)encodeParam->frameBuffer, 
		     (char *)encodeParam->bitstream, 
		     (int)encodeParam->bufferSize, 
		     (int *)&encodeParam->bitstreamLength);
	if (this->streamAudioEncodeCallback) {
		this->streamAudioEncodeCallback(encoderCreateParam->channels,encoderCreateParam->sampleRate,0,encodeParam->pts,encodeParam->bitstream,encodeParam->bitstreamLength,userdata);
	}
	return 0;
}

void G711uEncoder::close()
{

}

int G711uDecoder::decode(AudioDecodeParam *decodeParam)
{	
	g711u_Decode((char *)decodeParam->bitstream, 
		     (char *)decodeParam->frameBuffer, 
		     (int)decodeParam->bitstreamLength, 
		     (int *)&decodeParam->bufferSize);
	return 0;
}
