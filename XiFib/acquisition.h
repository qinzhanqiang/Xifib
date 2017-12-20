#include "..\include\xiApi.h"
#include "XiPlayPane.h"
#include "MainFrm.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"



#define	COLORS	256

typedef struct _myBITMAPINFO
{
	LONG	biSize;
	LONG	biWidth;
	LONG	biHeight;
	WORD	biPlanes;
	WORD	biBitCount;
	LONG	biCompression;
	LONG	biSizeImage;
	LONG	biXPelsPerMeter;
	LONG	biYPelsPerMeter;
	LONG	biClrUsed;
	LONG	biClrImportant;
	WORD	index[COLORS];
} myBITMAPINFO;

typedef struct _dsList {
	int     dsVal;
	CString dsStr; 
} dsList, *pdsList;

typedef struct _SETUP {
	UINT		display_refresh;
	//曝光时间
	UINT		uiExposure;	
	//降低采样速率
	UINT		uiDS;
	//增益
	float		fGain;
	float		fGammY;
	float		fGammC;
	float		fSharp;
	float		fkR;
	float		fkG;
	float		fkB;
	//放大倍数
	DWORD		zoom;
	//是否自动曝光
	BOOL		bAEAG;
    //自动带宽
	BOOL		bAWB;
	BOOL		bHDR;
	BOOL		bHDR_simplified;
	//坏点检测
	BOOL        bpl;
	//冷却、降温
	BOOL        bCooling;
	float       fTargetTemp;
	//图像格式
	XI_IMG_FORMAT frm;
	//是否显示图片
	BOOL		bDrawImage;	
	//目标图像区域选择
	DWORD		dwROIx0;
	DWORD		dwROIy0;
	DWORD		dwROIcx;
	DWORD		dwROIcy;
	//饱和度
	float       ccMtx[4][4];

	//实时数据与设置数据之间的边界
	UINT		dummy;			// border between setup and dynamic data ，dummy 虚拟的，
	//管理增益
	int         gainPos;
	//总帧数
	DWORD		dwNframes;
	//帧速率
	DWORD		dwPframes;
	//显示的帧数
	DWORD		dwNframes_disp;
	DWORD		dwTime;
	//所设置的图片格式的最大宽度
	DWORD		dwMaxCx;        // maximum image width for given image format
	//所设置的图片格式的最大高度
	DWORD		dwMaxCy;        // maximum image height for given image format
	int		x0;
	int		y0;
	int		cx;
	int		cy;
	//保存鼠标所在位置在图像上的实际坐标
	DWORD		px;
	DWORD		py;
	DWORD       mouseX;
	DWORD       mouseY;
	//鼠标所在位置的图像的像素值
	DWORD		pv;
	DWORD		pvR;
	DWORD		pvG;
	DWORD		pvB;
	//窗口大小
	CRect		wndsize;
	//每秒帧数
	double		fFPS;			// Frames per second 
	//每秒平均帧数
	double		fFPS_sum;		// Frames per second average
	//每帧的时间
	double		fTPF;			// Time per frame
	//每帧显示的时间
	double		fTPF_disp;
    //显示的总帧数
	double		fDisp_sum;
	DWORD		pcn_acq,pcn0_acq;
	DWORD		pcn_disp,pcn0_disp;
	XI_GPI_MODE myGpis[4];
	XI_GPO_MODE myGpos[4];
	//设备名称
	char        devName[256];
	//设备序列号
	char        devSerialNumber[256];
	int         numDsModes;
	//保存图像
	XI_IMG		img;
	//像素深度
	int			bpp;	//bytes per pixel

	XI_IMG		imgs;
	//获得的总帧数
	int			nGetFrame;
	//视频线程是否运行
	BOOL		bRun;
	//视频线程句柄
	HANDLE		hShadingThread;
	DWORD		dwHWN2;
	//BOOL		bWasRunning;
	//设备序列号
	DWORD		dwSerial;
	//是否有数据
	BOOL		bHaveData;
	//数据锁存
	BOOL		bDataLocked;
    //图像显示窗口
	XiPlayPane *childv;
	//调色句柄
	HPALETTE	hPalGray;
	//设备类型
	char        devType[MAX_PATH];
	int			hdr_t1;
	int			hdr_t2;
	int			hdr_kp1;
	int			hdr_kp2;
	int			hdr_simple_kp1;
	int			hdr_simple_comp;
	int			kp_count;
	int			max_kp_count;
	//用户帧速率
	float       fUserFrameRateFPS;

	DWORD devIndex;
	//鼠标左键按下时的坐标
	CPoint		cLBDpoint;		// Point where LeftButtonDown
	//鼠标拖拽的终点坐标
	CPoint		cDragPoint;		// Current Drag point

	CMainFrame * ShowWin;
	
	
} SETUP, *LPSETUP;

#define	SETUP_SIZE	offsetof(SETUP,dummy)

VOID shStopLive( HANDLE hMV );
BOOL shStartLive( HANDLE hMV, XiPlayPane *childv );
BOOL shIsLive( LPSETUP lps  );

void	pntInitPalette( void );
void    DrawImage(CDC * dc, XiPlayPane *childv);
void	setT0_acq(void);
double	getT_acq(void);

enum {
		ZOOM_25 = 0,
		ZOOM_50,
		ZOOM_100,
		ZOOM_200,
		ZOOM_STR
	};

bool RequestSaveImage(char* filename);
void shSlowDownAcquisition(int image_sleep_ms);
void Acquisition_Init();
int findHalf(cv::Mat& in, const int & maxval, std::vector<int> &Xp, std::vector<int> &Yp);
int linePoint(cv::Mat &src, cv::Point p, double ang, std::vector<double> &vx, std::vector<double> &value);
int linePointL(cv::Mat &src, cv::Point p, double ang, std::vector<double> &vx, std::vector<double> &value);
double round(double r);

//光纤型号，true为圆光纤，false为楔形光纤
//bool FiberModelFlag;


//extern double Z_distance = 0;
//cv::Point2d Center_0(1024,1024);



//void onSendMessage(int SData[6]);
//long OnReceiveEvent(WPARAM wParam, LPARAM lParam);






