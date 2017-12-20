#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ImageSave.h"


#include "DialogExpander.h"

// CSetup dialog
#define XI_API_VW_DATA_FRM_RAW8		0
#define XI_API_VW_DATA_FRM_MONO8	1
#define XI_API_VW_DATA_FRM_RGB32	2
#define XI_API_VW_DATA_FRM_RAW16	3

class CSetup : public CDialog
{
	DECLARE_DYNAMIC(CSetup)

public:
	CSetup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetup();

// Dialog Data
	enum { IDD = IDD_SETUP };

	DWORD	dwExposureLUT[256];

	CImageSave * hImageSaveDlg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeDS();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	void StopLiveMode();
	void StartLiveMode();
	void ResetFrameCounters();
protected:
	virtual void OnCancel();
	void ToggleGroup(id_group_e id_group);

	CExpandDialog* m_ExpandDialog;
public:
	CButton m_ss;
	BOOL m_draw;
	CString m_exp_txt;
	CString m_gain_txt;
	CString m_gammy_txt;
	CString m_gmmc_txt;
	CString m_sharp_txt;
	CString m_HDR_T1_txt;
	CString m_HDR_T2_txt;
	CString m_HDR_KP1_txt;
	CString m_HDR_KP2_txt;
	CString m_HDR_SIMPLE_KP_txt;
	CString m_HDR_SIMPLE_COMP_txt;
	CString m_HDR_SIMPLE_KP2_txt;
	
	////////////////////////////////////////////////
	CSliderCtrl m_ExposureSlider;
	CSliderCtrl m_GainSlider;
	CSliderCtrl m_GammYSlider;
	CSliderCtrl m_GammCSlider;
	CSliderCtrl m_SharpSlider;
	CSliderCtrl m_SatSlider;
	CSliderCtrl m_HDR_T1_Slider;
    CSliderCtrl m_HDR_T2_Slider;
	CSliderCtrl m_HDR_KP1_Slider;
	CSliderCtrl m_HDR_KP2_Slider;
	CSliderCtrl m_HDR_SIMPLE_KP_Slider;
	CSliderCtrl m_HDR_SIMPLE_COMP_Slider;
	
	////////////////////////////////////////////////
	void SetExposureSlider(void);
	unsigned int GetExposure(void);
	void FillExposure(void);
	void FillExposureLimit(int exp);
	void FillGain(void);
	void FillGainLimit(float fGain);
	void UpdateControls(void);
	void RefreshValues(void);
	float RoundTo(float n, int po);
	void FillCCMatrix(void);
	void UpdateSaturation(int val);
	void FillGammaY(void);
	void FillGammaC(void);
	void FillSharpness(void);
	void UpdateAcqFrameBurstCnt(int val);
	void UpdateFpsRange();
	void UpdateFpsValue(int val);
	void Update_HDR_param(int val,int hdr_slider_idc);
	void Update_simplified_HDR_param(void);
	void UpdateHdrControls();
	
	////////////////////////////////////////////////
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedDraw();
	afx_msg void OnBnClickedStartstop();
	////////////////////////////////////////////////
	UINT m_roi_x0;
	UINT m_roi_y0;
	UINT m_roi_cx;
	UINT m_roi_cy;
	////////////////////////////////////////////////
	afx_msg void OnEnChangeRoiX0();
	afx_msg void OnEnChangeRoiY0();
	afx_msg void OnEnChangeRoiCx();
	afx_msg void OnEnChangeRoiCy();
	afx_msg void OnEnKillfocusRoiX0();
	afx_msg void OnEnKillfocusRoiY0();
	afx_msg void OnEnKillfocusRoiCx();
	afx_msg void OnEnKillfocusRoiCy();
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);	
	afx_msg void OnCbnSelchangeZoom();
	afx_msg void OnCbnSelchangeDataFormat();
	afx_msg void OnCbnSelchangeTrigger();
	afx_msg void OnBnClickedAeag();
	afx_msg void OnBnClickedAwb();
	afx_msg void OnBnClickedTakewb();
	afx_msg void OnBnClickedCooling();
	afx_msg void OnBnClickedFps();
	afx_msg void OnBnClickedRgbEq1();
	afx_msg void OnBnClickedGpio();
	afx_msg void OnBnClickedPing();
	afx_msg void OnBnClickedDefWb();
	////////////////////////////////////////////////
	CComboBox m_zoom;
	CComboBox m_ds;
	CComboBox m_format;
	CComboBox m_trigger;
	CComboBox m_trigger_sel;
	////////////////////////////////////////////////
	CSliderCtrl m_ae_limit;
	CSliderCtrl m_ag_limit;
	CSliderCtrl m_aeag_level;
	CSliderCtrl m_eg_ratio;
	CSliderCtrl m_fr_b_cnt;
	CSliderCtrl m_fps;	
	////////////////////////////////////////////////
	CString m_ae_limit_txt;
	CString m_ag_limit_txt;
	CString m_aeag_level_txt;
	CString m_eg_ratio_txt;
	CString m_kR_txt;
	CString m_kG_txt;
	CString m_kB_txt;
	CString m_fps_str;
	////////////////////////////////////////////////
	CString m_CC_00;
	CString m_CC_01;
	CString m_CC_02;
	CString m_CC_03;
	CString m_CC_10;
	CString m_CC_11;
	CString m_CC_12;
	CString m_CC_13;
	CString m_CC_20;
	CString m_CC_21;
	CString m_CC_22;
	CString m_CC_23;
	CString m_CC_30;
	CString m_CC_31;
	CString m_CC_32;
	CString m_CC_33;
	////////////////////////////////////////////////
	CString m_chip_temp;
	CString m_hous_temp;
	CString m_fr_b_cnt_str;
	afx_msg void OnBnClickedMorebutton();
	LRESULT UpdateDialogMsg(WPARAM wParam, LPARAM lParam);
	BOOL m_note;
	afx_msg void OnBnClickedNote();
	afx_msg void OnBnClickedCmsOff();
	afx_msg void OnBnClickedCmsOn();
	afx_msg void OnBnClickedCmsOnFast();
	afx_msg void OnBnClickedButtonGroupGamma();
	afx_msg void OnBnClickedButtonGroupAeag();
	afx_msg void OnBnClickedButtonGroupCcm();
	afx_msg void OnBnClickedButtonGroupTrig();
	afx_msg void OnBnClickedButtonGroupTemper();
	afx_msg void OnBnClickedButtonGroupRoi();
	afx_msg void OnBnClickedSaveImage();
	afx_msg void OnBnClickedBadpixel();
	afx_msg void OnTrigSelChange();
	void UpdateDialog(void);
	void UpdateDownsamplingCombo(void);
	BOOL isProfileLoaded;			
	CComboBox m_taps;
	afx_msg void OnCbnSelchangeTaps();
	afx_msg void OnNMCustomdrawEgRatio2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedHdrEnable();
	afx_msg void OnBnClickedButtonGroupHdr();
	afx_msg void OnStnClickedCmsStatic2();
	afx_msg void OnBnClickedHdrSimplifiedEnable();
	afx_msg void OnNMCustomdrawSliderHdrSimpleKp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedKpCount1();
	afx_msg void OnBnClickedKpCnt1();
	afx_msg void OnBnClickedKpCnt2();
};
