//
// Created by chenjieliang on 2020/1/9.
//

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "Capture.h"

//把内存对齐定义为2个字节，
//这个可以避免BITMAPFILEHEADER
//出现4字节的对齐而使bmp位图的头出错
#pragma pack(2)

//下面两个结构是位图的结构
typedef struct BITMAPFILEHEADER
{
    u_int16_t bfType;
    u_int32_t bfSize;
    u_int16_t bfReserved1;
    u_int16_t bfReserved2;
    u_int32_t bfOffBits;
}BITMAPFILEHEADER;

typedef struct BITMAPINFOHEADER
{
    u_int32_t biSize;
    u_int32_t biWidth;
    u_int32_t biHeight;
    u_int16_t biPlanes;
    u_int16_t biBitCount;
    u_int32_t biCompression;
    u_int32_t biSizeImage;
    u_int32_t biXPelsPerMeter;
    u_int32_t biYPelsPerMeter;
    u_int32_t biClrUsed;
    u_int32_t biClrImportant;
}BITMAPINFODEADER;


void SaveAsBMP (uint8_t *pRGBBuffer, int width, int height,char *path)
{
    BITMAPFILEHEADER bmpheader;
    BITMAPINFOHEADER bmpinfo;
    FILE *fp;

    unsigned int uiTmp, uiTmp2;
    unsigned char *ucTmp = NULL;
    unsigned char ucRGB;
    int i;

    uiTmp = (width*3+3)/4*4*height;
    uiTmp2 = width*height*3;

    fp=fopen(path, "wb");
    if(fp==NULL)
        return;

    //文件标识"BM"（即0x4D42）表示位图
    bmpheader.bfType = 0x4D42;
    //保留。设置为0
    bmpheader.bfReserved1 = 0;
    //保留。设置为0
    bmpheader.bfReserved2 = 0;
    //从文件开始到位图数据的偏移量（单位：字节）
    bmpheader.bfOffBits = sizeof(bmpheader) + sizeof(bmpinfo);
    //整个文件的大小（单位：字节）
    bmpheader.bfSize = bmpheader.bfOffBits + uiTmp;

    //信息头长度（单位：字节）。典型值为28
    bmpinfo.biSize = 0x28;
    //位图宽度（单位：像素）
    bmpinfo.biWidth = width;
    //位图高度（单位：像素）。若其为正，表示倒向的位图。若为负，表示正向的位图
    bmpinfo.biHeight = height;
    //位图的面数（为1）
    bmpinfo.biPlanes = 1;
    //每个像素的位数
    bmpinfo.biBitCount = 24;
    //压缩说明。0(BI_RGB)表示不压缩
    bmpinfo.biCompression = 0;
    //用字节数表示的位图数据的大小（为4的位数）
    bmpinfo.biSizeImage = uiTmp;
    //水平分辨率（单位：像素/米）
    bmpinfo.biXPelsPerMeter = 0;
    //垂直分辨率（单位：像素/米）
    bmpinfo.biYPelsPerMeter = 0;
    //位图使用的颜色数
    bmpinfo.biClrUsed = 0;
    //重要的颜色数
    bmpinfo.biClrImportant = 0;

    fwrite(&bmpheader,sizeof(bmpheader),1,fp);
    fwrite(&bmpinfo,sizeof(bmpinfo),1,fp);

    //把图像数据倒置
    uint8_t tmp[width*3];//临时数据
    for(int i = 0; i < height/2; i++)
    {
        memcpy(tmp, &(pRGBBuffer[width*i*3]), width*3);
        memcpy(&(pRGBBuffer[width*i*3]), &(pRGBBuffer[width*(height-1-i)*3]), width*3);
        memcpy(&(pRGBBuffer[width*(height-1-i)*3]), tmp, width*3);
    }

    fwrite(pRGBBuffer,width*height*3,1,fp);
    fclose(fp);
}

void SaveAsJPEG (uint8_t *pRGBBuffer, int width, int height,char *path)
{

}