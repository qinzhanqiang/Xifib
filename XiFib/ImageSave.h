#pragma once

#include "../Include/jpeg.h"
#include "afxcmn.h"

typedef jpegStatus_t (__cdecl *jpegCUDAInit)(unsigned affinity);
typedef jpegStatus_t (__cdecl *jpegCUDAMalloc)(void **buffer, size_t size);
typedef jpegStatus_t (__cdecl *jpegCUDAFree)(void *buffer);
typedef jpegStatus_t (__cdecl *jpegCUDACreateEncoderHandle)(
    jpegEncoderHandle_t *handle,
    jpegSurfaceFormat_t  surfaceFmt,
    jpegSubsamplingFormat_t samplingFmt,
    unsigned               surfaceHeight,
    unsigned               surfaceWidth,
    unsigned               surfacePitch8,
	jpegCorrectionCoefficients_t	*colorBalance
);

typedef jpegStatus_t (__cdecl *jpegCUDADestroyEncoderHandle)(jpegEncoderHandle_t handle);

typedef jpegStatus_t (__cdecl *jpegCUDAEncode)(
    jpegEncoderHandle_t handle,

    unsigned char        *h_Bytestream,
    unsigned             *bytestreamSize,

    jpegQuantState_t   *quantTableState,
    jpegHuffmanState_t *huffmanTableState,
    jpegScanStruct_t      *scanMap,

    unsigned              quality,
    unsigned              restartInterval,
    const unsigned char  *h_Surface
);

typedef jpegStatus_t (__cdecl *fvjfifStoreCUDA)(
    const char                  *filename,

    const unsigned char         *h_Bytestream,
    unsigned                     bytestreamSize,

    const jpegQuantState_t    *quantState,
    const jpegHuffmanState_t  *huffmanState,
    const jpegScanStruct_t    *scanMap,

    unsigned                     surfaceHeight,
    unsigned                     surfaceWidth,
    jpegSubsamplingFormat_t       samplingFmt,
    unsigned                     restartInterval
);

enum{
	IMAGE_SAVE_TYPE_NONE = 0,
	IMAGE_SAVE_TYPE_JPEG_CUDA = 1,
	IMAGE_SAVE_TYPE_BMP = 2,
};

// CImageSave dialog

class CImageSave : public CDialog
{
	DECLARE_DYNAMIC(CImageSave)

public:
	CImageSave(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImageSave();

// Dialog Data
	enum { IDD = IDD_IMAGE_SAVE };

	unsigned char *h_JpegCUDABytestream;
	unsigned char *h_JpegCUDAResult;
	jpegEncoderHandle_t hCUDAEncoder;
	BOOL	isJpegCUDAInit;
	BOOL	isJpegInit;
	int JpegCUDAwidth;
	int JpegCUDAheight;
	int Quality;

	HINSTANCE hJpegCUDA;
	jpegCUDAInit jpegInit; 
	jpegCUDAMalloc jpegMalloc;
	jpegCUDAFree jpegFree;
	jpegCUDACreateEncoderHandle jpegCreateEncoderHandle;
	jpegCUDADestroyEncoderHandle jpegDestroyEncoderHandle;
	jpegCUDAEncode jpegEncode;
	fvjfifStoreCUDA fvjfifStore;


	int image_save_tts;
	int image_save_number;
	float image_save_fps;
	DWORD		pcn_acq,pcn0_acq;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedIfJpegCude();
	afx_msg void OnBnClickedIfNone();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSaveOneImage();
	afx_msg void OnBnClickedStartStopImgSave();
	BOOL m_img_save_ss;
	BOOL m_img_save_one;
	afx_msg void OnClose();
protected:
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg void OnDestroy();
	CString m_img_save_path;
	afx_msg void OnBnClickedImgSaveBrowsePath();
	CSliderCtrl m_jpeg_quality;
	afx_msg void OnNMReleasedcaptureJpegQuality(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_img_save_tts;
	LRESULT UpdateDialogMsg(WPARAM wParam, LPARAM lParam);
	CString m_img_save_number;
	CString m_img_save_fps;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_img_save_format;
	afx_msg void OnBnClickedIfBmp();
};
