cmake_minimum_required(VERSION 3.4.1)

#添加头文件路径（相对于本文件路径）
include_directories(include
                    include/soundtouch)

#设置ffmpeg库所在路径的变量
set(FF ${CMAKE_CURRENT_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})
add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${FF}/libavcodec.so)

add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${FF}/libavformat.so)

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${FF}/libavutil.so)

add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${FF}/libswscale.so)

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${FF}/libswresample.so)

add_library(avfilter SHARED IMPORTED)
set_target_properties(avfilter PROPERTIES IMPORTED_LOCATION ${FF}/libavfilter.so)

#add_library(x264 SHARED IMPORTED)
#set_target_properties(x264 PROPERTIES IMPORTED_LOCATION ${FF}/libx264.so)

#add_library(fdk-aac SHARED IMPORTED)
#set_target_properties(fdk-aac PROPERTIES IMPORTED_LOCATION ${FF}/libfdk-aac.so)

add_library(jplayer-lib
            SHARED
            src/main/cpp/jplayer-lib.cpp
             src/main/cpp/g711/g711_cq.cpp
             src/main/cpp/g711/g711a.cpp
             src/main/cpp/g711/g711u.cpp
            src/main/cpp/thread/XThread.cpp
            src/main/cpp/thread/IObserver.cpp
            src/main/cpp/Xlog.cpp
            src/main/cpp/XData.cpp
            src/main/cpp/XParameter.cpp



            src/main/cpp/player/demux/IDemux.cpp
            src/main/cpp/player/demux/FFDemux.cpp
            src/main/cpp/player/decode/IDecode.cpp
            src/main/cpp/player/decode/FFDecode.cpp
            src/main/cpp/player/decode/G711Decode.cpp
            src/main/cpp/player/decode/Capture.cpp
            src/main/cpp/player/resample/IResample.cpp
            src/main/cpp/player/resample/FFResample.cpp
            src/main/cpp/player/audio/IAudioPlay.cpp
            src/main/cpp/player/audio/SLAudioPlay.cpp
            src/main/cpp/player/video/GLVideoView.cpp
            src/main/cpp/player/video/IVideoView.cpp
            src/main/cpp/player/opengl/XEGL.cpp
            src/main/cpp/player/opengl/XShader.cpp
            src/main/cpp/player/opengl/XTexture.cpp
            src/main/cpp/player/builder/IPlayerBuilder.cpp
            src/main/cpp/player/builder/FFPlayerBuilder.cpp
            src/main/cpp/player/IPlayer.cpp
            src/main/cpp/player/IPlayerProxy.cpp
            src/main/cpp/player/filter/STFilter.cpp
            src/main/cpp/player/ts/ITSVideo.cpp
            src/main/cpp/player/ts/FFTSVideo.cpp
            src/main/cpp/player/ts/ITSAudio.cpp
            src/main/cpp/player/ts/FFTSAudio.cpp
            src/main/cpp/player/ts/G711TSAudio.cpp
            src/main/cpp/player/decode/DecodeInit.cpp

            #src/main/cpp/recorder/build/IRecorderBuilder.cpp
            #src/main/cpp/recorder/build/FFRecorderBuilder.cpp
            #src/main/cpp/recorder/video/IVideoRecorder.cpp
            #src/main/cpp/recorder/video/FFVideoRecorder.cpp
            #src/main/cpp/recorder/audio/IAudioEncodeRecorder.cpp
            #src/main/cpp/recorder/audio/FFAudioEncodeRecorder.cpp
            #src/main/cpp/recorder/encode/IEncode.cpp
            #src/main/cpp/recorder/encode/FFEncode.cpp
            #src/main/cpp/recorder/callback/StreamCallback.cpp
            #src/main/cpp/recorder/JMediaCodec.cpp
            #src/main/cpp/recorder/IRecorder.cpp
            #src/main/cpp/recorder/IRecorderProxy.cpp
            #src/main/cpp/recorder/RecordParam.cpp

            src/main/cpp/sccodec/FFCodec.cpp
            src/main/cpp/sccodec/G711Codec.cpp
            src/main/cpp/sccodec/SCCodec.cpp
            src/main/cpp/sccodec/SCCodecParam.cpp
            src/main/cpp/sccodec/ff/FFAudioEncoder.cpp
            src/main/cpp/sccodec/ff/FFVideoEncoder.cpp
            src/main/cpp/sccodec/ff/encode/IEncode.cpp
            src/main/cpp/sccodec/ff/encode/FFEncode.cpp

            )

# 添加源文件
add_library( soundtouch

        STATIC

        # library
        src/main/cpp/player/SoundTouch/sources/AAFilter.cpp
        src/main/cpp/player/SoundTouch/sources/BPMDetect.cpp
        src/main/cpp/player/SoundTouch/sources/cpu_detect_x86.cpp
        src/main/cpp/player/SoundTouch/sources/FIFOSampleBuffer.cpp
        src/main/cpp/player/SoundTouch/sources/FIRFilter.cpp
        src/main/cpp/player/SoundTouch/sources/InterpolateCubic.cpp
        src/main/cpp/player/SoundTouch/sources/InterpolateLinear.cpp
        src/main/cpp/player/SoundTouch/sources/InterpolateShannon.cpp
        src/main/cpp/player/SoundTouch/sources/mmx_optimized.cpp
        src/main/cpp/player/SoundTouch/sources/PeakFinder.cpp
        src/main/cpp/player/SoundTouch/sources/RateTransposer.cpp
        src/main/cpp/player/SoundTouch/sources/SoundTouch.cpp
        src/main/cpp/player/SoundTouch/sources/sse_optimized.cpp
        src/main/cpp/player/SoundTouch/sources/TDStretch.cpp

        # wrapper
        src/main/cpp/player/SoundTouch/SoundTouchWrapper.cpp
        )

find_library(log-lib
              log )
target_link_libraries(jplayer-lib
                      OpenSLES EGL
                      GLESv2
                      OpenSLES
                      android
                      avcodec avformat avutil swscale swresample avfilter
                      soundtouch
                       ${log-lib} )