/*************************************************
**
** 文件名：DVRSDK.h
**
** 发行商：Microvision Copyright
**
** 发行时间：2003.05.01
**
** 最后更新：2006.01.08
**
** 说明：DVRSDK.dll API 函数的声明文件, ver 3.6
**
**************************************************/

#ifndef ___DVR_SDK_H__
#define ___DVR_SDK_H__

// 定义 DVRSDK 多路透明色
#define	TRANSPARENCE_COLOR	RGB(238,0,238)

// 视频输入端口号选择
typedef enum
{
	VInput_AV1				= 0x0,
	VInput_AV2				= 0x1,
	VInput_AV3				= 0x2,
	VInput_AV4				= 0x3
} VInput;

// 视频流类型参量
typedef enum
{
	VSubType_RGB32			= 0x0,
	VSubType_RGB24			= 0x1,
	VSubType_RGB16			= 0x2,
	VSubType_RGB15			= 0x3,
	VSubType_YUY2			= 0x4,
	VSubType_BTYUV			= 0x5,
	VSubType_Y8				= 0x6,
	VSubType_RGB8			= 0x7,
	VSubType_PL422			= 0x8,
	VSubType_PL411			= 0x9,
	VSubType_YUV12			= 0xA,
	VSubType_YUV9			= 0xB,
	VSubType_RAW			= 0xE
} VSubType;

// 视频制式参量
typedef enum
{
	VStandard_NTSC			= 0x1,
	VStandard_PAL			= 0x3,
	VStandard_SECAM			= 0x6
} VStandard;

// 录像视频制式
typedef enum
{
	VRecord_NTSC			= 0x0,
	VRecord_PAL				= 0x1,
} VRecStandard;

// 视频属性参量
typedef enum
{
	VProperty_Brightness	= 0x00,             // 范围:	 0 ~ 510	最佳：255
	VProperty_Contrast		= 0x01,				// 范围:	 0 ~ 510	最佳：255
	VProperty_Hue			= 0x02,				// 范围:	 0 ~ 510	最佳：255
	VProperty_Saturation	= 0x03,				// 范围:	 0 ~ 510	最佳：300
} VProperty;

// 不规则显示参数设置
typedef struct tagCardRect
{
	RECT	rcWndPos;							// 窗口位置
	DWORD	dwCardID;							// 卡号
	DWORD	reserve1;							// 扩展保留
	DWORD	reserve2;							// 扩展保留
} CAP_WND_POS;

// MPEG4 录制参数设置
typedef struct tagProperty
{
	DWORD RecProperty_BitRate;					// 位率 (范围：56KBPS ~ 10MBPS)
	DWORD RecProperty_KeyFrameInterval;			// 关键帧间隔 (要求：大于等于帧率)
	DWORD RecProperty_FrameRate;				// 帧率 (范围：1 ~ 25(PAL)/30(NTSC)帧)
	VRecStandard RecProperty_VStandard;			// 视频制式 (取值：0 为NTSC, 1 为PAL)
} RecordMPEGPara;

// 捕捉模式定义用于 HxnDVR_SetCaptureFile () 函数接口
// 例如：要捕捉只有视频的 MPEG4 文件和捕捉音视流保存为 .WAV 文件，请使用
// 参数：CaptureMode_VO_MPEG4_FILE | CaptureMode_AO_WAV_FILE
#define	CaptureMode_VA_MPEG4_FILE	0xffffffff	// OCX取值为：1  捕捉音视频复合的 MPEG4 文件
#define	CaptureMode_VO_MPEG4_FILE	0xfff70000	// OCX取值为：2  捕捉只有视频的 MPEG4 文件
#define	CaptureMode_VO_YUY2_FILE	0xfff00000	// OCX取值为：4  捕捉未压缩 YUY2 文件
#define	CaptureMode_VO_USER			0xff000000	// OCX取值为：8  将捕捉到的视频流提供给用户
#define	CaptureMode_AO_WAV_USER		0x000000ff	// OCX取值为：16 将捕捉到的未压缩音频流提供给用户
#define	CaptureMode_AO_ADPCM_USER	0x00000fff	// OCX取值为：32 将捕捉到的压缩音频ADPCM流提供给用户
#define	CaptureMode_AO_WAV_FILE		0x0000fff7	// OCX取值为：64 捕捉音视流保存为 .WAV 文件

//  定义 MPEG4 捕捉时工作模式
typedef enum
{
	MPEG4Mode_ToFile			= 0x00,			// 只能用于捕捉成文件
	MPEG4Mode_Both				= 0x01,			// 可以捕捉成文件也能网传
	MPEG4Mode_ToTransmit		= 0x02			// 只能用于网传
} MPEG4Mode;

//  视频压缩引擎的信息
//  用于函数：HxnDVR_EnumVidCompressor ()
typedef struct VIDCOMPRESSORINFO
{
	char szName[128];
} VidCompressorInfo;

//  音频压缩引擎的信息
//  用于函数：HxnDVR_EnumAudCompressor()
typedef struct AUDCOMPRESSORINFO
{
	char szName[128];
} AudCompressorInfo;

//定义用户消息
#define		WM_Microvision_WINMSG		WM_USER + 0x300

// 系统函数 - 对所有DVR设备全局堆进行初始化
DWORD WINAPI HxnDVR_Init ( HWND hWnd );
// 系统函数 - 对所有DVR设备全局堆占用资源进行释放
void  WINAPI HxnDVR_UnInit ( void );
// 系统函数 - 获取DVR设备的实际总数量
DWORD WINAPI HxnDVR_GetDeviceAmount ( void );
// 系统函数 - 启动指定卡号的音视频设备, 可多次调用选择视频输入端口号
BOOL  WINAPI HxnDVR_ConnectDevice ( DWORD dwCardID, VInput m_Insel = VInput_AV1 );
// 系统函数 - 关闭指定卡号的音视频设备
void  WINAPI HxnDVR_DisconnectDevice ( DWORD dwCardID );
// 系统函数 - 判断当前设备是否已经启动
BOOL  WINAPI HxnDVR_IsConnected ( DWORD dwCardID );
// 系统函数 - 设置捕捉视频流类型
BOOL  WINAPI HxnDVR_SetVSubType ( DWORD dwCardID, VSubType subType );
// 系统函数 - 设置当前设备要显示视频制式
BOOL  WINAPI HxnDVR_SetVStandard ( DWORD dwCardID, VStandard vformat );
// 系统函数 - 设置捕捉视频属性(亮度、对比度、色度、饱和度和锐度(清晰度)等)
BOOL  WINAPI HxnDVR_SetVPropertyValue ( DWORD dwCardID, VProperty pro, DWORD dwValue );
// 系统函数 - 获取捕捉视频尺寸大小
void  WINAPI HxnDVR_GetVidCapSize ( DWORD dwCardID, DWORD * dwCapWidth, DWORD * dwCapHeight );
// 系统函数 - 设置捕捉视频尺寸大小
BOOL  WINAPI HxnDVR_SetVidCapSize ( DWORD dwCardID, DWORD dwCapWidth, DWORD dwCapHeight );
// 系统函数 - 获取显示、快照视频尺寸大小 (SDK7000 有效)
void  WINAPI HxnDVR_GetVidPreSize ( DWORD dwCardID, DWORD * dwCapWidth, DWORD * dwCapHeight );
// 系统函数 - 设置显示、快照视频尺寸大小 (SDK7000 有效)
BOOL  WINAPI HxnDVR_SetVidPreSize ( DWORD dwCardID, DWORD dwCapWidth, DWORD dwCapHeight );
// 系统函数 - 获取当前使用视频压缩卡的ID号, 必须先调用 HxnDVR_Init
BOOL  WINAPI HxnDVR_GetCurrentCardID ( BYTE RegID[32] );
// 系统函数 - 将YUY2数据转换成RGB24数据
BOOL  WINAPI HxnDVR_YUY2RGB24 ( void * dstBuf, void * srcBuf, DWORD dwWidth, DWORD dwHigh );
// 系统函数 - 后台播放一个指定WAV文件
BOOL  WINAPI HxnDVR_PlaySoundFromFile ( HWND m_hWnd,			 // 播放窗口句柄
									   char *m_filename,		 // 要播放的WAV文件名
									   DWORD dwPrimaryChannels,  // 音频通道数如：2
									   DWORD dwPrimaryFreq,		 // 频率如：22050
									   DWORD dwPrimaryBitRate ); // 位率如：16
// 系统函数 - 允许镜像
BOOL  WINAPI HxnDVR_EnableMirror ( DWORD dwCardID, BOOL m_isMirror );
// 系统函数 - 允许倒置
BOOL  WINAPI HxnDVR_EnableInversion ( DWORD dwCardID, BOOL m_isInversion );

// 显示函数 - 是否显示指定卡号的视频图像
BOOL  WINAPI HxnDVR_ShowWindow ( DWORD dwCardID, BOOL isShow );
// 显示函数 - 当只需要显示单路尺寸高度大于288请使用此函数
BOOL  WINAPI HxnDVR_SetOneScreenAnomaly ( DWORD dwCardID, HWND hWnd, RECT *rc );
// 显示函数 - 当只需按网格方式显示四路每路视频尺寸高度大于288请使用此函数
BOOL  WINAPI HxnDVR_SetFourScreenAnomaly ( DWORD dwCardID1, DWORD dwCardID2, DWORD dwCardID3, DWORD dwCardID4, HWND hWnd, RECT *rc );
// 显示函数 - 设定指定卡号的视频图像位置，并由 HxnDVR_ShowWindow 决定是否显示
BOOL  WINAPI HxnDVR_SetWindowPos ( DWORD dwCardID, HWND hWnd, RECT *rc );
// 显示函数 - 全屏显示或恢复正常显示指定卡号的视频设备
BOOL  WINAPI HxnDVR_SetFullScreen ( DWORD dwCardID, BOOL isFull );
// 显示函数 - 设置不规则窗口函数
BOOL  WINAPI HxnDVR_SetScreenAnomaly ( RECT			* rcLogic,
									   RECT			* rcWndPos,
									   CAP_WND_POS	* pCardPos,
									   DWORD		dwCardIDCount );
// 显示函数 - 设置Logo内容(如场景说明等)
BOOL WINAPI HxnDVR_SetLogoText ( DWORD dwCardID, LPCTSTR szLogoText );
// 显示函数 - 显示或隐藏Logo
void WINAPI HxnDVR_ShowLogo ( DWORD dwCardID, BOOL bShow, int x, int y );
// 显示函数 - 显示或隐藏时间
void WINAPI HxnDVR_ShowTime ( DWORD dwCardID, BOOL bShow, int x, int y );
// 显示函数 - 显示或隐藏日期
void WINAPI HxnDVR_ShowDate ( DWORD dwCardID, BOOL bShow, int x, int y );
// 显示函数 - 画质显示增强模式, 选择范围(0,1,...14)
BOOL WINAPI HxnDVR_ShowInfocus ( DWORD dwCardID, int iMode );
// 显示函数 - 图像卜莱兹显示模式, 选择范围(0,1,...14)
BOOL WINAPI HxnDVR_ShowInBlaze ( DWORD dwCardID, int iMode );
// 显示函数 - 显示或不显示视频边框区域
void WINAPI HxnDVR_DisplaySideForVideoScreen ( DWORD dwCardID, DWORD Color, BOOL isShow );
// 显示函数 - 设置视频显示帧率
BOOL WINAPI HxnDVR_SetDisplayFrame ( DWORD dwCardID, int iFrame );
// 显示函数 - 判断当前通道是否有视频图像输入
BOOL WINAPI HxnDVR_IsVideoSignalLocked ( DWORD dwCardID );

// 录像函数 - 设置要录制的文件名称和捕捉模式, 其中 dwMode 参考捕捉模式定义
BOOL  WINAPI HxnDVR_SetCaptureFile ( DWORD dwCardID, char *filename, DWORD dwMode );
// 录像函数 - 开始捕捉流或文件
BOOL  WINAPI HxnDVR_StartCapture ( DWORD dwCardID );
// 录像函数 - 停止捕捉流或文件
void  WINAPI HxnDVR_StopCapture ( DWORD dwCardID );
// 录像函数 - 判断指定卡是否正在进行捕捉
BOOL  WINAPI HxnDVR_IsVideoCapture	( DWORD dwCardID );
// 录像函数 - 设置 MPEG4 压缩流的录制参数和捕捉模式属性
void  WINAPI HxnDVR_SetMPEG4Property ( DWORD dwCardID, RecordMPEGPara * mpgPara, MPEG4Mode dwMode);
// 录像函数 - 获取 MPEG4 压缩流的录制参数和捕捉模式属性
void  WINAPI HxnDVR_GetMPEG4Property ( DWORD dwCardID, RecordMPEGPara * mpgPara, MPEG4Mode * dwMode );
// 录像函数 - 获取系统支持的可用视频压缩引擎及可用数量
BOOL  WINAPI HxnDVR_EnumVidCompressor ( VidCompressorInfo * pInfo, int * piVidCompNum );
// 录像函数 - 指定录像时使用的视频压缩引擎
BOOL  WINAPI HxnDVR_SetVidCompressor ( LPCTSTR szCompName );
// 录像函数 - 获取当前使用的视频压缩引擎
BOOL  WINAPI HxnDVR_GetCurrentVidCompressor ( LPSTR pszCompName );
// 录像函数 - 获取系统支持的可用音频压缩引擎及可用数量
BOOL  WINAPI HxnDVR_EnumAudCompressor ( AudCompressorInfo * pInfo, int * piAudCompNum );
// 录像函数 - 指定录像时使用的音频压缩引擎
BOOL  WINAPI HxnDVR_SetAudCompressor ( LPCTSTR szCompName );
// 录像函数 - 获取当前使用的音频压缩引擎
BOOL  WINAPI HxnDVR_GetCurrentAudCompressor ( LPSTR pszCompName );
// 录像函数 - 设置视频压缩的质量范围 (0 - 100)
BOOL  WINAPI HxnDVR_SetVidCompressorQuality ( int iQuality );
// 录像函数 - 获取视频压缩的质量
int   WINAPI HxnDVR_GetVidCompressorQuality ( );
// 录像函数 - 设置成快照大小一半尺寸进行录像 (对 640 X 480 尺寸有效)
BOOL  WINAPI HxnDVR_SetHalfSizeForVideo ( BOOL ishalf );

// 抓图函数 - 抓拍 BMP 图片
BOOL  WINAPI HxnDVR_SaveToBmpFile ( DWORD dwCardID, char * filename );
// 抓图函数 - 抓拍 JPG 图片
BOOL  WINAPI HxnDVR_SaveToJpgFile ( DWORD dwCardID, char * filename, DWORD dwQuality );
// 抓图函数 - 获取图像 DIB 点阵
BOOL  WINAPI HxnDVR_GetCurrentDib ( DWORD dwCardID, BYTE * pDestBuf, RECT * prc );
// 抓图函数 - 将图像存入剪贴板
BOOL  WINAPI HxnDVR_CopyToClipboard ( DWORD dwCardID );

// 回调函数 - 设置未压缩 RGB24 格式视频流回调
//获取DIB数据流
extern "C" {
typedef void (__stdcall *ForRGB24VideoCallBack) (DWORD dwCardID, const BYTE* pDIBHead, const BYTE* pDIBits, LPVOID pParam);
}
void  WINAPI HxnDVR_SetCallBackForRGB24VideoBuffer(int dwCardID, ForRGB24VideoCallBack proc, LPVOID pParam);
// 回调函数 - 设置未压缩 YUY2 格式视频流回调
typedef void (CALLBACK *ForVideoCallBack) ( DWORD dwCardID, BYTE * pbuff, DWORD dwSize );
void  WINAPI HxnDVR_SetCallBackForVideoBuffer ( DWORD dwCardID, ForVideoCallBack ppCall );
// 回调函数 - 设置MPEG4 压缩视频流回调
typedef void (CALLBACK *ForMpegCallBack) ( DWORD dwCardID, BYTE * pbuff, DWORD dwSize, BOOL isKeyFrm );
void  WINAPI HxnDVR_SetCallBackForMPEG4VideoBuffer ( DWORD dwCardID, ForMpegCallBack ppCall );
// 回调函数 - 设置音频流回调
typedef void (CALLBACK *ForAudioCallBack) ( DWORD dwCardID, BYTE*pbuf, DWORD dwSize );
void  WINAPI HxnDVR_SetCallBackForAudioBuffer ( DWORD dwCardID, ForAudioCallBack pAuCall );

//===========================
// 动态监测函数
//===========================
#define MOTION_DETECT_RECT_NUM_WIDTH    (16)
#define MOTION_DETECT_RECT_NUM_HEIGHT   (12)

// 动态监测回调函数
typedef void (WINAPI * MotionDetectCallback) ( DWORD dwCardID, BYTE * pucIsChange, PVOID pContext );
// 定义动态监测返回消息 wParam = 卡号，lParam = 变化值
#define VM_SENSOR_OUTPUT	(WM_USER + 0x2300)

typedef struct tagMOTIONPARAM
{
	DWORD dwAlarmMode;							// 0:Callback, 1:Message
	MotionDetectCallback pAlarmCallbackFunc;	// 设置回调函数
	PVOID pAlarmCallbackContext;				// 由用户定义的回调时识别数据结构指针
	HWND hAlarmWnd;								// 报警时接受消息窗口句柄
	DWORD dwAlarmMessageID;						// 自定义报警消息常量，WPARAM: card id, LPARAM: pucIsChange[][]

	BYTE ucThreshold_Y;							// 亮度阈值 (0 ~ 255)
	BYTE ucThreshold_UV;						// 色度阈值 (0 ~ 255)

	// 0:该区域不检测, 1:灵敏度最高, 12:灵敏度最低, >12:不检测
	BYTE ucPrecision[MOTION_DETECT_RECT_NUM_HEIGHT][MOTION_DETECT_RECT_NUM_WIDTH];
} MOTIONPARAM;

// 动态监测函数 - 启动动态监测
BOOL WINAPI HxnDVR_StartMotionDetect ( DWORD dwCardID );
// 动态监测函数 - 停止动态监测
void WINAPI HxnDVR_StopMotionDetect ( DWORD dwCardID );
// 动态监测函数 - 设定动态监测参数
BOOL WINAPI HxnDVR_SetMotionDetectParam ( DWORD dwCardID, MOTIONPARAM MotionParam );
// 动态监测函数 - 获取当前动态监测参数
void WINAPI HxnDVR_GetMotionDetectParam ( DWORD dwCardID, MOTIONPARAM * pParam );
// 动态监测函数 - 获取当前是否启动动态监测
BOOL WINAPI HxnDVR_IsStartMotionDetect ( DWORD dwCardID);
// 动态监测函数 - 是否显示当前监测参数网格
void WINAPI HxnDVR_DisplayMotionGrid ( DWORD dwCardID, BOOL isShow );
// 动态监测函数 - 是否显示当前监测变化网格
BOOL WINAPI HxnDVR_DisplayCheckMotionGrid ( DWORD dwCardID, BYTE * lparam );
// 动态监测函数 - 设置当前动态监测参数(按坐标点设定)
BOOL WINAPI HxnDVR_SetMotionDetectArea ( DWORD dwCardID, POINT point, BOOL isDetect, BYTE ucPrecision = 3 );

// 串口通讯函数 -通讯事件消息定义及参数说明
#define WM_COMM_BREAK_DETECTED		WM_USER+1	// EV_BREAK		A break was detected on input.
// 其中 (WPARAM) 0, (LPARAM) portId
#define WM_COMM_CTS_DETECTED		WM_USER+2	// EV_CTS		The CTS (clear-to-send) signal changed state. 
// 其中 (WPARAM) 0, (LPARAM) portId
#define WM_COMM_DSR_DETECTED		WM_USER+3	// EV_DSR		The DSR (data-set-ready) signal changed state. 
#define WM_COMM_ERR_DETECTED		WM_USER+4	// EV_ERR		A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY. 
// 其中 (WPARAM) 0, (LPARAM) portId
#define WM_COMM_RING_DETECTED		WM_USER+5	// EV_RING		A ring indicator was detected. 
// 其中 (WPARAM) 0, (LPARAM) portId
#define WM_COMM_RLSD_DETECTED		WM_USER+6	// EV_RLSD		The RLSD (receive-line-signal-detect) signal changed state. 
#define WM_COMM_RXCHAR				WM_USER+7	// EV_RXCHAR	A character was received and placed in the input buffer. 
// 其中 (WPARAM) RXBuff, (LPARAM) portId
#define WM_COMM_RXFLAG_DETECTED		WM_USER+8	// EV_RXFLAG	The event character was received and placed in the input buffer.  
// 其中 (WPARAM) 0, (LPARAM) portId
#define WM_COMM_TXEMPTY_DETECTED	WM_USER+9	// EV_TXEMPTY	The last character in the output buffer was sent.  

// 通讯事件设置 < 具体请参见 WinBase.h 中定义 >
// 将需要的消息事件进行或运算如：DWORD dwCommEvents = EV_RXCHAR | EV_CTS | ... 等

// 串口通讯函数 - 初始化并配置串口
BOOL  WINAPI HxnDVR_COMPortInit ( HWND pPortOwner,							// 设置接收消息传窗口
								  UINT  portId		  = 1,					// 串口号 (1..4)
								  UINT  baud		  = 9600,				// 波特率 baudrate
								  char  parity		  = 'N',				// 奇偶位 parity 
								  UINT  databits	  = 8,					// 数据位 databits 
								  UINT  stopbits	  = 1,					// 停止位 stopbits 
								  DWORD dwCommEvents  = EV_RXCHAR | EV_CTS,	// 通讯事件设置 EV_RXCHAR, EV_CTS 等
								  UINT  writebuffersize = 512);				// 通讯缓冲大小 size to the writebuffer
//  串口通讯函数 - 获取当前串口DCB配置信息  
DCB	  WINAPI HxnDVR_COMPortGetDCB ( UINT portId );
//  串口通讯函数 - 获取当前通讯事件设置
DWORD WINAPI HxnDVR_COMPortGetCommEvents ( UINT portId );
//  串口通讯函数 - 获取当前通讯缓冲大小 size to the writebuffer
DWORD WINAPI HxnDVR_COMPortGetWriteBufferSize ( UINT portId );
//  串口通讯函数 - 开始通讯
BOOL  WINAPI HxnDVR_COMPortStart ( UINT portId );
//  串口通讯函数 - 停止通讯
BOOL  WINAPI HxnDVR_COMPortStop ( UINT portId );
//  串口通讯函数 - 发送指定的通讯字节和字节数
void  WINAPI HxnDVR_COMPortSend ( UINT portId, BYTE * SendString, int iNum);
//  串口通讯函数 - 卸载指定串口号的串口通讯资源
void  WINAPI HxnDVR_COMPortUnInit ( UINT portId );

//  音频专用函数 - 选择音频输出到声卡的通道号
BOOL WINAPI HxnDVR_SetAudioToSoundCard ( DWORD dwCardID );
//  音频专用函数 - 允许音频通过声卡输出
void WINAPI HxnDVR_EnableAudioToSoundCard ( BOOL m_isAudioOut );

//  报警输入输出 - 获取输入报警口信号 
BOOL WINAPI HxnDVR_GetIO ( DWORD dwCardID, BOOL * isHigh );
//  报警输入输出 - 设置输出报警口信号 
void WINAPI HxnDVR_SetIO ( DWORD dwCardID, BOOL m_isHigh );

#endif