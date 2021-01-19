//
// Created by chenjieliang on 2020/3/6.
//

#ifndef JPLAYER_G711DECODE_H
#define JPLAYER_G711DECODE_H


#include "IDecode.h"
#include "../../sccodec/SCCodec.h"

class G711Decode : public IDecode
{
public:
    virtual bool Open(XParameter para, bool isHard = false);
    virtual void Close();
    virtual void Clear();

    //future模型 发送数据到线程解码
    virtual bool SendPacket(XData pkt);

    //从线程中获取解码结果
    virtual XData RecvFrame();

private:
    AudioDecoder *audioDecoder;
    XData decodeData;
    std::mutex mux;
};


#endif //JPLAYER_G711DECODE_H
