//
// Created by chenjieliang on 2020/1/14.
//

#ifndef JPLAYER_IENCODE_H
#define JPLAYER_IENCODE_H


#include <list>
#include "../../thread/IObserver.h"
#include "../../XParameter.h"
#include "../../sdk.h"

class IEncode: public IObserver
{
public:
    //打开解码器
    virtual bool Open(XParameter para, bool isHard =false) = 0;
    virtual void Close() = 0;
    virtual void Clear();

    //future模型 发送数据到线程编码
    virtual bool SendFrame(XData para) = 0;

    //从线程中获取编码结果 再次调用会复用上次空间 线程不安全
    virtual  XData RecvPacket() = 0;

    //由主体notify的数据 阻塞
    virtual  void Update(XData pkt);

    virtual void setVideoStreamCallback(stream_video_encode_callback callback,void *userdata);
    virtual void setAudioStreamCallback(stream_audio_encode_callback callback,void *userdata);

    bool isAudio = false;

    //最大的队列缓冲
    int maxList = 200;

protected:
    virtual void Main();

    //读取缓冲
    std::list<XData> packs;
    std::mutex packsMutex;

    stream_video_encode_callback streamVideoEncodeCallback;
    stream_audio_encode_callback streamAudioEncodeCallback;
    void *userdata;
};


#endif //JPLAYER_IENCODE_H
