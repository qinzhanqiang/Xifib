// ImageSave.cpp : implementation file
//

#include "stdafx.h"
//#include "Viewer.h"
#include "XiFib.h"
#include "ViewerDlg.h"
#include "ImageSave.h"
#include <conio.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <mmsystem.h>
#include "acquisition.h"

int WM_UPDATE_IMG_SAVE_DIALOG = RegisterWindowMessage( "UPDATE_DIALOG");

extern LPSETUP lps;

#define	TIMER_ID	101
#define	TIMER_ID2	201
#define	TIMEOUT		1000	// 1sec.

// CImageSave dialog

IMPLEMENT_DYNAMIC(CImageSave, CDialog)

CImageSave::CImageSave(CWnd* pParent /*=NULL*/)
	: CDialog(CImageSave::IDD, pParent)
	, m_img_save_ss(FALSE)
	, m_img_save_path(_T("C:"))
	, m_img_save_tts(_T("0"))
	, m_img_save_number(_T("0"))
	, m_img_save_fps(_T("0"))
	, m_img_save_format(IMAGE_SAVE_TYPE_NONE)
{
	isJpegCUDAInit = FALSE;
	isJpegInit = FALSE;
	JpegCUDAwidth = 0;
	JpegCUDAheight = 0;
	h_JpegCUDABytestream = NULL;
	h_JpegCUDAResult = NULL;
	Quality = 70;
	m_img_save_one = FALSE;
	image_save_tts = 0;
	image_save_number = 0;
	image_save_fps = 0;
	pcn_acq = 0;
	pcn0_acq = 0;
}

CImageSave::~CImageSave()
{
	
}

void CImageSave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_START_STOP_IMG_SAVE, m_img_save_ss);
	DDX_Text(pDX, IDC_IMG_SAVE_PATH, m_img_save_path);
	DDX_Control(pDX, IDC_JPEG_QUALITY, m_jpeg_quality);
	DDX_Text(pDX, IDC_IMG_SAVE_TTS, m_img_save_tts);
	DDX_Text(pDX, IDC_IMG_SAVE_NUMBER, m_img_save_number);
	DDX_Text(pDX, IDC_IMG_SAVE_FPS, m_img_save_fps);
	DDX_Radio(pDX, IDC_IF_NONE, m_img_save_format);
}


BEGIN_MESSAGE_MAP(CImageSave, CDialog)
	ON_REGISTERED_MESSAGE(WM_UPDATE_IMG_SAVE_DIALOG, UpdateDialogMsg)
	ON_BN_CLICKED(IDC_IF_JPEG_CUDE, &CImageSave::OnBnClickedIfJpegCude)
	ON_BN_CLICKED(IDC_IF_NONE, &CImageSave::OnBnClickedIfNone)
	ON_BN_CLICKED(IDC_SAVE_ONE_IMAGE, &CImageSave::OnBnClickedSaveOneImage)
	ON_BN_CLICKED(IDC_START_STOP_IMG_SAVE, &CImageSave::OnBnClickedStartStopImgSave)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_IMG_SAVE_BROWSE_PATH, &CImageSave::OnBnClickedImgSaveBrowsePath)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_JPEG_QUALITY, &CImageSave::OnNMReleasedcaptureJpegQuality)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_IF_BMP, &CImageSave::OnBnClickedIfBmp)
END_MESSAGE_MAP()


// CImageSave message handlers

void CImageSave::OnBnClickedIfJpegCude()
{
	m_img_save_format = IMAGE_SAVE_TYPE_JPEG_CUDA;
	if(isJpegCUDAInit == FALSE)
	{
		hJpegCUDA = LoadLibrary("jpeg.dll");
		if (hJpegCUDA != NULL){
			if((jpegInit = (jpegCUDAInit)GetProcAddress(hJpegCUDA, "jpegInit")) == NULL){
				FreeLibrary(hJpegCUDA);
				return;
			}
			if((jpegMalloc = (jpegCUDAMalloc)GetProcAddress(hJpegCUDA, "jpegMalloc")) == NULL){
				FreeLibrary(hJpegCUDA);
				return;
			}
			if((jpegFree = (jpegCUDAFree)GetProcAddress(hJpegCUDA, "jpegFree")) == NULL){
				FreeLibrary(hJpegCUDA);
				return;
			}
			if((jpegCreateEncoderHandle = (jpegCUDACreateEncoderHandle)GetProcAddress(hJpegCUDA, "jpegCreateEncoderHandle")) == NULL){
				FreeLibrary(hJpegCUDA);
				return;
			}
			if((jpegDestroyEncoderHandle = (jpegCUDADestroyEncoderHandle)GetProcAddress(hJpegCUDA, "jpegDestroyEncoderHandle")) == NULL){
				FreeLibrary(hJpegCUDA);
				return;
			}
			if((jpegEncode = (jpegCUDAEncode)GetProcAddress(hJpegCUDA, "jpegEncode")) == NULL){
				FreeLibrary(hJpegCUDA);
				return;
			}
			if((fvjfifStore = (fvjfifStoreCUDA)GetProcAddress(hJpegCUDA, "fvjfifStore")) == NULL){
				FreeLibrary(hJpegCUDA);
				return;
			}
			isJpegCUDAInit = TRUE;
		}
	}
}

LRESULT CImageSave::UpdateDialogMsg(WPARAM wParam, LPARAM lParam)
{
	float	tau = 0.5;
	float fTPF = (float)(timeGetTime()-pcn0_acq) / 1000;
	float fFPS = 0;

	pcn0_acq = timeGetTime();

	if(fTPF) fFPS = 1/fTPF;
	if (image_save_number) image_save_fps = image_save_fps*tau + fFPS*(1-tau);
	else		      image_save_fps = fFPS;
	m_img_save_tts.Format("%d", image_save_tts);
	m_img_save_number.Format("%d", image_save_number);
	if((image_save_number % 32) == 0) m_img_save_fps.Format("%.1f", image_save_fps);
	UpdateData(FALSE);
	return FALSE;
}

void CImageSave::OnBnClickedIfNone()
{
	m_img_save_format = IMAGE_SAVE_TYPE_NONE;
}

BOOL CImageSave::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(TRUE);
	
	m_jpeg_quality.SetRangeMin(0);
	m_jpeg_quality.SetRangeMax(100);
	m_jpeg_quality.SetTicFreq(10);
	m_jpeg_quality.SetPos(Quality);

	if(m_img_save_format == IMAGE_SAVE_TYPE_NONE) CheckDlgButton(IDC_IF_NONE, TRUE);
	if(m_img_save_format == IMAGE_SAVE_TYPE_JPEG_CUDA) CheckDlgButton(IDC_IF_JPEG_CUDE, TRUE);
	if(m_img_save_format == IMAGE_SAVE_TYPE_BMP) CheckDlgButton(IDC_IF_BMP, TRUE);

	SetTimer( TIMER_ID2, TIMEOUT, NULL );

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CImageSave::OnBnClickedSaveOneImage()
{
	image_save_tts = 0;
	image_save_number = 0;
	image_save_fps = 0;
	m_img_save_one = TRUE;
}

void CImageSave::OnBnClickedStartStopImgSave()
{
	if(m_img_save_ss) m_img_save_ss = FALSE;
	else{
		image_save_tts = 0;
		image_save_number = 0;
		image_save_fps = 0;
		m_img_save_ss = TRUE;
	}
	UpdateData(FALSE);
}

void CImageSave::OnClose()
{
	m_img_save_one = FALSE;
	m_img_save_ss = FALSE;
	UpdateData(FALSE);
	CDialog::OnClose();
}

void CImageSave::OnCancel()
{
	m_img_save_one = FALSE;
	m_img_save_ss = FALSE;
	UpdateData(FALSE);

	CDialog::OnCancel();
}

void CImageSave::OnOK()
{
	m_img_save_one = FALSE;
	m_img_save_ss = FALSE;
	UpdateData(FALSE);

	CDialog::OnOK();
}

void CImageSave::OnDestroy()
{
	KillTimer(TIMER_ID2);
	if (hJpegCUDA != NULL){
		if(isJpegInit){
			jpegDestroyEncoderHandle(hCUDAEncoder);
			if(h_JpegCUDAResult) jpegFree(h_JpegCUDAResult);
			if(h_JpegCUDABytestream) jpegFree(h_JpegCUDABytestream);
		}
		FreeLibrary(hJpegCUDA);
	}
	CDialog::OnDestroy();
}

void CImageSave::OnBnClickedImgSaveBrowsePath()
{
	LPMALLOC lpMalloc;  // pointer to IMalloc
	BROWSEINFO browseInfo;
	LPITEMIDLIST lpItemIDList, pidlRoot=NULL;
	char szDisplay[_MAX_PATH]="";

	UpdateData(TRUE);
  
//	CoInitializeEx( NULL, COINIT_APARTMENTTHREADED );
	if (::SHGetMalloc(&lpMalloc) != NOERROR)
		return; // failed to get allocator

	strcpy(szDisplay, m_img_save_path);
	browseInfo.hwndOwner = m_hWnd;
	browseInfo.pszDisplayName = szDisplay;
	browseInfo.lpszTitle = "Browse for database_path files location";
	browseInfo.ulFlags = /*BIF_USENEWUI | */BIF_VALIDATE | BIF_RETURNONLYFSDIRS;
	browseInfo.lpfn = NULL;		// not used
	browseInfo.lParam = 0;		// not used
	browseInfo.iImage = 0;

	// set root at Desktop
	browseInfo.pidlRoot = pidlRoot;

	if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL) {
		// Get the path of the selected folder from the item ID list.
		::SHGetPathFromIDList(lpItemIDList, szDisplay);
		lpMalloc->Free(lpItemIDList);
		m_img_save_path.Format("%s", szDisplay);
	}
	if(pidlRoot){
		lpMalloc->Free(pidlRoot);
	}
	lpMalloc->Release();
	UpdateData(FALSE);
}

void CImageSave::OnNMReleasedcaptureJpegQuality(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	Quality = m_jpeg_quality.GetPos();
	*pResult = 0;
}

void CImageSave::OnTimer(UINT_PTR nIDEvent)
{
	if(lps->frm != XI_RAW8 || hJpegCUDA == NULL){
		GetDlgItem(IDC_IF_JPEG_CUDE)->EnableWindow(FALSE);
		if(m_img_save_format == IMAGE_SAVE_TYPE_JPEG_CUDA){
			m_img_save_format = IMAGE_SAVE_TYPE_NONE;
			CheckDlgButton(IDC_IF_NONE, TRUE);
		}
		UpdateData(FALSE);
	}else{
		GetDlgItem(IDC_IF_JPEG_CUDE)->EnableWindow(TRUE);
		UpdateData(FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

void CImageSave::OnBnClickedIfBmp()
{
	m_img_save_format = IMAGE_SAVE_TYPE_BMP;
}
