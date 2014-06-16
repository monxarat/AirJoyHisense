
#ifndef __AJMEDIACLIENTLISTENER_H__
#define __AJMEDIACLIENTLISTENER_H__

class AJMediaClientListener: public AirMediaClientQueryListener
{
public:
    AJMediaClientListener() {}
    virtual ~AJMediaClientListener() {}

	/**
	 * 订阅事件成功
	 */
	virtual void didSubscribeEvent(const std::string &serverIp);

	/**
	 * 取消订阅事件成功
	 */
	virtual void didUnsubscribeEvent(const std::string &serverIp);

	
	/**
	 * 收到事件
	 *  @type           [in] 类型：photo/audio/video
	 *  @action         [in] 行为：play/pause/stop
	 *  @id             [in] id for photo/audio/video
	 *  @url            [in, option] url for photo/audio/video
	 *  @name           [in, option] name for photo/audio/video
	 */
	virtual void recvEvent(const std::string &type,
						   const std::string &action,
						   const std::string &id,
						   const std::string &url,
						   const std::string &name,
						   const std::string &fromIp);


	/**
	 * 超时
	 */
	virtual void didTimeout(const AirJoySessionId sessionId);
	
	/**
	 * 网络错误
	 */
	virtual void didErrorNet(const AirJoySessionId sessionId);


	/**
	 * 处理结果
	 *  @sessionId     [in] 会话ID
	 *  @code          [in] 返回代码
	 *      ResultCodeOk：表示接收方已经收到，并且处理成功。
	 *      ResultCodeErrorNotSupport: 对方不支持此请求。
	 *      ResultCodeErrorTimeout: 超时。
	 */
	virtual void handleResult                  (const AirJoySessionId sessionId,
												ResultCode code);

	/**
	 * 处理结果
	 *  @sessionId      [in] 会话ID
	 *  @appAction      [in] 行为
	 *  @appResult      [in] 数据
	 */
	virtual void handleResult                  (const AirJoySessionId sessionId,
												const std::string &appAction,
												const std::string &appResult);

	/**
	 * 处理获取视频播放进度结果
	 *  @sessionId      [in] 会话ID
	 *  @code           [in] 返回代码，如果是ResultCodeOk：表示rate和position为有效值。
	 *  @rate           [in] 0: 暂停，1: 正常速率播放
	 *  @position       [in] 当前播放视频的时间点，单位为秒
	 *  @return 会话ID
	 */
	virtual void handleGetPlayVideoProgressResult    (const AirJoySessionId sessionId,
													  ResultCode code,
													  float rate,
													  float position);

	/**
	 * 处理获取视频播放信息结果
	 *  @sessionId      [in] 会话ID
	 *  @code           [in] 返回代码，如果是ResultCodeOk：表示rate和position为有效值。
	 *  @info           [in] 回放信息，见 PlayInfo 定义
	 *  @return 会话ID
	 */
	virtual void handleGetPlayVideoInfoResult        (const AirJoySessionId sessionId,
													  ResultCode code,
													  AirJoyPlayBackInfo &info);

	/**
	 * 处理获取音频播放进度结果
	 *  @sessionId      [in] 会话ID
	 *  @code           [in] 返回代码，如果是ResultCodeOk：表示rate和position为有效值。
	 *  @rate           [in] 0: 暂停，1: 正常速率播放
	 *  @position       [in] 当前播放视频的时间点，单位为秒
	 *  @return 会话ID
	 */
	virtual void handleGetPlayAudioProgressResult    (const AirJoySessionId sessionId,
													  ResultCode code,
													  float rate,
													  float position);

	/**
	 * 处理获取音频播放信息结果
	 *  @sessionId      [in] 会话ID
	 *  @code           [in] 返回代码，如果是ResultCodeOk：表示info为有效值。
	 *  @info           [in] 回放信息，见 PlayInfo 定义
	 *  @return 会话ID
	 */
	virtual void handleGetPlayAudioInfoResult        (const AirJoySessionId sessionId,
													  ResultCode code,
													  AirJoyPlayBackInfo &info);

	/**
	 * 处理获取音量结果
	 *  @sessionId      [in] 会话ID
	 *  @code           [in] 返回代码，如果是ResultCodeOk：表示volume为有效值。
	 *  @volume         [in] 音量值（0 -> 100）
	 *  @return 会话ID
	 */
	virtual void handleGetVolumeResult               (const AirJoySessionId sessionId,
													  ResultCode code,
													  int volume);
};

#endif
