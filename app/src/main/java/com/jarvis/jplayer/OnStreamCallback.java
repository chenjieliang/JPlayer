package com.jarvis.jplayer;

public interface OnStreamCallback {

	/*
	 * @brief  码流数据回调函数定义
	 *
	 * @param [out] wnd  - unsigned long  当前的预览/回放句柄
	 * @param [out] size     缓冲区大小
	 *
	 * @return false, 通知内部,播放缓存已满
	 */


	//标准流，帧类型为I帧或P帧或NALU包时调用
	public void onStdVideoCallback(
            int width,
            int height,
            long pts,
            byte[] dataArray,
			int size);


	//标准流,帧类型为A帧时调用
	public void onStdAudioCallback(
            int channels,
            int sps,
            int bps,
            long pts,
            byte[] dataArray,
			int size);


	//私有流调用
	public void onPriStreamCallback(
            int manuType,
            byte[] dataArray,
            long size);



//	public void onEndFrameCallback();     /* FRAME_END */
}
