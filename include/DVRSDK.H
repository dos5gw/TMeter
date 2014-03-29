/*************************************************
**
** �ļ�����DVRSDK.h
**
** �����̣�Microvision Copyright
**
** ����ʱ�䣺2003.05.01
**
** �����£�2006.01.08
**
** ˵����DVRSDK.dll API �����������ļ�, ver 3.6
**
**************************************************/

#ifndef ___DVR_SDK_H__
#define ___DVR_SDK_H__

// ���� DVRSDK ��·͸��ɫ
#define	TRANSPARENCE_COLOR	RGB(238,0,238)

// ��Ƶ����˿ں�ѡ��
typedef enum
{
	VInput_AV1				= 0x0,
	VInput_AV2				= 0x1,
	VInput_AV3				= 0x2,
	VInput_AV4				= 0x3
} VInput;

// ��Ƶ�����Ͳ���
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

// ��Ƶ��ʽ����
typedef enum
{
	VStandard_NTSC			= 0x1,
	VStandard_PAL			= 0x3,
	VStandard_SECAM			= 0x6
} VStandard;

// ¼����Ƶ��ʽ
typedef enum
{
	VRecord_NTSC			= 0x0,
	VRecord_PAL				= 0x1,
} VRecStandard;

// ��Ƶ���Բ���
typedef enum
{
	VProperty_Brightness	= 0x00,             // ��Χ:	 0 ~ 510	��ѣ�255
	VProperty_Contrast		= 0x01,				// ��Χ:	 0 ~ 510	��ѣ�255
	VProperty_Hue			= 0x02,				// ��Χ:	 0 ~ 510	��ѣ�255
	VProperty_Saturation	= 0x03,				// ��Χ:	 0 ~ 510	��ѣ�300
} VProperty;

// ��������ʾ��������
typedef struct tagCardRect
{
	RECT	rcWndPos;							// ����λ��
	DWORD	dwCardID;							// ����
	DWORD	reserve1;							// ��չ����
	DWORD	reserve2;							// ��չ����
} CAP_WND_POS;

// MPEG4 ¼�Ʋ�������
typedef struct tagProperty
{
	DWORD RecProperty_BitRate;					// λ�� (��Χ��56KBPS ~ 10MBPS)
	DWORD RecProperty_KeyFrameInterval;			// �ؼ�֡��� (Ҫ�󣺴��ڵ���֡��)
	DWORD RecProperty_FrameRate;				// ֡�� (��Χ��1 ~ 25(PAL)/30(NTSC)֡)
	VRecStandard RecProperty_VStandard;			// ��Ƶ��ʽ (ȡֵ��0 ΪNTSC, 1 ΪPAL)
} RecordMPEGPara;

// ��׽ģʽ�������� HxnDVR_SetCaptureFile () �����ӿ�
// ���磺Ҫ��׽ֻ����Ƶ�� MPEG4 �ļ��Ͳ�׽����������Ϊ .WAV �ļ�����ʹ��
// ������CaptureMode_VO_MPEG4_FILE | CaptureMode_AO_WAV_FILE
#define	CaptureMode_VA_MPEG4_FILE	0xffffffff	// OCXȡֵΪ��1  ��׽����Ƶ���ϵ� MPEG4 �ļ�
#define	CaptureMode_VO_MPEG4_FILE	0xfff70000	// OCXȡֵΪ��2  ��׽ֻ����Ƶ�� MPEG4 �ļ�
#define	CaptureMode_VO_YUY2_FILE	0xfff00000	// OCXȡֵΪ��4  ��׽δѹ�� YUY2 �ļ�
#define	CaptureMode_VO_USER			0xff000000	// OCXȡֵΪ��8  ����׽������Ƶ���ṩ���û�
#define	CaptureMode_AO_WAV_USER		0x000000ff	// OCXȡֵΪ��16 ����׽����δѹ����Ƶ���ṩ���û�
#define	CaptureMode_AO_ADPCM_USER	0x00000fff	// OCXȡֵΪ��32 ����׽����ѹ����ƵADPCM���ṩ���û�
#define	CaptureMode_AO_WAV_FILE		0x0000fff7	// OCXȡֵΪ��64 ��׽����������Ϊ .WAV �ļ�

//  ���� MPEG4 ��׽ʱ����ģʽ
typedef enum
{
	MPEG4Mode_ToFile			= 0x00,			// ֻ�����ڲ�׽���ļ�
	MPEG4Mode_Both				= 0x01,			// ���Բ�׽���ļ�Ҳ������
	MPEG4Mode_ToTransmit		= 0x02			// ֻ����������
} MPEG4Mode;

//  ��Ƶѹ���������Ϣ
//  ���ں�����HxnDVR_EnumVidCompressor ()
typedef struct VIDCOMPRESSORINFO
{
	char szName[128];
} VidCompressorInfo;

//  ��Ƶѹ���������Ϣ
//  ���ں�����HxnDVR_EnumAudCompressor()
typedef struct AUDCOMPRESSORINFO
{
	char szName[128];
} AudCompressorInfo;

//�����û���Ϣ
#define		WM_Microvision_WINMSG		WM_USER + 0x300

// ϵͳ���� - ������DVR�豸ȫ�ֶѽ��г�ʼ��
DWORD WINAPI HxnDVR_Init ( HWND hWnd );
// ϵͳ���� - ������DVR�豸ȫ�ֶ�ռ����Դ�����ͷ�
void  WINAPI HxnDVR_UnInit ( void );
// ϵͳ���� - ��ȡDVR�豸��ʵ��������
DWORD WINAPI HxnDVR_GetDeviceAmount ( void );
// ϵͳ���� - ����ָ�����ŵ�����Ƶ�豸, �ɶ�ε���ѡ����Ƶ����˿ں�
BOOL  WINAPI HxnDVR_ConnectDevice ( DWORD dwCardID, VInput m_Insel = VInput_AV1 );
// ϵͳ���� - �ر�ָ�����ŵ�����Ƶ�豸
void  WINAPI HxnDVR_DisconnectDevice ( DWORD dwCardID );
// ϵͳ���� - �жϵ�ǰ�豸�Ƿ��Ѿ�����
BOOL  WINAPI HxnDVR_IsConnected ( DWORD dwCardID );
// ϵͳ���� - ���ò�׽��Ƶ������
BOOL  WINAPI HxnDVR_SetVSubType ( DWORD dwCardID, VSubType subType );
// ϵͳ���� - ���õ�ǰ�豸Ҫ��ʾ��Ƶ��ʽ
BOOL  WINAPI HxnDVR_SetVStandard ( DWORD dwCardID, VStandard vformat );
// ϵͳ���� - ���ò�׽��Ƶ����(���ȡ��Աȶȡ�ɫ�ȡ����ͶȺ����(������)��)
BOOL  WINAPI HxnDVR_SetVPropertyValue ( DWORD dwCardID, VProperty pro, DWORD dwValue );
// ϵͳ���� - ��ȡ��׽��Ƶ�ߴ��С
void  WINAPI HxnDVR_GetVidCapSize ( DWORD dwCardID, DWORD * dwCapWidth, DWORD * dwCapHeight );
// ϵͳ���� - ���ò�׽��Ƶ�ߴ��С
BOOL  WINAPI HxnDVR_SetVidCapSize ( DWORD dwCardID, DWORD dwCapWidth, DWORD dwCapHeight );
// ϵͳ���� - ��ȡ��ʾ��������Ƶ�ߴ��С (SDK7000 ��Ч)
void  WINAPI HxnDVR_GetVidPreSize ( DWORD dwCardID, DWORD * dwCapWidth, DWORD * dwCapHeight );
// ϵͳ���� - ������ʾ��������Ƶ�ߴ��С (SDK7000 ��Ч)
BOOL  WINAPI HxnDVR_SetVidPreSize ( DWORD dwCardID, DWORD dwCapWidth, DWORD dwCapHeight );
// ϵͳ���� - ��ȡ��ǰʹ����Ƶѹ������ID��, �����ȵ��� HxnDVR_Init
BOOL  WINAPI HxnDVR_GetCurrentCardID ( BYTE RegID[32] );
// ϵͳ���� - ��YUY2����ת����RGB24����
BOOL  WINAPI HxnDVR_YUY2RGB24 ( void * dstBuf, void * srcBuf, DWORD dwWidth, DWORD dwHigh );
// ϵͳ���� - ��̨����һ��ָ��WAV�ļ�
BOOL  WINAPI HxnDVR_PlaySoundFromFile ( HWND m_hWnd,			 // ���Ŵ��ھ��
									   char *m_filename,		 // Ҫ���ŵ�WAV�ļ���
									   DWORD dwPrimaryChannels,  // ��Ƶͨ�����磺2
									   DWORD dwPrimaryFreq,		 // Ƶ���磺22050
									   DWORD dwPrimaryBitRate ); // λ���磺16
// ϵͳ���� - ��������
BOOL  WINAPI HxnDVR_EnableMirror ( DWORD dwCardID, BOOL m_isMirror );
// ϵͳ���� - ��������
BOOL  WINAPI HxnDVR_EnableInversion ( DWORD dwCardID, BOOL m_isInversion );

// ��ʾ���� - �Ƿ���ʾָ�����ŵ���Ƶͼ��
BOOL  WINAPI HxnDVR_ShowWindow ( DWORD dwCardID, BOOL isShow );
// ��ʾ���� - ��ֻ��Ҫ��ʾ��·�ߴ�߶ȴ���288��ʹ�ô˺���
BOOL  WINAPI HxnDVR_SetOneScreenAnomaly ( DWORD dwCardID, HWND hWnd, RECT *rc );
// ��ʾ���� - ��ֻ�谴����ʽ��ʾ��·ÿ·��Ƶ�ߴ�߶ȴ���288��ʹ�ô˺���
BOOL  WINAPI HxnDVR_SetFourScreenAnomaly ( DWORD dwCardID1, DWORD dwCardID2, DWORD dwCardID3, DWORD dwCardID4, HWND hWnd, RECT *rc );
// ��ʾ���� - �趨ָ�����ŵ���Ƶͼ��λ�ã����� HxnDVR_ShowWindow �����Ƿ���ʾ
BOOL  WINAPI HxnDVR_SetWindowPos ( DWORD dwCardID, HWND hWnd, RECT *rc );
// ��ʾ���� - ȫ����ʾ��ָ�������ʾָ�����ŵ���Ƶ�豸
BOOL  WINAPI HxnDVR_SetFullScreen ( DWORD dwCardID, BOOL isFull );
// ��ʾ���� - ���ò����򴰿ں���
BOOL  WINAPI HxnDVR_SetScreenAnomaly ( RECT			* rcLogic,
									   RECT			* rcWndPos,
									   CAP_WND_POS	* pCardPos,
									   DWORD		dwCardIDCount );
// ��ʾ���� - ����Logo����(�糡��˵����)
BOOL WINAPI HxnDVR_SetLogoText ( DWORD dwCardID, LPCTSTR szLogoText );
// ��ʾ���� - ��ʾ������Logo
void WINAPI HxnDVR_ShowLogo ( DWORD dwCardID, BOOL bShow, int x, int y );
// ��ʾ���� - ��ʾ������ʱ��
void WINAPI HxnDVR_ShowTime ( DWORD dwCardID, BOOL bShow, int x, int y );
// ��ʾ���� - ��ʾ����������
void WINAPI HxnDVR_ShowDate ( DWORD dwCardID, BOOL bShow, int x, int y );
// ��ʾ���� - ������ʾ��ǿģʽ, ѡ��Χ(0,1,...14)
BOOL WINAPI HxnDVR_ShowInfocus ( DWORD dwCardID, int iMode );
// ��ʾ���� - ͼ��������ʾģʽ, ѡ��Χ(0,1,...14)
BOOL WINAPI HxnDVR_ShowInBlaze ( DWORD dwCardID, int iMode );
// ��ʾ���� - ��ʾ����ʾ��Ƶ�߿�����
void WINAPI HxnDVR_DisplaySideForVideoScreen ( DWORD dwCardID, DWORD Color, BOOL isShow );
// ��ʾ���� - ������Ƶ��ʾ֡��
BOOL WINAPI HxnDVR_SetDisplayFrame ( DWORD dwCardID, int iFrame );
// ��ʾ���� - �жϵ�ǰͨ���Ƿ�����Ƶͼ������
BOOL WINAPI HxnDVR_IsVideoSignalLocked ( DWORD dwCardID );

// ¼���� - ����Ҫ¼�Ƶ��ļ����ƺͲ�׽ģʽ, ���� dwMode �ο���׽ģʽ����
BOOL  WINAPI HxnDVR_SetCaptureFile ( DWORD dwCardID, char *filename, DWORD dwMode );
// ¼���� - ��ʼ��׽�����ļ�
BOOL  WINAPI HxnDVR_StartCapture ( DWORD dwCardID );
// ¼���� - ֹͣ��׽�����ļ�
void  WINAPI HxnDVR_StopCapture ( DWORD dwCardID );
// ¼���� - �ж�ָ�����Ƿ����ڽ��в�׽
BOOL  WINAPI HxnDVR_IsVideoCapture	( DWORD dwCardID );
// ¼���� - ���� MPEG4 ѹ������¼�Ʋ����Ͳ�׽ģʽ����
void  WINAPI HxnDVR_SetMPEG4Property ( DWORD dwCardID, RecordMPEGPara * mpgPara, MPEG4Mode dwMode);
// ¼���� - ��ȡ MPEG4 ѹ������¼�Ʋ����Ͳ�׽ģʽ����
void  WINAPI HxnDVR_GetMPEG4Property ( DWORD dwCardID, RecordMPEGPara * mpgPara, MPEG4Mode * dwMode );
// ¼���� - ��ȡϵͳ֧�ֵĿ�����Ƶѹ�����漰��������
BOOL  WINAPI HxnDVR_EnumVidCompressor ( VidCompressorInfo * pInfo, int * piVidCompNum );
// ¼���� - ָ��¼��ʱʹ�õ���Ƶѹ������
BOOL  WINAPI HxnDVR_SetVidCompressor ( LPCTSTR szCompName );
// ¼���� - ��ȡ��ǰʹ�õ���Ƶѹ������
BOOL  WINAPI HxnDVR_GetCurrentVidCompressor ( LPSTR pszCompName );
// ¼���� - ��ȡϵͳ֧�ֵĿ�����Ƶѹ�����漰��������
BOOL  WINAPI HxnDVR_EnumAudCompressor ( AudCompressorInfo * pInfo, int * piAudCompNum );
// ¼���� - ָ��¼��ʱʹ�õ���Ƶѹ������
BOOL  WINAPI HxnDVR_SetAudCompressor ( LPCTSTR szCompName );
// ¼���� - ��ȡ��ǰʹ�õ���Ƶѹ������
BOOL  WINAPI HxnDVR_GetCurrentAudCompressor ( LPSTR pszCompName );
// ¼���� - ������Ƶѹ����������Χ (0 - 100)
BOOL  WINAPI HxnDVR_SetVidCompressorQuality ( int iQuality );
// ¼���� - ��ȡ��Ƶѹ��������
int   WINAPI HxnDVR_GetVidCompressorQuality ( );
// ¼���� - ���óɿ��մ�Сһ��ߴ����¼�� (�� 640 X 480 �ߴ���Ч)
BOOL  WINAPI HxnDVR_SetHalfSizeForVideo ( BOOL ishalf );

// ץͼ���� - ץ�� BMP ͼƬ
BOOL  WINAPI HxnDVR_SaveToBmpFile ( DWORD dwCardID, char * filename );
// ץͼ���� - ץ�� JPG ͼƬ
BOOL  WINAPI HxnDVR_SaveToJpgFile ( DWORD dwCardID, char * filename, DWORD dwQuality );
// ץͼ���� - ��ȡͼ�� DIB ����
BOOL  WINAPI HxnDVR_GetCurrentDib ( DWORD dwCardID, BYTE * pDestBuf, RECT * prc );
// ץͼ���� - ��ͼ����������
BOOL  WINAPI HxnDVR_CopyToClipboard ( DWORD dwCardID );

// �ص����� - ����δѹ�� RGB24 ��ʽ��Ƶ���ص�
//��ȡDIB������
extern "C" {
typedef void (__stdcall *ForRGB24VideoCallBack) (DWORD dwCardID, const BYTE* pDIBHead, const BYTE* pDIBits, LPVOID pParam);
}
void  WINAPI HxnDVR_SetCallBackForRGB24VideoBuffer(int dwCardID, ForRGB24VideoCallBack proc, LPVOID pParam);
// �ص����� - ����δѹ�� YUY2 ��ʽ��Ƶ���ص�
typedef void (CALLBACK *ForVideoCallBack) ( DWORD dwCardID, BYTE * pbuff, DWORD dwSize );
void  WINAPI HxnDVR_SetCallBackForVideoBuffer ( DWORD dwCardID, ForVideoCallBack ppCall );
// �ص����� - ����MPEG4 ѹ����Ƶ���ص�
typedef void (CALLBACK *ForMpegCallBack) ( DWORD dwCardID, BYTE * pbuff, DWORD dwSize, BOOL isKeyFrm );
void  WINAPI HxnDVR_SetCallBackForMPEG4VideoBuffer ( DWORD dwCardID, ForMpegCallBack ppCall );
// �ص����� - ������Ƶ���ص�
typedef void (CALLBACK *ForAudioCallBack) ( DWORD dwCardID, BYTE*pbuf, DWORD dwSize );
void  WINAPI HxnDVR_SetCallBackForAudioBuffer ( DWORD dwCardID, ForAudioCallBack pAuCall );

//===========================
// ��̬��⺯��
//===========================
#define MOTION_DETECT_RECT_NUM_WIDTH    (16)
#define MOTION_DETECT_RECT_NUM_HEIGHT   (12)

// ��̬���ص�����
typedef void (WINAPI * MotionDetectCallback) ( DWORD dwCardID, BYTE * pucIsChange, PVOID pContext );
// ���嶯̬��ⷵ����Ϣ wParam = ���ţ�lParam = �仯ֵ
#define VM_SENSOR_OUTPUT	(WM_USER + 0x2300)

typedef struct tagMOTIONPARAM
{
	DWORD dwAlarmMode;							// 0:Callback, 1:Message
	MotionDetectCallback pAlarmCallbackFunc;	// ���ûص�����
	PVOID pAlarmCallbackContext;				// ���û�����Ļص�ʱʶ�����ݽṹָ��
	HWND hAlarmWnd;								// ����ʱ������Ϣ���ھ��
	DWORD dwAlarmMessageID;						// �Զ��屨����Ϣ������WPARAM: card id, LPARAM: pucIsChange[][]

	BYTE ucThreshold_Y;							// ������ֵ (0 ~ 255)
	BYTE ucThreshold_UV;						// ɫ����ֵ (0 ~ 255)

	// 0:�����򲻼��, 1:���������, 12:���������, >12:�����
	BYTE ucPrecision[MOTION_DETECT_RECT_NUM_HEIGHT][MOTION_DETECT_RECT_NUM_WIDTH];
} MOTIONPARAM;

// ��̬��⺯�� - ������̬���
BOOL WINAPI HxnDVR_StartMotionDetect ( DWORD dwCardID );
// ��̬��⺯�� - ֹͣ��̬���
void WINAPI HxnDVR_StopMotionDetect ( DWORD dwCardID );
// ��̬��⺯�� - �趨��̬������
BOOL WINAPI HxnDVR_SetMotionDetectParam ( DWORD dwCardID, MOTIONPARAM MotionParam );
// ��̬��⺯�� - ��ȡ��ǰ��̬������
void WINAPI HxnDVR_GetMotionDetectParam ( DWORD dwCardID, MOTIONPARAM * pParam );
// ��̬��⺯�� - ��ȡ��ǰ�Ƿ�������̬���
BOOL WINAPI HxnDVR_IsStartMotionDetect ( DWORD dwCardID);
// ��̬��⺯�� - �Ƿ���ʾ��ǰ����������
void WINAPI HxnDVR_DisplayMotionGrid ( DWORD dwCardID, BOOL isShow );
// ��̬��⺯�� - �Ƿ���ʾ��ǰ���仯����
BOOL WINAPI HxnDVR_DisplayCheckMotionGrid ( DWORD dwCardID, BYTE * lparam );
// ��̬��⺯�� - ���õ�ǰ��̬������(��������趨)
BOOL WINAPI HxnDVR_SetMotionDetectArea ( DWORD dwCardID, POINT point, BOOL isDetect, BYTE ucPrecision = 3 );

// ����ͨѶ���� -ͨѶ�¼���Ϣ���弰����˵��
#define WM_COMM_BREAK_DETECTED		WM_USER+1	// EV_BREAK		A break was detected on input.
// ���� (WPARAM) 0, (LPARAM) portId
#define WM_COMM_CTS_DETECTED		WM_USER+2	// EV_CTS		The CTS (clear-to-send) signal changed state. 
// ���� (WPARAM) 0, (LPARAM) portId
#define WM_COMM_DSR_DETECTED		WM_USER+3	// EV_DSR		The DSR (data-set-ready) signal changed state. 
#define WM_COMM_ERR_DETECTED		WM_USER+4	// EV_ERR		A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY. 
// ���� (WPARAM) 0, (LPARAM) portId
#define WM_COMM_RING_DETECTED		WM_USER+5	// EV_RING		A ring indicator was detected. 
// ���� (WPARAM) 0, (LPARAM) portId
#define WM_COMM_RLSD_DETECTED		WM_USER+6	// EV_RLSD		The RLSD (receive-line-signal-detect) signal changed state. 
#define WM_COMM_RXCHAR				WM_USER+7	// EV_RXCHAR	A character was received and placed in the input buffer. 
// ���� (WPARAM) RXBuff, (LPARAM) portId
#define WM_COMM_RXFLAG_DETECTED		WM_USER+8	// EV_RXFLAG	The event character was received and placed in the input buffer.  
// ���� (WPARAM) 0, (LPARAM) portId
#define WM_COMM_TXEMPTY_DETECTED	WM_USER+9	// EV_TXEMPTY	The last character in the output buffer was sent.  

// ͨѶ�¼����� < ������μ� WinBase.h �ж��� >
// ����Ҫ����Ϣ�¼����л������磺DWORD dwCommEvents = EV_RXCHAR | EV_CTS | ... ��

// ����ͨѶ���� - ��ʼ�������ô���
BOOL  WINAPI HxnDVR_COMPortInit ( HWND pPortOwner,							// ���ý�����Ϣ������
								  UINT  portId		  = 1,					// ���ں� (1..4)
								  UINT  baud		  = 9600,				// ������ baudrate
								  char  parity		  = 'N',				// ��żλ parity 
								  UINT  databits	  = 8,					// ����λ databits 
								  UINT  stopbits	  = 1,					// ֹͣλ stopbits 
								  DWORD dwCommEvents  = EV_RXCHAR | EV_CTS,	// ͨѶ�¼����� EV_RXCHAR, EV_CTS ��
								  UINT  writebuffersize = 512);				// ͨѶ�����С size to the writebuffer
//  ����ͨѶ���� - ��ȡ��ǰ����DCB������Ϣ  
DCB	  WINAPI HxnDVR_COMPortGetDCB ( UINT portId );
//  ����ͨѶ���� - ��ȡ��ǰͨѶ�¼�����
DWORD WINAPI HxnDVR_COMPortGetCommEvents ( UINT portId );
//  ����ͨѶ���� - ��ȡ��ǰͨѶ�����С size to the writebuffer
DWORD WINAPI HxnDVR_COMPortGetWriteBufferSize ( UINT portId );
//  ����ͨѶ���� - ��ʼͨѶ
BOOL  WINAPI HxnDVR_COMPortStart ( UINT portId );
//  ����ͨѶ���� - ֹͣͨѶ
BOOL  WINAPI HxnDVR_COMPortStop ( UINT portId );
//  ����ͨѶ���� - ����ָ����ͨѶ�ֽں��ֽ���
void  WINAPI HxnDVR_COMPortSend ( UINT portId, BYTE * SendString, int iNum);
//  ����ͨѶ���� - ж��ָ�����ںŵĴ���ͨѶ��Դ
void  WINAPI HxnDVR_COMPortUnInit ( UINT portId );

//  ��Ƶר�ú��� - ѡ����Ƶ�����������ͨ����
BOOL WINAPI HxnDVR_SetAudioToSoundCard ( DWORD dwCardID );
//  ��Ƶר�ú��� - ������Ƶͨ���������
void WINAPI HxnDVR_EnableAudioToSoundCard ( BOOL m_isAudioOut );

//  ����������� - ��ȡ���뱨�����ź� 
BOOL WINAPI HxnDVR_GetIO ( DWORD dwCardID, BOOL * isHigh );
//  ����������� - ��������������ź� 
void WINAPI HxnDVR_SetIO ( DWORD dwCardID, BOOL m_isHigh );

#endif