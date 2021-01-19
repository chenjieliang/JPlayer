#include <jni.h>
#include <string>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "player/builder/FFPlayerBuilder.h"
#include "player/IPlayerProxy.h"
#include "Xlog.h"

#include "sccodec/SCCodecParam.h"
#include "sccodec/SCCodec.h"

extern "C"
{
#include <libavcodec/avcodec.h>
}

static const int EncoderTypeG711a = 1;
static const int EncoderTypeG711u = 2;
static const int EncoderTypeG726 = 3;

// from java layer
static const int DecoderTypeG711a = 1;
static const int DecoderTypeG711u = 2;
static const int DecoderTypeG726 = 3;

// from java layer
static const int DecoderTypeOpenCoreH264 = 1;
static const int DecoderTypeH264 = 2;
static const int DecoderTypeMPEG4 = 3;
static const int DecoderTypeMJPEG = 4;
static const int DecoderTypeMJPEGB = 5;

static const int OutputFormatYUV420P = 1;
static const int OutputFormatARGB32 = 2;
static const int OutputFormatRGB24 = 3;
static const int OutputFormatRGB565 = 4;

static JavaVM *gJavaVM = NULL;

void StreamAudioEncodeCallback(int channels, int sps, int bps, long pts,void *pData, int size, void *pUser)
{
    XLOGI("StreamAudioEncodeCallback called pts %d , size %d", pts , size);
    if (!pData)
    {
        return;
    }

    jobject j_callbackObj = (jobject)pUser;
    if (!j_callbackObj)
    {
        return ;
    }

    JNIEnv *j_env;
    int status;
    bool isAttached = false;
    status = gJavaVM->GetEnv((void**)&j_env, JNI_VERSION_1_4);
    if (status < 0) {
        if (gJavaVM->AttachCurrentThread(&j_env, NULL))////将当前线程注册到虚拟机中
        {
            return;
        }
        isAttached = true;
    }
    jclass j_objclass = j_env->GetObjectClass(j_callbackObj);
    if (!j_objclass)
    {
        XLOGI("Fail to find java object class\n");
        return;
    }
    XLOGI("StreamAudioEncodeCallback success to find java object class\n");
    jbyte *dataByte  = (jbyte *)((char *)pData);
    jbyteArray dataArray = j_env->NewByteArray(size);
    j_env->SetByteArrayRegion(dataArray, 0, size, dataByte);

    jmethodID j_callback = j_env->GetMethodID(j_objclass, "onStdAudioCallback",
                                              "(IIIJ[BI)V");

    j_env->CallVoidMethod(j_callbackObj, j_callback,
                          channels,
                          sps,
                          bps,
                          (jlong)pts,
                          dataArray,
                          size);

    jbyte* ba = j_env->GetByteArrayElements(dataArray, JNI_FALSE);
    j_env->ReleaseByteArrayElements(dataArray, ba, 0);
    if (isAttached) {
        gJavaVM->DetachCurrentThread();
    }
    return;
}

void StreamVideoEncodeCallback(int width, int height, long pts, void *pData, int size, void *pUser)
{
    XLOGI("StreamVideoEncodeCallback called pts %d , size %d", pts , size);
    if (!pData)
    {
        return;
    }

    jobject j_callbackObj = (jobject)pUser;
    if (!j_callbackObj)
    {
        return ;
    }

    JNIEnv *j_env;
    int status;
    bool isAttached = false;
    status = gJavaVM->GetEnv((void**)&j_env, JNI_VERSION_1_4);
    if (status < 0) {
        if (gJavaVM->AttachCurrentThread(&j_env, NULL))////将当前线程注册到虚拟机中
        {
            return;
        }
        isAttached = true;
    }
    //gJavaVM->AttachCurrentThread(&j_env, NULL);
    jclass j_objclass = j_env->GetObjectClass(j_callbackObj);
    if (!j_objclass)
    {
        XLOGI("Fail to find java object class\n");
        return;
    }
    XLOGI("success StreamVideoEncodeCallback to find java object class\n");
    jbyte *dataByte  = (jbyte *)((char *)pData);
    jbyteArray dataArray = j_env->NewByteArray(size);
    j_env->SetByteArrayRegion(dataArray, 0, size, dataByte);

    jmethodID j_callback = j_env->GetMethodID(j_objclass, "onStdVideoCallback",
                                              "(IIJ[BI)V");

    j_env->CallVoidMethod(j_callbackObj, j_callback,
                          width,
                          height,
                          (jlong)pts,
                          dataArray,
                          size);

    jbyte* ba = j_env->GetByteArrayElements(dataArray, JNI_FALSE);
    j_env->ReleaseByteArrayElements(dataArray, ba, 0);
    if (isAttached) {
        gJavaVM->DetachCurrentThread();
    }
    return;
}

extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM *vm, void *res) {
    gJavaVM = vm;
    FFPlayerBuilder::InitHard(vm);
    //IPlayerProxy::Get()->InitNet(vm);

    return JNI_VERSION_1_4;
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_jarvis_jplayer_JPlayer_init(JNIEnv *env, jobject instance) {

    XLOGD("Java_com_jarvis_jplayer_JPlayer_init");
    IPlayerProxy *iPlayerProxy = new IPlayerProxy;
    iPlayerProxy->InitNet();

    return (jlong)iPlayerProxy;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_jarvis_jplayer_JPlayer_doSetPlayView(JNIEnv *env, jobject instance, jint handle, jobject surface) {

    XLOGD("Java_com_jarvis_jplayer_JPlayer_initView %d",handle);
    IPlayerProxy *iPlayerProxy;

    if (-1 == handle) {
        return;
    }
    iPlayerProxy = (IPlayerProxy *)handle;
    //视频显示
    ANativeWindow *win = ANativeWindow_fromSurface(env, surface);
    iPlayerProxy->InitView(win);

}


extern "C"
JNIEXPORT void JNICALL
Java_com_jarvis_jplayer_JPlayer_doPlay(JNIEnv *env, jobject instance, jint handle, jint width,
                                       jint height) {
    XLOGD("Java_com_jarvis_jplayer_JPlayer_doPrepare %d",handle);
    IPlayerProxy *iPlayerProxy;

    if (-1 == handle) {
        return;
    }
    iPlayerProxy = (IPlayerProxy *)handle;

    iPlayerProxy ->Open(width,height);
    iPlayerProxy->Start();

}

extern "C"
JNIEXPORT void JNICALL
Java_com_jarvis_jplayer_JPlayer_doClose(JNIEnv *env, jobject instance, jint handle) {

    XLOGD("Java_com_jarvis_jplayer_JPlayer_doClose %d",handle);
    IPlayerProxy *iPlayerProxy;

    if (-1 == handle) {
        return;
    }
    iPlayerProxy = (IPlayerProxy *)handle;

    iPlayerProxy->Close();


}


extern "C"
JNIEXPORT void JNICALL
Java_com_jarvis_jplayer_JPlayer_doSendVideoFrame(JNIEnv *env, jobject instance, jint handle,
                                                 jint type, jint camType, jint width, jint height,
                                                 jint fps, jlong pts, jbyteArray dataArray_,
                                                 jlong size) {

    XLOGD("Java_com_jarvis_jplayer_JPlayer_doSendVideoFrame %d",handle);

    IPlayerProxy *iPlayerProxy;

    if (-1 == handle) {
        return;
    }

    jbyte *dataArray = env->GetByteArrayElements(dataArray_, NULL);

    iPlayerProxy = (IPlayerProxy *)handle;

    iPlayerProxy->receiveVideoFrame(type, camType, width, height, fps, pts,
                                      (unsigned char *)dataArray, size);

    env->ReleaseByteArrayElements(dataArray_, dataArray, 0);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_jarvis_jplayer_JPlayer_doSendAudioFrame(JNIEnv *env, jobject instance, jint handle,
                                                 jint type, jint channels, jint sps, jint bps,
                                                 jlong pts, jbyteArray dataArray_, jlong size) {

    XLOGD("Java_com_jarvis_jplayer_JPlayer_doSendAudioFrame %d",handle);
    IPlayerProxy *iPlayerProxy;

    if (-1 == handle) {
        return;
    }

    jbyte *dataArray = env->GetByteArrayElements(dataArray_, NULL);

    iPlayerProxy = (IPlayerProxy *)handle;

    iPlayerProxy->receiveAudioFrame(type,channels,sps,bps,pts,(unsigned char *)dataArray,size);

    env->ReleaseByteArrayElements(dataArray_, dataArray, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_jarvis_jplayer_JPlayer_doPause(JNIEnv *env, jobject instance, jint handle, jboolean isPause) {

    XLOGD("Java_com_jarvis_jplayer_JPlayer_doPause %d",isPause);
    IPlayerProxy *iPlayerProxy;

    if (-1 == handle) {
        return;
    }

    iPlayerProxy = (IPlayerProxy *)handle;

    iPlayerProxy->SetPause(isPause);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_jarvis_jplayer_JPlayer_doSetSpeedRate(JNIEnv *env, jobject instance, jint handle,
                                               jfloat rate) {

    XLOGD("Java_com_jarvis_jplayer_JPlayer_doSetSpeedRate %d",rate);
    IPlayerProxy *iPlayerProxy;

    if (-1 == handle) {
        return;
    }

    iPlayerProxy = (IPlayerProxy *)handle;

    iPlayerProxy->setSpeedRate(rate);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_jarvis_jplayer_JPlayer_doCapture(JNIEnv *env, jobject instance, jint handle, jstring path_,
                                          jint type) {
    const char *path = env->GetStringUTFChars(path_, 0);

    XLOGD("Java_com_jarvis_jplayer_JPlayer_doCapture %d",path);
    IPlayerProxy *iPlayerProxy;

    if (-1 == handle) {
        return;
    }

    char* filePath=new char[100];
    strcpy(filePath,path);

    iPlayerProxy = (IPlayerProxy *)handle;

    iPlayerProxy->capture(filePath,type);

    env->ReleaseStringUTFChars(path_, path);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_jarvis_jplayer_JPlayer_doGetPlayTime(JNIEnv *env, jobject instance, jint handle) {

    IPlayerProxy *iPlayerProxy;

    if (-1 == handle) {
        return -1;
    }
    iPlayerProxy = (IPlayerProxy *)handle;

    return iPlayerProxy->PlayTime();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_jarvis_jplayer_JPlayer_doSetSkipFrame(JNIEnv *env, jobject instance, jint handle,
                                               jint skipNum) {

    IPlayerProxy *iPlayerProxy;

    if (-1 == handle) {
        return;
    }
    iPlayerProxy = (IPlayerProxy *)handle;

    iPlayerProxy->setSkipFrame(skipNum);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_jarvis_jplayer_JPlayer_doOpenVolume(JNIEnv *env, jobject instance, jint handle,
                                             jboolean isOpen) {

    IPlayerProxy *iPlayerProxy;

    if (-1 == handle) {
        return;
    }
    iPlayerProxy = (IPlayerProxy *)handle;

    iPlayerProxy->openVolume(isOpen);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_jarvis_jplayer_JCodec_init(JNIEnv *env, jclass type, jstring key_) {
    const char *key = env->GetStringUTFChars(key_, 0);

    // TODO

    env->ReleaseStringUTFChars(key_, key);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_jarvis_jplayer_sccodec_VideoEncoder_init(JNIEnv *env, jclass type, jint bitrate, jint fps,
                                          jint gop, jint width, jint height, jint opaque,
                                          jint filterFormat, jobject onStreamCallback) {
    XLOGD("Java_com_jarvis_jplayer_VideoEncoder_init");
    VideoEncoderCreateParam createParam;
    memset(&createParam, 0x0, sizeof (createParam));

    createParam.bitrate = bitrate;
    createParam.fps = fps;
    createParam.width = width;
    createParam.height = height;
    createParam.gop = gop;
    createParam.is_yuv420p = true;
    createParam.filterFormat = filterFormat;

    jobject streamData = env->NewGlobalRef(onStreamCallback);
    /*IRecorderProxy *iRecorderProxy = new IRecorderProxy;
    iRecorderProxy->Init();
    jobject streamData = env->NewGlobalRef(onStreamCallback);
    iRecorderProxy->createVideoEncoder(&createParam,StreamVideoEncodeCallback, streamData);
    iRecorderProxy->Start();*/

    SCCodecFactory *factory = SCCodecFactory::factory();
    if (NULL == factory) {
        return -1;
    }

    VideoEncoder *encoder  = factory->createVideoEncoder(&createParam,StreamVideoEncodeCallback, streamData);
    if (NULL == encoder) {
        return -1;
    }

    return (jlong)encoder;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_jarvis_jplayer_sccodec_VideoEncoder_encode(JNIEnv *env, jclass type, jint handle,
                                            jbyteArray inBuf_, jint inLen, jlong pts, jint width,
                                            jint height, jint filterFormat) {


    if (-1 == handle) {
        return;
    }
    VideoEncoder *encoder = (VideoEncoder *)handle;
    //IRecorderProxy *iRecorderProxy = (IRecorderProxy *)handle;
    VideoEncodeParam encodeParam;
    uint8_t *bs;

    bs = (uint8_t *)env->GetByteArrayElements(inBuf_, NULL);
    if (NULL == bs) {
        return;
    }

    encodeParam.bitstream = bs;
    encodeParam.bitstreamLength = inLen;
    encodeParam.pts = pts;
    encodeParam.width = width;
    encodeParam.height = height;
    encodeParam.filterFormat = filterFormat;
    //iRecorderProxy->videoEncode(&encodeParam);
    encoder->encode(&encodeParam);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_jarvis_jplayer_sccodec_VideoEncoder_destroy(JNIEnv *env, jclass type, jint handle) {

    if (-1 != handle) {
        //IRecorderProxy *iRecorderProxy = (IRecorderProxy *)handle;
        //iRecorderProxy->Close();
        //delete iRecorderProxy;
        VideoEncoder *encoder = (VideoEncoder *)handle;
        encoder->close();
        delete encoder;
    }

}
extern "C"
JNIEXPORT jint JNICALL
Java_com_jarvis_jplayer_sccodec_AudioEncoder_doInit(JNIEnv *env, jobject instance, jint encoderType,
                                            jint bitRate, jint sampleRate, jint channels,
                                            jint sampleSize, jint frameSize, jint opaque,
                                            jobject onStreamCallback) {
    XLOGD("Java_com_jarvis_jplayer_AudioEncoder_doInit");
    AudioEncoderCreateParam createParam;
    memset(&createParam, 0x0, sizeof (createParam));

    switch (encoderType) {
        case EncoderTypeG711a:
            createParam.bitstreamType = AudioEncoderBitstreamTypeG711a;
            break;
        case EncoderTypeG711u:
            createParam.bitstreamType = AudioEncoderBitstreamTypeG711u;
            break;
        case EncoderTypeG726:
            createParam.bitstreamType = AudioEncoderBitstreamTypeADPCM_G726;
            break;
        default:
            return -1;
    }

    createParam.sampleRate = sampleRate;
    createParam.channels = channels;
    createParam.bitRate = bitRate;
    createParam.frameSize = frameSize;


    jobject streamData = env->NewGlobalRef(onStreamCallback);
    SCCodecFactory *factory = SCCodecFactory::factory();
    if (NULL == factory) {
        return -1;
    }

    AudioEncoder *encoder  = factory->createAudioEncoder(&createParam,StreamAudioEncodeCallback, streamData);
    if (NULL == encoder) {
        return -1;
    }
    XLOGD("Java_com_jarvis_jplayer_AudioEncoder_doInit 111");
    return (jlong)encoder;

}

extern "C"
JNIEXPORT jint JNICALL
Java_com_jarvis_jplayer_sccodec_AudioEncoder_doEncode(JNIEnv *env, jobject instance, jint handle,
                                              jshortArray samplesBuffer, jint bufferSize,
                                              jlong pts, jbyteArray bitstream,
                                              jint bitstreamLength) {
    AudioEncoder *encoder;
    AudioEncodeParam encodeParam;
    uint8_t *bs, *fb;
    jint retVal = -1;

    if (-1 == handle) {
        return -1;
    }

    bs = (uint8_t *)env->GetByteArrayElements(bitstream, NULL);
    if (NULL == bs) {
        goto FAIL_0;
    }

    fb = (uint8_t *)env->GetShortArrayElements(samplesBuffer, NULL);
    if (NULL == fb) {
        goto FAIL_1;
    }

    encodeParam.bitstream = bs;
    encodeParam.bitstreamLength = bitstreamLength;
    encodeParam.frameBuffer = fb;
    encodeParam.bufferSize = bufferSize;

    encoder = (AudioEncoder *)handle;
    if (-1 == encoder->encode(&encodeParam)) {
        goto FAIL_2;
    }

    retVal = 0;

    FAIL_2:
    env->ReleaseShortArrayElements(samplesBuffer, (jshort *)fb, NULL);
    FAIL_1:
    env->ReleaseByteArrayElements(bitstream, (jbyte *)bs, NULL);
    FAIL_0:
    return retVal;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_jarvis_jplayer_sccodec_AudioEncoder_doDestroy(JNIEnv *env, jobject instance, jint handle) {

    if (-1 != handle) {
        delete (AudioEncoder *)handle;
    }

}

extern "C"
JNIEXPORT jint JNICALL
Java_com_jarvis_jplayer_sccodec_AudioDecoder_doInit(JNIEnv *env, jobject instance,
                                                      jint decoderType, jint bitRate,
                                                      jint sampleRate, jint channels,
                                                      jint sampleSize, jint opeque) {

    AudioDecoderCreateParam createParam;
    memset(&createParam, 0x0, sizeof (createParam));

    switch (decoderType) {
        case DecoderTypeG711a:
            createParam.bitstreamType = AudioDecoderBitstreamTypeG711a;
            break;
        case DecoderTypeG711u:
            createParam.bitstreamType = AudioDecoderBitstreamTypeG711u;
            break;
        case DecoderTypeG726:
            createParam.bitstreamType = AudioDecoderBitstreamTypeADPCM_G726;
            break;
        default:
            return -1;
    }

    //createParam.bitstreamType = (AudioDecoderBitstreamType)decoderType;
    createParam.sampleRate = sampleRate;
    createParam.channels = channels;
    createParam.bitRate = bitRate;

    printf("pre get factory\n");

    SCCodecFactory *factory = SCCodecFactory::factory();
    if (NULL == factory) {
        return -1;
    }

    printf("post get factory\n");

    AudioDecoder *decoder  = factory->createAudioDecoder(&createParam);
    if (NULL == decoder) {
        return -1;
    }

    printf("post create decoder\n");

    return (jlong)decoder;

}

extern "C"
JNIEXPORT jint JNICALL
Java_com_jarvis_jplayer_sccodec_AudioDecoder_doDecode(JNIEnv *env, jobject instance, jint handle,
                                                        jbyteArray bitstream, jint bitstreamLength,
                                                        jlong pts, jshortArray samplesBuffer,
                                                        jint bufferSize) {
    AudioDecoder *decoder;
    AudioDecodeParam decodeParam;
    uint8_t *bs, *fb;
    jint retVal = -1;

    if (-1 == handle) {
        return -1;
    }

    bs = (uint8_t *)env->GetByteArrayElements(bitstream, NULL);
    if (NULL == bs) {
        goto FAIL_0;
    }

    fb = (uint8_t *)env->GetShortArrayElements(samplesBuffer, NULL);
    if (NULL == fb) {
        goto FAIL_1;
    }

    decodeParam.bitstream = bs;
    decodeParam.bitstreamLength = bitstreamLength;
    decodeParam.frameBuffer = fb;
    decodeParam.bufferSize = bufferSize;

    decoder = (AudioDecoder *)handle;
    if (-1 == decoder->decode(&decodeParam)) {
        goto FAIL_2;
    }

    retVal = 0;

    FAIL_2:
    env->ReleaseShortArrayElements(samplesBuffer, (jshort *)fb, NULL);
    FAIL_1:
    env->ReleaseByteArrayElements(bitstream, (jbyte *)bs, NULL);
    FAIL_0:
    return retVal;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_jarvis_jplayer_sccodec_AudioDecoder_doDestroy(JNIEnv *env, jobject instance,
                                                         jint handle) {

    if (-1 != handle) {
        delete (AudioDecoder *)handle;
    }

}


extern "C"
JNIEXPORT jint JNICALL
Java_com_jarvis_jplayer_sccodec_VideoDecoder_doInit(JNIEnv *env, jobject instance,
                                                      jint decoderType, jint outputFormat,
                                                      jint frameWidth, jint frameHeight,
                                                      jint opaque) {

    VideoDecoderCreateParam createParam;
    memset(&createParam, 0x0, sizeof (createParam));

    printf("decoderType<2>: %d\n", decoderType);

    switch (decoderType) {
        case DecoderTypeOpenCoreH264:
            createParam.bitstreamType = VideoDecoderBitstreamTypeOpenCoreH264;
            break;
        case DecoderTypeH264:
            createParam.bitstreamType = VideoDecoderBitstreamTypeH264;
            break;
        case DecoderTypeMPEG4:
            createParam.bitstreamType = VideoDecoderBitstreamTypeMPEG4;
            break;
        case DecoderTypeMJPEG:
            createParam.bitstreamType = VideoDecoderBitstreamTypeMJPEG;
            break;
        case DecoderTypeMJPEGB:
            createParam.bitstreamType = VideoDecoderBitstreamTypeMJPEGB;
            break;
        default:
            return -1;
    }

    switch (outputFormat) {
        case OutputFormatYUV420P:
            createParam.outputFormatType = VideoDecoderOutputFormatTypeYUV420P;
            break;
        case OutputFormatARGB32:// 网络字节序
            createParam.outputFormatType = VideoDecoderOutputFormatTypeRGBA;
            break;
        case OutputFormatRGB24:
            createParam.outputFormatType = VideoDecoderOutputFormatTypeRGB24;
            break;
        case OutputFormatRGB565:
            createParam.outputFormatType = VideoDecoderOutputFormatTypeRGB565;
            break;
        default:
            return -1;
    }

    //createParam.bitstreamType = (VideoDecoderBitstreamType)decoderType;
    //createParam.outputFormatType = (VideoDecoderOutputFormatType)outputFormat;
    createParam.frameWidth = frameWidth;
    createParam.frameHeight = frameHeight;

    SCCodecFactory *factory = SCCodecFactory::factory();
    if (NULL == factory) {
        return -1;
    }

    VideoDecoder *decoder = factory->createVideoDecoder(&createParam);
    if (NULL == decoder) {
        return -1;
    }

    return (jlong)decoder;

}

extern "C"
JNIEXPORT jint JNICALL
Java_com_jarvis_jplayer_sccodec_VideoDecoder_doDecode(JNIEnv *env, jobject instance, jint handle,
                                                        jbyteArray bitstream, jint bitstreamLength,
                                                        jlong pts, jbyteArray frameBuffer,
                                                        jint bufferSize) {
    VideoDecoder *decoder;
    VideoDecodeParam decodeParam;
    uint8_t *bs, *fb;
    jint retVal = -1;

    if (-1 == handle) {
        return -1;
    }

    bs = (uint8_t *)env->GetByteArrayElements(bitstream, NULL);
    if (NULL == bs) {
        goto FAIL_0;
    }

    fb = (uint8_t *)env->GetByteArrayElements(frameBuffer, NULL);
    if (NULL == fb) {
        goto FAIL_1;
    }

    decodeParam.bitstream = bs;
    decodeParam.bitstreamLength = bitstreamLength;
    decodeParam.frameBuffer = fb;
    decodeParam.bufferSize = bufferSize;

    decoder = (VideoDecoder *)handle;
    if (-1 == decoder->decode(&decodeParam)) {
        goto FAIL_2;
    }

    retVal = 0;

    FAIL_2:
    env->ReleaseByteArrayElements(frameBuffer, (jbyte *)fb, NULL);
    FAIL_1:
    env->ReleaseByteArrayElements(bitstream, (jbyte *)bs, NULL);
    FAIL_0:
    return retVal;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_jarvis_jplayer_sccodec_VideoDecoder_doDestroy(JNIEnv *env, jobject instance,
                                                         jint handle) {
    if (-1 != handle) {
        delete (VideoDecoder *)handle;
    }
}