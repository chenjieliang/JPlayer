//
// Created by chenjieliang on 2019/2/12.
//

#include "IDecode.h"
#include "../../Xlog.h"

//当前时间戳 clock
long long GetNowMs()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    int sec = tv.tv_sec%360000;
    long long t = sec*1000+tv.tv_usec/1000;
    return t;
}
long long start = 0;
int frameCount = 0;

//由主体notify的数据
void IDecode::Update(XData pkt)
{
    if(pkt.isAudio != isAudio)
    {
        return;
    }

    while (!isExit && !IsPause()){
        packsMutex.lock();

        //阻塞
        XLOGE("IDecode::isAudio %d packs.size() %d",isAudio, packs.size());
        if(packs.size() < maxList){
            //生产者
            packs.push_back(pkt);
            packsMutex.unlock();
            break;
        }
        packsMutex.unlock();
        XSleep(1);
    }
}

void IDecode::Clear()
{
    packsMutex.lock();
    while (!packs.empty())
    {
        packs.front().Drop();
        packs.pop_front();
    }
    packsMutex.unlock();
    pts = 0;
    synPts = 0;
}

void IDecode::Main()
{
    while(!isExit)
    {
        if(IsPause())
        {
            XSleep(2);
            continue;
        }
        packsMutex.lock();
        //XLOGE("IDecode::isAudio %d packs.size() %d",isAudio, packs.size());
        //判断音视频同步
        if(!isAudio && synPts >0){
            //c
            XLOGD("synPts %d pts %d ",synPts,pts);
            if(synPts < pts){
                packsMutex.unlock();
                XSleep(1);
                continue;
            }
        }

        if(packs.empty()){
            packsMutex.unlock();
            XSleep(1);
            continue;
        }
        if (start == 0) {
            start = GetNowMs();
        }
        //取出packet 消费者
        XData pack = packs.front();
        packs.pop_front();
        packsMutex.unlock();
        //发送数据到解码线程， 一个数据包，可能解码多个结果
        //XLOGE("SendPacket 1");
        if(this->SendPacket(pack))
        {
            //XLOGE("SendPacket 2");
            while(!isExit)
            {
                //获取解码数据
                XData frame = RecvFrame();
                if(!frame.data){
                    //XLOGE("SendPacket break");
                    break;
                }
                if (!isAudio) {
                    frameCount++;
                    //这里是测试每秒解码的帧数  每三秒解码多少帧
                    if(GetNowMs() - start >= 3000)
                    {
                        XLOGE("now decode fps is %d", frameCount/3);
                        start = GetNowMs();
                        frameCount = 0;
                    }

                }
                //XLOGE("SendPacket 3");
                this->Notify(frame);
            }
        }
        //XLOGE("SendPacket 5");
        pack.Drop();
        //packsMutex.unlock();
        //XLOGE("SendPacket 6");
    }
}

void IDecode::Capture(char *path, int type) {
    this->capturePath = path;
    this->captureType = type;
    this->isCapture = true;
}
