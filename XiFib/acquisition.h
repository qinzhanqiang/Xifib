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
	//�ع�ʱ��
	UINT		uiExposure;	
	//���Ͳ�������
	UINT		uiDS;
	//����
	float		fGain;
	float		fGammY;
	float		fGammC;
	float		fSharp;
	float		fkR;
	float		fkG;
	float		fkB;
	//�Ŵ���
	DWORD		zoom;
	//�Ƿ��Զ��ع�
	BOOL		bAEAG;
    //�Զ�����
	BOOL		bAWB;
	BOOL		bHDR;
	BOOL		bHDR_simplified;
	//������
	BOOL        bpl;
	//��ȴ������
	BOOL        bCooling;
	float       fTargetTemp;
	//ͼ���ʽ
	XI_IMG_FORMAT frm;
	//�Ƿ���ʾͼƬ
	BOOL		bDrawImage;	
	//Ŀ��ͼ������ѡ��
	DWORD		dwROIx0;
	DWORD		dwROIy0;
	DWORD		dwROIcx;
	DWORD		dwROIcy;
	//���Ͷ�
	float       ccMtx[4][4];

	//ʵʱ��������������֮��ı߽�
	UINT		dummy;			// border between setup and dynamic data ��dummy ����ģ�
	//��������
	int         gainPos;
	//��֡��
	DWORD		dwNframes;
	//֡����
	DWORD		dwPframes;
	//��ʾ��֡��
	DWORD		dwNframes_disp;
	DWORD		dwTime;
	//�����õ�ͼƬ��ʽ�������
	DWORD		dwMaxCx;        // maximum image width for given image format
	//�����õ�ͼƬ��ʽ�����߶�
	DWORD		dwMaxCy;        // maximum image height for given image format
	int		x0;
	int		y0;
	int		cx;
	int		cy;
	//�����������λ����ͼ���ϵ�ʵ������
	DWORD		px;
	DWORD		py;
	DWORD       mouseX;
	DWORD       mouseY;
	//�������λ�õ�ͼ�������ֵ
	DWORD		pv;
	DWORD		pvR;
	DWORD		pvG;
	DWORD		pvB;
	//���ڴ�С
	CRect		wndsize;
	//ÿ��֡��
	double		fFPS;			// Frames per second 
	//ÿ��ƽ��֡��
	double		fFPS_sum;		// Frames per second average
	//ÿ֡��ʱ��
	double		fTPF;			// Time per frame
	//ÿ֡��ʾ��ʱ��
	double		fTPF_disp;
    //��ʾ����֡��
	double		fDisp_sum;
	DWORD		pcn_acq,pcn0_acq;
	DWORD		pcn_disp,pcn0_disp;
	XI_GPI_MODE myGpis[4];
	XI_GPO_MODE myGpos[4];
	//�豸����
	char        devName[256];
	//�豸���к�
	char        devSerialNumber[256];
	int         numDsModes;
	//����ͼ��
	XI_IMG		img;
	//�������
	int			bpp;	//bytes per pixel

	XI_IMG		imgs;
	//��õ���֡��
	int			nGetFrame;
	//��Ƶ�߳��Ƿ�����
	BOOL		bRun;
	//��Ƶ�߳̾��
	HANDLE		hShadingThread;
	DWORD		dwHWN2;
	//BOOL		bWasRunning;
	//�豸���к�
	DWORD		dwSerial;
	//�Ƿ�������
	BOOL		bHaveData;
	//��������
	BOOL		bDataLocked;
    //ͼ����ʾ����
	XiPlayPane *childv;
	//��ɫ���
	HPALETTE	hPalGray;
	//�豸����
	char        devType[MAX_PATH];
	int			hdr_t1;
	int			hdr_t2;
	int			hdr_kp1;
	int			hdr_kp2;
	int			hdr_simple_kp1;
	int			hdr_simple_comp;
	int			kp_count;
	int			max_kp_count;
	//�û�֡����
	float       fUserFrameRateFPS;

	DWORD devIndex;
	//����������ʱ������
	CPoint		cLBDpoint;		// Point where LeftButtonDown
	//�����ק���յ�����
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

//�����ͺţ�trueΪԲ���ˣ�falseΪШ�ι���
//bool FiberModelFlag;


//extern double Z_distance = 0;
//cv::Point2d Center_0(1024,1024);



//void onSendMessage(int SData[6]);
//long OnReceiveEvent(WPARAM wParam, LPARAM lParam);






