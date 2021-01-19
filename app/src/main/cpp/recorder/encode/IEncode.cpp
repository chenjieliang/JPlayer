//
// Created by chenjieliang on 2020/1/14.
//

#include "IEncode.h"
#include "../../thread/XThread.h"
#include "../../XData.h"
#include "../../Xlog.h"

//由主体notify的数据
void IEncode::Update(XData pkt)
{
    if(pkt.isAudio != isAudio)
    {
        return;
    }

    if(this->SendFrame(pkt))
    {
        while(!isExit)
        {
            //获取解码数据
            XData frame = RecvPacket();
            if(!frame.data){
                break;
            }
            //this->Notify(frame);
            if (isAudio) {
                if (streamAudioEncodeCallback) {
                    streamAudioEncodeCallback(1, 8000, 25, frame.pts, frame.data, frame.size,
                                              userdata);
                }
            } else {
                if (streamVideoEncodeCallback) {
                    streamVideoEncodeCallback(frame.width, frame.height, frame.pts, frame.data,
                                              frame.size, userdata);
                }
            }
        }
    }
    //pkt.Drop();
    /*while (!isExit){
        packsMutex.lock();

        //阻塞
        XLOGE("IEncode::isAudio %d packs.size() %d",isAudio, packs.size());
        if(packs.size() < maxList){
            //生产者
            packs.push_back(pkt);
            packsMutex.unlock();
            break;
        }
        packsMutex.unlock();
        XLOGE("0000000000");
        XSleep(1);
    }*/
}

void IEncode::Clear()
{
    while (!packs.empty())
    {
        packs.front().Drop();
        packs.pop_front();
    }
}

void IEncode::Main()
{
    /*while(!isExit)
    {
        if(IsPause())
        {
            XSleep(2);
            continue;
        }
        packsMutex.lock();

        if(packs.empty()){
            packsMutex.unlock();
            XSleep(1);
            continue;
        }
        //取出packet 消费者
        XData pack = packs.front();
        packs.pop_front();
        packsMutex.unlock();

        //发送数据到编码线程， 一个数据包，可能编码多个结果
        if(this->SendFrame(pack))
        {
            while(!isExit)
            {
                //获取解码数据
                XData frame = RecvPacket();
                if(!frame.data){
                    break;
                }
                this->Notify(frame);
            }
        }
    }*/
}

void IEncode::setVideoStreamCallback(stream_video_encode_callback callback, void *userdata)
{
    this->streamVideoEncodeCallback = callback;
    this->userdata = userdata;
}

void IEncode::setAudioStreamCallback(stream_audio_encode_callback callback, void *userdata)
{
    this->streamAudioEncodeCallback = callback;
    this->userdata = userdata;
}