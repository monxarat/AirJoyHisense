
#ifndef __AJMEDIACLIENTLISTENER_H__
#define __AJMEDIACLIENTLISTENER_H__

class AJMediaClientListener: public AirMediaClientQueryListener
{
public:
    AJMediaClientListener() {}
    virtual ~AJMediaClientListener() {}

	/**
	 * �����¼��ɹ�
	 */
	virtual void didSubscribeEvent(const std::string &serverIp);

	/**
	 * ȡ�������¼��ɹ�
	 */
	virtual void didUnsubscribeEvent(const std::string &serverIp);

	
	/**
	 * �յ��¼�
	 *  @type           [in] ���ͣ�photo/audio/video
	 *  @action         [in] ��Ϊ��play/pause/stop
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
	 * ��ʱ
	 */
	virtual void didTimeout(const AirJoySessionId sessionId);
	
	/**
	 * �������
	 */
	virtual void didErrorNet(const AirJoySessionId sessionId);


	/**
	 * ������
	 *  @sessionId     [in] �ỰID
	 *  @code          [in] ���ش���
	 *      ResultCodeOk����ʾ���շ��Ѿ��յ������Ҵ���ɹ���
	 *      ResultCodeErrorNotSupport: �Է���֧�ִ�����
	 *      ResultCodeErrorTimeout: ��ʱ��
	 */
	virtual void handleResult                  (const AirJoySessionId sessionId,
												ResultCode code);

	/**
	 * ������
	 *  @sessionId      [in] �ỰID
	 *  @appAction      [in] ��Ϊ
	 *  @appResult      [in] ����
	 */
	virtual void handleResult                  (const AirJoySessionId sessionId,
												const std::string &appAction,
												const std::string &appResult);

	/**
	 * �����ȡ��Ƶ���Ž��Ƚ��
	 *  @sessionId      [in] �ỰID
	 *  @code           [in] ���ش��룬�����ResultCodeOk����ʾrate��positionΪ��Чֵ��
	 *  @rate           [in] 0: ��ͣ��1: �������ʲ���
	 *  @position       [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
	 *  @return �ỰID
	 */
	virtual void handleGetPlayVideoProgressResult    (const AirJoySessionId sessionId,
													  ResultCode code,
													  float rate,
													  float position);

	/**
	 * �����ȡ��Ƶ������Ϣ���
	 *  @sessionId      [in] �ỰID
	 *  @code           [in] ���ش��룬�����ResultCodeOk����ʾrate��positionΪ��Чֵ��
	 *  @info           [in] �ط���Ϣ���� PlayInfo ����
	 *  @return �ỰID
	 */
	virtual void handleGetPlayVideoInfoResult        (const AirJoySessionId sessionId,
													  ResultCode code,
													  AirJoyPlayBackInfo &info);

	/**
	 * �����ȡ��Ƶ���Ž��Ƚ��
	 *  @sessionId      [in] �ỰID
	 *  @code           [in] ���ش��룬�����ResultCodeOk����ʾrate��positionΪ��Чֵ��
	 *  @rate           [in] 0: ��ͣ��1: �������ʲ���
	 *  @position       [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
	 *  @return �ỰID
	 */
	virtual void handleGetPlayAudioProgressResult    (const AirJoySessionId sessionId,
													  ResultCode code,
													  float rate,
													  float position);

	/**
	 * �����ȡ��Ƶ������Ϣ���
	 *  @sessionId      [in] �ỰID
	 *  @code           [in] ���ش��룬�����ResultCodeOk����ʾinfoΪ��Чֵ��
	 *  @info           [in] �ط���Ϣ���� PlayInfo ����
	 *  @return �ỰID
	 */
	virtual void handleGetPlayAudioInfoResult        (const AirJoySessionId sessionId,
													  ResultCode code,
													  AirJoyPlayBackInfo &info);

	/**
	 * �����ȡ�������
	 *  @sessionId      [in] �ỰID
	 *  @code           [in] ���ش��룬�����ResultCodeOk����ʾvolumeΪ��Чֵ��
	 *  @volume         [in] ����ֵ��0 -> 100��
	 *  @return �ỰID
	 */
	virtual void handleGetVolumeResult               (const AirJoySessionId sessionId,
													  ResultCode code,
													  int volume);
};

#endif
