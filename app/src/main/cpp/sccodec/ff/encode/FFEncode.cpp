//
// Created by chenjieliang on 2020/1/14.
//

#include "FFEncode.h"
#include "../../../Xlog.h"
#include "../../SCCodecParam.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavcodec/jni.h>
#include "libswscale/swscale.h"
#include <libavutil/opt.h>
}


void FFEncode::InitHard(void *vm)
{
    av_jni_set_java_vm(vm,0);
}

void FFEncode::Clear()
{
    IEncode::Clear();
    mux.lock();
    if(codec)
        avcodec_flush_buffers(codec);
    mux.unlock();
}

void FFEncode::Close()
{
    IEncode::Clear();
    mux.lock();
    if(avPacket)
    {
        av_packet_free(&avPacket);
    }

    if(codec)
    {
        avcodec_close(codec);
        avcodec_free_context(&codec);
    }
    mux.unlock();
}


bool FFEncode::Open(XParameter para, bool isHard)
{
    Close();
    if(!para.para){
        return false;
    }
    AVCodecParameters *p = para.para;

    //1查找解码器
    AVCodec *cd = avcodec_find_encoder(p->codec_id);

    if(!cd)
    {
        cd = avcodec_find_encoder_by_name("h264_mediacodec");
    }

    if(!cd){
        XLOGE("avcodec_find_encoder %d failed", p->codec_id);
        return false;
    }

    XLOGI("avcodec_find_encoder success!");

    mux.lock();
    //2 创建解码上下文 并复制参数
    codec = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(codec, p);
    codec->gop_size = para.gop_size;
    codec->time_base.num = 1;
    codec->time_base.den = para.fps;
    codec->qmin = 10;
    codec->qmax = 51;
    //Optional Param
    //codec->max_b_frames = 0;

    codec->thread_count = 8;
    AVDictionary *param = 0;
    //H.264
    if (codec->codec_id == AV_CODEC_ID_H264) {
        av_dict_set(&param, "tune", "zerolatency", 0);
        av_opt_set(codec->priv_data, "preset", "ultrafast", 0);
        //av_dict_set(&param, "profile", "baseline", 0);
    }
    //3 打开解码器
    int re = avcodec_open2(codec,cd,&param);
    if(re != 0){
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf)-1);
        XLOGE("%s", buf);
        return false;
    }
    if(codec->codec_type == AVMEDIA_TYPE_VIDEO){
        this->isAudio = false;
    }else{
        this->isAudio = true;
    }

    avFrame = av_frame_alloc();

    int size = 0;
    if(codec->codec_type == AVMEDIA_TYPE_VIDEO) {
        size = avpicture_get_size((AVPixelFormat) codec->pix_fmt, codec->width,
                                              codec->height);
        uint8_t *buf = (uint8_t *) av_malloc(size);
        avpicture_fill((AVPicture *) avFrame, buf, (AVPixelFormat) codec->pix_fmt, codec->width,
                       codec->height);
    } else {
        avFrame->nb_samples = codec->frame_size;
        avFrame->format = codec->sample_fmt;
        size = av_samples_get_buffer_size(NULL, codec->channels, codec->frame_size,
                                          codec->sample_fmt, 1);
        uint8_t *frame_buf = (uint8_t *) av_malloc(size);
        avcodec_fill_audio_frame(avFrame, codec->channels, codec->sample_fmt,
                                 (const uint8_t *) frame_buf, size, 1);
    }
    avPacket = av_packet_alloc();
    av_new_packet(avPacket, size);

    mux.unlock();

    XLOGI("avcodec_open2 success!");
    return true;
}

bool FFEncode::SendFrame(XData pkt)
{
    if(pkt.size <= 0 || !pkt.data)
    {
        return false;
    }
    mux.lock();

    if(!codec)
    {
        return false;
    }

    if (pkt.rotateFormat == ROTATE_90_CROP_LT
        || pkt.rotateFormat == ROTATE_270_CROP_LT_MIRROR_LR) {
        codec->width = pkt.height;
        codec->height = pkt.width;
    } else {
        codec->width = pkt.width;
        codec->height = pkt.height;
    }

    int in_y_size = codec->width * codec->height;
    rotateYUV420(pkt.data, pkt.width,pkt.height,avFrame,pkt.width,pkt.height, in_y_size,
                 pkt.rotateFormat);

    avFrame->pts = pkt.pts;
    XLOGI("test 000 FFEncode avFrame->pts %d " , pkt.pts);
    XLOGI("test 111 FFEncode avFrame->pts %d " , avFrame->pts);
    //avFrame->pts = frameCount++;
    //XLOGD("SendPacket AVPacket->pts %d : ",(int) ((AVPacket *)pkt.data)->pts);

    int re = avcodec_send_frame(codec, avFrame);
    mux.unlock();
    if(re != 0){
        XLOGI("avcodec_send_frame fail! this->isAudio : %d",this->isAudio);
        return false;
    }
    XLOGI("FFEncode avcodec_send_frame success");
    return true;
}

//从线程中获取解码结果
XData FFEncode::RecvPacket()
{
    mux.lock();

    if(!codec)
    {
        mux.unlock();
        return XData();
    }

    /*if(!avPacket){
        avPacket = av_packet_alloc();
    }*/
    avPacket->data = NULL;    // packet data will be allocated by the encoder
    avPacket->size = 0;

    int re = avcodec_receive_packet(codec,avPacket);
    if(re != 0)
    {
        mux.unlock();
        return XData();
    }
    XLOGI("FFEncode avcodec_receive_packet");
    XData d;
    if (avPacket)
    {
        //d.Alloc(avPacket->size, (const char *)(avPacket->data));
        d.Alloc(avPacket->size);
        memcpy(d.data,avPacket->data, avPacket->size);
        //d.data = (unsigned char *)avPacket;
        d.type = UCHAR_TYPE;
        d.pts = avPacket->pts;
        //d.pts = frameCount++;
        XLOGI("test 222 FFEncode avFrame->pts %d " , avPacket->pts);
        if (!this->isAudio) {
            d.width = codec->width;
            d.height = codec->height;
        }
       // memcpy(d.frameData,avPacket->data, avPacket->size);
        av_packet_unref(avPacket);
    } else {
        XLOGD("frame null");
    }
    XLOGI("FFEncode avcodec_receive_packet success");
    mux.unlock();
    return d;
}

void FFEncode::rotateYUV420(const uint8_t *picture_buf, int in_width, int in_height,
                                   AVFrame *outFrame, int out_width, int out_height, int in_y_size, int format) {
    AVFrame *pFrame;
    //   y值在H方向开始行
    int y_height_start_index = in_height - out_height;
    //   uv值在H方向开始行
    int uv_height_start_index = y_height_start_index / 2;

    if (format == ROTATE_90_CROP_LT) {

        for (int i = y_height_start_index; i < in_height; i++) {

            for (int j = 0; j < out_width; j++) {

                int index = in_width * i + j;
                uint8_t value = *(picture_buf + index);
                *(outFrame->data[0] + j * out_height +
                  (out_height - (i - y_height_start_index) - 1)) = value;
            }
        }

        for (int i = uv_height_start_index; i < in_height / 2; i++) {
            for (int j = 0; j < out_width / 2; j++) {
                int index = in_width / 2 * i + j;
                uint8_t v = *(picture_buf + in_y_size + index);
                uint8_t u = *(picture_buf + in_y_size * 5 / 4 + index);
                *(outFrame->data[2] + (j * out_height / 2 +
                                       (out_height / 2 - (i - uv_height_start_index) -
                                        1))) = v;
                *(outFrame->data[1] + (j * out_height / 2 +
                                       (out_height / 2 - (i - uv_height_start_index) -
                                        1))) = u;
            }
        }
    } else if (format == ROTATE_0_CROP_LT) {


        for (int i = y_height_start_index; i < in_height; i++) {

            for (int j = 0; j < out_width; j++) {

                int index = in_width * i + j;
                uint8_t value = *(picture_buf + index);

                *(outFrame->data[0] + (i - y_height_start_index) * out_width +
                  j) = value;
            }
        }


        for (int i = uv_height_start_index; i < in_height / 2; i++) {
            for (int j = 0; j < out_width / 2; j++) {

                int index = in_width / 2 * i + j;
                uint8_t v = *(picture_buf + in_y_size + index);

                uint8_t u = *(picture_buf + in_y_size * 5 / 4 + index);
                *(outFrame->data[2] +
                  ((i - uv_height_start_index) * out_width / 2 + j)) = v;
                *(outFrame->data[1] +
                  ((i - uv_height_start_index) * out_width / 2 + j)) = u;
            }
        }
    } else if (format == ROTATE_270_CROP_LT_MIRROR_LR) {

        int y_width_start_index = in_width - out_width;
        int uv_width_start_index = y_width_start_index / 2;

        for (int i = 0; i < out_height; i++) {

            for (int j = y_width_start_index; j < in_width; j++) {

                int index = in_width * (out_height - i - 1) + j;
                uint8_t value = *(picture_buf + index);
                *(outFrame->data[0] + (out_width - (j - y_width_start_index) - 1)
                                      * out_height +
                  i) = value;
            }
        }
        for (int i = 0; i < out_height / 2; i++) {
            for (int j = uv_width_start_index; j < in_width / 2; j++) {
                int index = in_width / 2 * (out_height / 2 - i - 1) + j;
                uint8_t v = *(picture_buf + in_y_size + index);
                uint8_t u = *(picture_buf + in_y_size * 5 / 4 + index);
                *(outFrame->data[2] + (out_width / 2 - (j - uv_width_start_index) - 1)
                                      * out_height / 2 +
                  i) = v;
                *(outFrame->data[1] + (out_width / 2 - (j - uv_width_start_index) - 1)
                                      * out_height / 2 +
                  i) = u;
            }
        }
    }
}
