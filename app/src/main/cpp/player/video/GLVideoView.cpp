//
// Created by chenjieliang on 2019/2/12.
//
#include "GLVideoView.h"
#include "../opengl/XTexture.h"
#include "../../XData.h"
#include "../../Xlog.h"
#include "../decode/Capture.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include "libswscale/swscale.h"
}

void GLVideoView::SetRender(void *win)
{
    view = win;
}

void GLVideoView::Close()
{
    mux.lock();
    if(txt)
    {
        txt->Drop();
        txt = 0;
    }
    mux.unlock();
}

void GLVideoView::Render(XData data)
{
    if(!view){
        return;
    }
    if(!txt){
        txt = XTexture::Create();
        txt->Init(view, (XTextureType)data.format);
    }

    txt->Draw(data.datas, data.width, data.height);

    mutex.lock();
    if (this->isCapture) {
        if (data.type == AVFRAME_TYPE) {
            AVFrame *pFrameRGB = av_frame_alloc();
            int PictureSize = avpicture_get_size(AV_PIX_FMT_BGR24, data.width, data.height);
            uint8_t *buf = (uint8_t *) av_malloc(PictureSize);
            if (buf != NULL) {
                avpicture_fill((AVPicture *) pFrameRGB, buf, AV_PIX_FMT_BGR24, data.width,
                               data.height);
                //设置图像转换上下文
                SwsContext *pSwsCtx = sws_getContext(data.width,
                                                     data.height,
                                                     (AVPixelFormat) data.format,
                                                     data.width,
                                                     data.height,
                                                     AV_PIX_FMT_BGR24,
                                                     SWS_BICUBIC,
                                                     NULL, NULL, NULL);
                //转换图像格式，将解压出来的YUV420P的图像转换为BRG24的图像
                sws_scale(pSwsCtx, (const uint8_t **) ((AVFrame *) data.data)->data,
                          ((AVFrame *) data.data)->linesize, 0, data.height, pFrameRGB->data,
                          pFrameRGB->linesize);
                SaveAsBMP(pFrameRGB->data[0], data.width, data.height, this->capturePath);
                sws_freeContext(pSwsCtx);
            }
            av_free(pFrameRGB);
        }
        this->isCapture = false;
    }
    mutex.unlock();
}