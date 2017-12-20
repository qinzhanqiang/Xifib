// ViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include <windowsx.h>
#include <mmsystem.h>
//#include "Viewer.h"
#include "XiFib.h"
#include "ViewerDlg.h"
#include "acquisition.h"
#include "psapi.h"
#include "..\include\versions.h"
#include "camSelect.h"

CString strWindowpos = "windowspos";
CString strSetups = "setup";
CString strCamType = "Virt";

#define APP_NAME "XIMEA Viewer " API_INST_PACKAGE_VERSION_STRING

#define REGISTRY_KEY "HKEY_CURRENT_USER\Software\XIMEA\XIMEA Camera Viewer"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	TIMER_ID	100
#define	TIMER_ID2	200
#define	TIMEOUT		1000	// 1sec.

// CViewerDlg dialog

extern LPSETUP lps;//保存设置的结构体指针，SETUP保存设置参数的结构体

//*****************************************************************************

CViewerDlg::CViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	hMV = INVALID_HANDLE_VALUE;
	sdlg = NULL;
	sdlgOpened = FALSE;	
}

//*****************************************************************************

void CViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//*****************************************************************************

BEGIN_MESSAGE_MAP(CViewerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_MOVE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


//*****************************************************************************

BOOL CViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	Acquisition_Init(); //设置初始化

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Read the default setup
	memset(lps, 0, sizeof(SETUP));
	devIndex = 0;

	for(int i = 0; i < NUMBER_ROWS; i++)
	{
		note[i] = NULL;
	}
	font_size = 10;
	char tmp[MAX_PATH];
	FILE * fp = fopen("note.txt", "rt");
	if(fp){
		fgets(tmp, MAX_PATH, fp);
		sscanf(tmp, "%d", &font_size);
		VERIFY(font.CreateFont(
		   font_size,                 // nHeight
		   0,                         // nWidth
		   0,                         // nEscapement
		   0,                         // nOrientation
		   FW_NORMAL,                 // nWeight
		   FALSE,                     // bItalic
		   FALSE,                     // bUnderline
		   0,                         // cStrikeOut
		   ANSI_CHARSET,              // nCharSet 
		   OUT_DEFAULT_PRECIS,        // nOutPrecision
		   CLIP_DEFAULT_PRECIS,       // nClipPrecision
		   DEFAULT_QUALITY,           // nQuality
		   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		   "Arial"));                 // lpszFacename
		int i = 0;
		for(i = 0; i < NUMBER_ROWS; i++)
		{
			note[i] = (char *)malloc(MAX_PATH);
			memset(note[i], 0, MAX_PATH);
			if(!fgets(note[i], MAX_PATH, fp)) break;
		}
		number_of_rows = i + 1;
		
		fclose(fp);
	}

	SetTimer( TIMER_ID2, TIMEOUT, NULL );//定时器函数，第一个参数为定时器ID，
	//第二个参数为定时时长，第三个参数为回调函数指针,第三个参数是一个回调函数，
	//在这个函数里，放入你想要做的事情的代码，
	//你可以将它设定为NULL，也就是使用系统默认的回调函数，系统默认认的是onTime函数。
	return TRUE;  // return TRUE  unless you set the focus to a control
}

//*****************************************************************************
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CViewerDlg::OnPaint()
{
	CPaintDC dcl(this); // device context for painting
	CDC * dc = &dcl;
	if(lps->bDrawImage){
		DrawImage(dc, this);
	}else lps->fDisp_sum = 0;
}

//*****************************************************************************
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//*****************************************************************************

BOOL CViewerDlg::DestroyWindow()
{
	KillTimer(TIMER_ID);
	lps->bHaveData = FALSE;
	if(sdlg != NULL){
		sdlg->ShowWindow(SW_HIDE);
		sdlg->DestroyWindow();
		delete sdlg;
		sdlg = NULL;
	}
	if(hMV != INVALID_HANDLE_VALUE){
		if(isRunWithSN)
			SavePresetsToReg(hMV);

		if (lps->bRun) 
			shStopLive( hMV );
		xiCloseDevice( hMV );
	}

	for(int i = 0; i < NUMBER_ROWS; i++)
	{
		if(note[i]) 
			free(note[i]);
	}

	return CDialog::DestroyWindow();
}

//*****************************************************************************

void CViewerDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	CRect wndsize;
	CRect swndsize;

	if(!sdlg) return;

	if(sdlgOpened == FALSE){
		if (hMV != INVALID_HANDLE_VALUE) {
			//hide advanced setup dialog
			sdlg->ShowWindow(SW_HIDE);
			sdlgOpened = FALSE;
			//show minimal setup dialog
			GetWindowRect( &wndsize );
			sdlg->RefreshValues();
			sdlg->GetWindowRect( &swndsize );
			sdlg->SetWindowPos(NULL, wndsize.right - swndsize.Width() - 8, wndsize.top + 30, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			sdlg->ShowWindow(SW_SHOW);
			sdlgOpened = TRUE;
			SetFocus();
		}
	}else{
		sdlg->ShowWindow(SW_HIDE);
		sdlgOpened = FALSE;
	}
	
	CDialog::OnRButtonDown(nFlags, point);
}

//*****************************************************************************

void CViewerDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	CRect wndsize;
	CRect swndsize;

	if(sdlgOpened)
	{
		GetWindowRect( &wndsize );
		sdlg->GetWindowRect( &swndsize );
		sdlg->SetWindowPos(NULL, wndsize.right - swndsize.Width() - 8, wndsize.top + 30, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
}

//*****************************************************************************

void CViewerDlg::OnTimer(UINT_PTR nIDEvent)
{	
	XI_RETURN ret = XI_OK;
	if(nIDEvent == TIMER_ID)
	{
		KillTimer(TIMER_ID);
		int isexist = FALSE;
		if (hMV != INVALID_HANDLE_VALUE) xiGetParamInt(hMV, XI_PRM_IS_DEVICE_EXIST, &isexist);
		if(!isexist){
			CloseCurrentDevice();
			return;
		}
		double	tau = 0.5;
		DWORD level = 0;
		lps->fTPF = getT_acq();
		setT0_acq();
		if (lps->dwNframes != lps->dwPframes && lps->dwNframes) {
			lps->fTPF /= (double)(lps->dwNframes - lps->dwPframes);
		}
		lps->dwPframes = lps->dwNframes;
		if(lps->fTPF) lps->fFPS = 1./lps->fTPF;
		if (lps->dwNframes) lps->fFPS_sum = lps->fFPS_sum*tau + lps->fFPS*(1.-tau);
		else		      lps->fFPS_sum = lps->fFPS;

		float ch_t = 0, h_t = 0;
		xiGetParamFloat(hMV, XI_PRM_CHIP_TEMP, &ch_t);
		xiGetParamFloat(hMV, XI_PRM_HOUS_TEMP, &h_t);

		sdlg->m_chip_temp.Format("%4.2fC", ch_t);
		sdlg->m_hous_temp.Format("%4.2fC", h_t);

		RefreshWindowText();
		lps->dwTime++;

		if(lps->bAEAG)
		{
			if(sdlgOpened) sdlg->UpdateControls();
		}
		SetTimer( TIMER_ID, TIMEOUT, NULL );
	}

	else if(nIDEvent == TIMER_ID2)
	{
		DWORD	devices;
		bool foundDevice = false;
		KillTimer(TIMER_ID2);

		SetWindowText("Searching for connected cameras. Please wait...");

		hMV = INVALID_HANDLE_VALUE;
		xiGetNumberDevices( &devices );
		if(!devices)
		{
			SetTimer( TIMER_ID2, TIMEOUT, NULL );
			return;
		}
		if(isRunWithSN)
		{
			for(unsigned int i=0; i < devices; i++)
			{
				DWORD dwSerial = 0;
				char serial[MAX_PATH]="";				
				xiGetDeviceInfoString(i, XI_PRM_DEVICE_SN, serial, sizeof(serial));
				if(!strcmp(serial, runSN))
				{
					devIndex = i;
					foundDevice = true;
				}
			}
			if(!foundDevice)
				CViewerDlg::EndDialog(-1);
		}
		else if( devices > 1)
		{
			camSelect camSel;
			camSel.GetIndex(&devIndex);
			camSel.DoModal();
		}
		
		if (!auto_bandwidth_measure_enabled || bandwidth_limit_enabled)
		{
			// disable auto bandwidth calculation
			xiSetParamInt(0,XI_PRM_AUTO_BANDWIDTH_CALCULATION, 0);
		}
		lps->devIndex = devIndex;
		if (devices) {
			ret = xiOpenDevice(devIndex, &hMV );   //如果发现设备，启动设备
			if (bandwidth_limit_enabled)
			{
				xiSetParamInt(hMV,XI_PRM_LIMIT_BANDWIDTH, bandwidth_limit_MBPS*8);
			}
			if (ret != XI_OK) 
			{
				SetTimer( TIMER_ID2, TIMEOUT, NULL );
				return;
			} 
			else 
			{
				// check privilegies  权限
				if (XI_NOT_ENOUGH_PRIVILEGES == xiSetParamInt(hMV, XI_PRM_EXPOSURE, 1000))
				{
					// error opening
					xiCloseDevice(hMV);
					hMV = INVALID_HANDLE_VALUE;
					Sleep(1000); // wait until other process return the privilegies
					printf("Camera doesn't have priv\n");
					SetTimer( TIMER_ID2, TIMEOUT, NULL );
				}
				else
				{
					// create setup dialog instance
					sdlg = new CSetup(this);
					sdlg->Create(IDD_SETUP, this);
					sdlgOpened = FALSE;
					// ini acquisition dialog
					pntInitPalette();
					lps->dwNframes = 0;
					lps->dwPframes = 0;
					lps->dwTime = 0;
					xiGetParamInt(hMV, XI_PRM_DEVICE_SN, (int *)&lps->dwSerial);
					xiGetParamString(hMV, XI_PRM_DEVICE_TYPE, lps->devType, MAX_PATH);
					
					if(isRunWithSN)LoadPresetsFromReg(hMV);
					shStartLive(hMV, this);	     //启动摄像头
					SetTimer( TIMER_ID, TIMEOUT, NULL );

					// Updates main window sze and position					
					if(!sdlg->isProfileLoaded)
					{
						// show application in fullscreen mode
						WINDOWPLACEMENT wpp;  //结构中包含了有关窗口在屏幕上位置的信息
						const long nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
						const long nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
						
						memset(&wpp, 0, sizeof(WINDOWPLACEMENT));  
						//作用是在一段内存块中填充某个给定的值，它是对较大的结构体或数组进行清零操作的一种最快方法
						wpp.length = sizeof(WINDOWPLACEMENT);
						wpp.flags = WPF_RESTORETOMAXIMIZED;
						wpp.showCmd = SW_SHOWMAXIMIZED;
						wpp.rcNormalPosition.left = 0;
						wpp.rcNormalPosition.top = 0;
						wpp.rcNormalPosition.right = nScreenWidth/2;
						wpp.rcNormalPosition.bottom = nScreenHeight/2;
						SetWindowPlacement( &wpp );
					}
				}
			}
		}else{
			SetTimer( TIMER_ID2, TIMEOUT, NULL );
			SetWindowText(APP_NAME", No camera found.");
		}
		
	}

	CDialog::OnTimer(nIDEvent);
}

//*****************************************************************************

void CViewerDlg::RefreshWindowText()
{
	CString caption_base;
	CString t;
	CString zoomStr;
	if(sdlgOpened)sdlg->m_zoom.GetWindowText(zoomStr);
	else sdlg->m_zoom.GetWindowText(zoomStr);
	
	caption_base.Format(APP_NAME " %s, SN(%s), Fr:%d, Fps:%.1f, Disp:%.1f, (%dx%d), Zoom(%s), ", lps->devName, lps->devSerialNumber, lps->dwNframes, (float)lps->fFPS_sum, (float)lps->fDisp_sum, lps->dwMaxCx, lps->dwMaxCy, zoomStr);
	if(lps->bDrawImage)
	{
		if(lps->imgs.frm == XI_MONO8 || lps->imgs.frm == XI_RAW8)
			t.Format("%s, x:%d y:%d v:%d", caption_base, lps->px, lps->py, lps->pv);
		else
			t.Format("%s, x:%d y:%d vR:%d vG:%d vB:%d", caption_base, lps->px, lps->py, lps->pvR, lps->pvG, lps->pvB);
		
		SetWindowText(t);
	}
	else
		SetWindowText(caption_base);
}

//*****************************************************************************

void CViewerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect wndsize;
	CRect swndsize;

	if(sdlgOpened)
	{
		GetWindowRect( &wndsize );
		sdlg->GetWindowRect( &swndsize );
		sdlg->SetWindowPos(NULL, wndsize.right - swndsize.Width() - 8, wndsize.top + 30, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
}

//*****************************************************************************

BOOL CViewerDlg::OnEraseBkgnd(CDC* pDC)
{
	if(lps->bDrawImage && lps->bRun) return FALSE;
	else return CDialog::OnEraseBkgnd(pDC);
}

//*****************************************************************************

void CViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CString t;
	if(!lps->bDrawImage) return;
	if(!lps->img.bp) return;
	if(!lps->bRun) return;
	if(!lps->bHaveData) return;
	if(lps->bDataLocked) return;

	int x = (DWORD)point.x;
	int y = (DWORD)point.y;

	if(x < lps->x0 || x > lps->cx + lps->x0){
		lps->pv = 0;
		return;
	}
	if(y < lps->y0 || y > lps->cy + lps->y0){
		lps->pv = 0;
		return;
	}

	lps->pv = 0;
	lps->px = (DWORD)((x - lps->x0) * (float)(lps->img.width + lps->img.padding_x / lps->bpp) / lps->cx);
	lps->py = (DWORD)((y - lps->y0) * (float)lps->img.height / lps->cy);
	if((lps->img.frm == XI_MONO8 || lps->img.frm == XI_RAW8) && lps->img.nframe > 0)
		lps->pv = *((LPBYTE)lps->img.bp + (lps->py * (lps->img.width + lps->img.padding_x / lps->bpp)) + lps->px);
	if(lps->img.frm == XI_RGB32 && lps->img.nframe > 0){
		lps->px = lps->px*4;
		lps->pvB = *((LPBYTE)lps->img.bp + (lps->py * (lps->img.width + lps->img.padding_x / lps->bpp)*4) + lps->px);
		lps->pvG = *((LPBYTE)lps->img.bp + (lps->py * (lps->img.width + lps->img.padding_x / lps->bpp)*4) + lps->px + 1);
		lps->pvR = *((LPBYTE)lps->img.bp + (lps->py * (lps->img.width + lps->img.padding_x / lps->bpp)*4) + lps->px + 2);
	}

	if	(nFlags & MK_LBUTTON) {
		lps->cDragPoint = point;
		InvalidateRect( NULL, FALSE );
	}
	
	RefreshWindowText();
	CDialog::OnMouseMove(nFlags, point);
}

//*****************************************************************************

void CViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
	lps->cDragPoint = lps->cLBDpoint = point;
	InvalidateRect( NULL, FALSE );
}

//*****************************************************************************

BOOL CViewerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int	 iDelta = ((int)zDelta)/WHEEL_DELTA;
	
	int pos = 0;
	if(sdlgOpened) pos = sdlg->m_zoom.GetCurSel();
	else  pos = sdlg->m_zoom.GetCurSel();

	if((pos + iDelta) >= 0 && (pos + iDelta) < 5){
		sdlg->m_zoom.SetCurSel(pos + iDelta);
		sdlg->m_zoom.SetCurSel(pos + iDelta);
		lps->zoom = pos + iDelta;
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

//*****************************************************************************

void CViewerDlg::CloseCurrentDevice()
{
	KillTimer(TIMER_ID);
	lps->bHaveData = FALSE;
	if(sdlg != NULL){
		sdlg->ShowWindow(SW_HIDE);
		sdlg->DestroyWindow();
		delete sdlg;
		sdlg = NULL;
	}
	if(hMV != INVALID_HANDLE_VALUE){
		if (lps->bRun) shStopLive( hMV );
		xiCloseDevice( hMV );
		hMV = INVALID_HANDLE_VALUE;
	}
	SetTimer( TIMER_ID2, TIMEOUT, NULL );
}

//*****************************************************************************

#define POS_LEFT   "pos_left"
#define POS_TOP    "pos_top"
#define POS_RIGHT  "pos_right"
#define POS_BOTTOM "pos_bottom"
#define ZOOM  "zoom"

void CViewerDlg::LoadPresetsFromReg(void* handle)
{
	HKEY key_presets=NULL;
	char sn[MAX_PATH]="";
	char subKey[MAX_PATH]="";
	sdlg->isProfileLoaded=false;
	
	//--------------------------------------------------------------------
	//check if presets are saved
	xiGetParamString(handle, XI_PRM_DEVICE_SN, &sn, MAX_PATH);

	sprintf_s(subKey, "software\\XIMEA\\xiAPIViewer\\settings\\%s",sn);
	if(RegOpenKeyEx(HKEY_CURRENT_USER, subKey, 0, KEY_READ, &key_presets) != ERROR_SUCCESS)
	{
		OutputDebugString("Failed to read application presets from registry\n");
		return;
	}

	//--------------------------------------------------------------------
	//load window position
	WINDOWPLACEMENT wpp;
	DWORD dwLen=sizeof(DWORD);
	memset(&wpp, 0, sizeof(WINDOWPLACEMENT));
	wpp.length = sizeof(WINDOWPLACEMENT);
	wpp.flags = 0;
	wpp.showCmd = SW_SHOW;

	RegQueryValueEx( key_presets, POS_LEFT, NULL, NULL, (BYTE*)&wpp.rcNormalPosition.left, &dwLen);
	RegQueryValueEx( key_presets, POS_TOP, NULL, NULL, (BYTE*)&wpp.rcNormalPosition.top, &dwLen);
	RegQueryValueEx( key_presets, POS_RIGHT, NULL, NULL, (BYTE*)&wpp.rcNormalPosition.right, &dwLen);
	RegQueryValueEx( key_presets, POS_BOTTOM, NULL, NULL, (BYTE*)&wpp.rcNormalPosition.bottom, &dwLen);	
	SetWindowPlacement( &wpp );

	//--------------------------------------------------------------------
	//load application presets and update GUI
	DWORD ival=0;
	float fval=0;
	char  sval[MAX_PATH]="";

	RegQueryValueEx( key_presets, XI_PRM_DOWNSAMPLING, NULL, NULL, (BYTE*)&lps->uiDS, &dwLen);
	sdlg->m_ds.SetCurSel(lps->uiDS);
	sdlg->OnCbnSelchangeDS();

	RegQueryValueEx( key_presets, XI_PRM_IMAGE_DATA_FORMAT, NULL, NULL, (BYTE*)&lps->frm, &dwLen);
	switch(lps->frm)
	{
	case XI_RAW8:  sdlg->m_format.SetCurSel(XI_API_VW_DATA_FRM_RAW8); break;
	case XI_RAW16: sdlg->m_format.SetCurSel(XI_API_VW_DATA_FRM_RAW16); break;
	case XI_MONO8: sdlg->m_format.SetCurSel(XI_API_VW_DATA_FRM_MONO8); break;
	case XI_RGB32: sdlg->m_format.SetCurSel(XI_API_VW_DATA_FRM_RGB32); break;
	}
	sdlg->OnCbnSelchangeDataFormat();	

	RegQueryValueEx( key_presets, XI_PRM_EXPOSURE, NULL, NULL, (BYTE*)&lps->uiExposure, &dwLen);
	xiSetParamInt(handle, XI_PRM_EXPOSURE, lps->uiExposure);
	sdlg->SetExposureSlider();
	sdlg->FillExposure();
	
	dwLen=MAX_PATH;
	RegQueryValueEx( key_presets, XI_PRM_GAIN, NULL, NULL, (BYTE*)&sval, &dwLen);
	lps->fGain=(float)atof(sval);
	xiSetParamFloat(handle, XI_PRM_GAIN, lps->fGain);

	RegQueryValueEx( key_presets, XI_PRM_AEAG, NULL, NULL, (BYTE*)&lps->bAEAG, &dwLen);
	{
		CButton* butAeag = (CButton*)sdlg->GetDlgItem(IDC_AEAG);
		CSliderCtrl* slidExp = (CSliderCtrl*)sdlg->GetDlgItem(IDC_EXPOSURE); 
		CSliderCtrl* slidGain = (CSliderCtrl*)sdlg->GetDlgItem(IDC_GAIN); 
		
		xiSetParamInt(handle, XI_PRM_AEAG, lps->bAEAG);
		if(lps->bAEAG)
			butAeag->SetCheck(BST_CHECKED);
		else
		{
			butAeag->SetCheck(BST_UNCHECKED);
			slidExp->EnableWindow(TRUE); 
		    slidGain->EnableWindow(TRUE);     
		}
	}


	dwLen=sizeof(DWORD);
	RegQueryValueEx( key_presets, XI_PRM_AUTO_WB, NULL, NULL, (BYTE*)&lps->bAWB, &dwLen);
	{
		CButton* butAwb = (CButton*)sdlg->GetDlgItem(IDC_AWB);
		xiSetParamInt(handle, XI_PRM_AUTO_WB, lps->bAEAG);
		if(lps->bAEAG)
			butAwb->SetCheck(BST_CHECKED);
		else
			butAwb->SetCheck(BST_UNCHECKED);
	}

	RegQueryValueEx( key_presets, ZOOM, NULL, NULL, (BYTE*)&lps->zoom, &dwLen);
	sdlg->m_zoom.SetCurSel(lps->zoom);
	sdlg->OnCbnSelchangeZoom();

	//--------------------------------------------------------------------
	//close key
	sdlg->UpdateControls();
	sdlg->isProfileLoaded=true;
	RegCloseKey(key_presets);
}

//*****************************************************************************

void CViewerDlg::SavePresetsToReg(void* handle)
{	
	HKEY key_presets=NULL;
	char sn[MAX_PATH]="";
	char subKey[MAX_PATH]="";

	//--------------------------------------------------------------------
	//open key in registry
	xiGetParamString(handle, XI_PRM_DEVICE_SN, &sn, MAX_PATH);

	sprintf_s(subKey, "software\\XIMEA\\xiAPIViewer\\settings\\%s",sn);
	if(RegCreateKey(HKEY_CURRENT_USER, subKey, &key_presets) != ERROR_SUCCESS)
	{
		OutputDebugString("Failed to save application presets to registry\n");
		return;
	}
	
	//--------------------------------------------------------------------
	//save window position
	WINDOWPLACEMENT wp = { sizeof(WINDOWPLACEMENT) };
	GetWindowPlacement( &wp );
	DWORD ival=0;
	char sval[MAX_PATH]="";

	RegSetValueEx( key_presets, POS_LEFT, NULL, REG_DWORD, (BYTE*)&wp.rcNormalPosition.left, sizeof(DWORD));
	RegSetValueEx( key_presets, POS_TOP, NULL, REG_DWORD, (BYTE*)&wp.rcNormalPosition.top, sizeof(DWORD));
	RegSetValueEx( key_presets, POS_RIGHT, NULL, REG_DWORD, (BYTE*)&wp.rcNormalPosition.right, sizeof(DWORD));
	RegSetValueEx( key_presets, POS_BOTTOM, NULL, REG_DWORD, (BYTE*)&wp.rcNormalPosition.bottom, sizeof(DWORD));
	
	//--------------------------------------------------------------------
	//save application presets
	RegSetValueEx( key_presets, XI_PRM_DOWNSAMPLING, NULL, REG_DWORD, (BYTE*)(&lps->uiDS), sizeof(DWORD));
	RegSetValueEx( key_presets, XI_PRM_IMAGE_DATA_FORMAT, NULL, REG_DWORD, (BYTE*)&lps->frm, sizeof(DWORD));	
	RegSetValueEx( key_presets, XI_PRM_EXPOSURE, NULL, REG_DWORD, (BYTE*)&lps->uiExposure, sizeof(DWORD));

	sprintf(sval,"%f",lps->fGain);
	RegSetValueEx( key_presets, XI_PRM_GAIN, NULL, REG_SZ, (BYTE*)&sval, MAX_PATH);
	RegSetValueEx( key_presets, XI_PRM_AEAG, NULL, REG_DWORD, (BYTE*)&lps->bAEAG, sizeof(DWORD));
	RegSetValueEx( key_presets, XI_PRM_AUTO_WB, NULL, REG_DWORD, (BYTE*)&lps->bAWB, sizeof(DWORD));	
	RegSetValueEx( key_presets, ZOOM, NULL, REG_DWORD, (BYTE*)&lps->zoom, sizeof(DWORD));		

	//--------------------------------------------------------------------
	//close key
	RegCloseKey(key_presets);
}

//*****************************************************************************