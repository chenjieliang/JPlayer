//
// Created by chenjieliang on 2020/3/6.
//

#include "G711Decode.h"
#include "../../sccodec/G711Codec.h"
#include "../../sdk.h"
#include "../../Xlog.h"

bool G711Decode::Open(XParameter para, bool isHard)
{
    mux.lock();
    switch (para.streamType) {
        case AudioStreamTypeG711a:
            this->audioDecoder = new G711aDecoder;
            break;
        case AudioStreamTypeG711u:
            this->audioDecoder = new G711uDecoder;
            break;
        default:
            mux.unlock();
            return false;
    }
    this->isAudio = true;
    mux.unlock();
    return true;
}

void G711Decode::Clear()
{
    IDecode::Clear();
    mux.lock();
    if (&decodeData){
        decodeData.Drop();
    }
    mux.unlock();
}

void G711Decode::Close() {
    IDecode::Clear();
    mux.lock();
    if (audioDecoder){
        delete audioDecoder;
        audioDecoder = 0;
    }
    mux.unlock();
}

bool G711Decode::SendPacket(XData pkt)
{
    if(pkt.size <= 0 || !pkt.data)
    {
        return false;
    }
    mux.lock();
    if (!audioDecoder) {
        mux.unlock();
        return false;
    }
    XLOGI("G711Decode decode 000 ");
    if (!decodeData.data) {
        XLOGI("G711Decode Alloc %d ", pkt.size);
        decodeData.Alloc(pkt.size);
    }
    decodeData.size = pkt.size;
    XLOGI("G711Decode decode 111 ");
    AudioDecodeParam * audioDecodeParam;
    audioDecodeParam->bitstream = pkt.data;
    audioDecodeParam->bitstreamLength = pkt.size;
    audioDecodeParam->frameBuffer = decodeData.data;
    audioDecodeParam->bufferSize = pkt.size*2;
    audioDecoder->decode(audioDecodeParam);
    mux.unlock();
    XLOGI("G711Decode decode");
    return true;
}

XData G711Decode::RecvFrame()
{
    mux.lock();
    XData outData;
    if (decodeData.data) {
         outData.Alloc(decodeData.size, (const char *)(decodeData.data));
        //decodeData.Drop();
    }
    mux.unlock();
    XLOGI("G711Decode RecvFrame");
    return outData;
}