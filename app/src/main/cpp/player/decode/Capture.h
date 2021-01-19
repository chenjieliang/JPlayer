//
// Created by chenjieliang on 2020/1/9.
//

#ifndef JPLAYER_CAPTURE_H
#define JPLAYER_CAPTURE_H

#include <cstdint>

//保存BMP文件的函数
void SaveAsBMP (uint8_t *pRGBBuffer, int width, int height,char *path);

void SaveAsJPEG (uint8_t *pRGBBuffer, int width, int height,char *path);

#endif //JPLAYER_CAPTURE_H
