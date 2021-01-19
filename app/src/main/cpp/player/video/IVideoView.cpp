//
// Created by chenjieliang on 2019/2/12.
//
#include "IVideoView.h"
#include "../../Xlog.h"

void IVideoView::Update(XData data)
{
    this->Render(data);
}

void IVideoView::Capture(char *path, int type) {
    mutex.lock();
    this->capturePath = path;
    this->captureType = type;
    this->isCapture = true;
    mutex.unlock();
}