// Setup.cpp : implementation file
//

#include "stdafx.h"
//#include "Viewer.h"
#include "XiFib.h"
#include "ViewerDlg.h"
#include "Setup.h"
#include "acquisition.h"
#include <math.h>
#include "Gpio.h"
#include <stdio.h>
#include "../Include/m3api.h"

int WM_UPDATE_DIALOG = RegisterWindowMessage( "UPDATE_DIALOG");

#define SUPPORT_FOR_RAW16_ENABLED 0 // default:0 =1 to enable access to RAW16 modes
#define PIXEL_CLOCK_FREQ_PRINT    0 // default:0 =1 to enable printing of sensor frequency to terminal

static unsigned char taps_lut[16];

extern CString strSetups;
extern CString strDefault;

extern LPSETUP lps;
extern pdsList pDSL;
// CSetup dialog

IMPLEMENT_DYNAMIC(CSetup, CDialog)

CSetup::CSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CSetup::IDD, pParent)
	, m_exp_txt(_T(""))
	, m_gain_txt(_T(""))
	, m_draw(FALSE)
	, m_roi_x0(0)
	, m_roi_y0(0)
	, m_roi_cx(0)
	, m_roi_cy(0)
	, m_ae_limit_txt(_T(""))
	, m_ag_limit_txt(_T(""))
	, m_aeag_level_txt(_T(""))
	, m_eg_ratio_txt(_T(""))
	, m_note(TRUE)
	, m_fps_str(_T(""))
{
	isProfileLoaded=FALSE;	
	hImageSaveDlg = NULL;
}

CSetup::~CSetup()
{
}

void CSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EXP_TXT, m_exp_txt);
	DDX_Control(pDX, IDC_EXPOSURE, m_ExposureSlider);
	DDX_Control(pDX, IDC_GAIN, m_GainSlider);

	DDX_Text(pDX, IDC_GAMMY_TXT, m_gammy_txt);
	DDX_Text(pDX, IDC_GAMMC_TXT, m_gmmc_txt);
	DDX_Text(pDX, IDC_SHARP_TXT, m_sharp_txt);

	DDX_Text(pDX, IDC_WB_KR, m_kR_txt);
	DDX_Text(pDX, IDC_WB_KG, m_kG_txt);
	DDX_Text(pDX, IDC_WB_KB, m_kB_txt);

	DDX_Control(pDX, IDC_GAMMY, m_GammYSlider);
	DDX_Control(pDX, IDC_GAMMC, m_GammCSlider);
	DDX_Control(pDX, IDC_SHARP, m_SharpSlider);

	DDX_Control(pDX, IDC_CSATURATION, m_SatSlider);
	DDX_Text(pDX, IDC_GAIN_TXT, m_gain_txt);
	DDX_Check(pDX, IDC_DRAW, m_draw);
	DDX_Control(pDX, IDC_STARTSTOP, m_ss);
	DDX_Text(pDX, IDC_ROI_X0, m_roi_x0);
	DDX_Text(pDX, IDC_ROI_Y0, m_roi_y0);
	DDX_Text(pDX, IDC_ROI_CX, m_roi_cx);
	DDX_Text(pDX, IDC_ROI_CY, m_roi_cy);
	DDX_Text(pDX, IDC_TEMP_CHIP, m_chip_temp);
	DDX_Text(pDX, IDC_TEMP_HOUS, m_hous_temp);
	DDX_Control(pDX, IDC_ZOOM, m_zoom);
	DDX_Control(pDX, IDC_DS, m_ds);
	DDX_Control(pDX, IDC_DATA_FORMAT, m_format);
	DDX_Control(pDX, IDC_AE_LIMIT, m_ae_limit);
	DDX_Control(pDX, IDC_AG_LIMIT, m_ag_limit);
	DDX_Text(pDX, IDC_EXP_LIMIT, m_ae_limit_txt);
	DDX_Text(pDX, IDC_GAIN_LIMIT, m_ag_limit_txt);
	DDX_Control(pDX, IDC_AEAG_LEVEL, m_aeag_level);
	DDX_Text(pDX, IDC_AEAG_LEVEL_TXT, m_aeag_level_txt);
	DDX_Control(pDX, IDC_EG_RATIO, m_eg_ratio);
	DDX_Text(pDX, IDC_EG_RATIO_TXT, m_eg_ratio_txt);
	DDX_Text(pDX, IDC_CC00, m_CC_00);
	DDX_Text(pDX, IDC_CC01, m_CC_01);
	DDX_Text(pDX, IDC_CC02, m_CC_02);
	DDX_Text(pDX, IDC_CC03, m_CC_03);
	DDX_Text(pDX, IDC_CC10, m_CC_10);
	DDX_Text(pDX, IDC_CC11, m_CC_11);
	DDX_Text(pDX, IDC_CC12, m_CC_12);
	DDX_Text(pDX, IDC_CC13, m_CC_13);
	DDX_Text(pDX, IDC_CC20, m_CC_20);
	DDX_Text(pDX, IDC_CC21, m_CC_21);
	DDX_Text(pDX, IDC_CC22, m_CC_22);
	DDX_Text(pDX, IDC_CC23, m_CC_23);
	DDX_Text(pDX, IDC_CC30, m_CC_30);
	DDX_Text(pDX, IDC_CC31, m_CC_31);
	DDX_Text(pDX, IDC_CC32, m_CC_32);
	DDX_Text(pDX, IDC_CC33, m_CC_33);
	DDX_Control(pDX, IDC_TRG_MODE, m_trigger);
	DDX_Check(pDX, IDC_NOTE, m_note);
	DDX_Control(pDX, IDC_TRG_SELECTOR, m_trigger_sel);
	DDX_Control(pDX, IDC_FRAME_BURST_COUNT, m_fr_b_cnt);
	DDX_Text(pDX, IDC_ACQ_FRAME_BURST_COUNT_TXT, m_fr_b_cnt_str);
	DDX_Control(pDX, IDC_FPS_SLIDER, m_fps);
	DDX_Text(pDX, IDC_FPS_TXT, m_fps_str);
	DDX_Control(pDX, IDC_TAPS, m_taps);

	DDX_Text(pDX, IDC_TXT_HDR_T1, m_HDR_T1_txt);
	DDX_Text(pDX, IDC_TXT_HDR_T2, m_HDR_T2_txt);
	DDX_Text(pDX, IDC_TXT_HDR_KP1, m_HDR_KP1_txt);
	DDX_Text(pDX, IDC_TXT_HDR_KP2, m_HDR_KP2_txt);
	DDX_Text(pDX,IDC_TXT_HDR_SIMPLE_KP,m_HDR_SIMPLE_KP_txt);
	DDX_Text(pDX,IDC_TXT_HDR_SIMPLE_COMP,m_HDR_SIMPLE_COMP_txt);
	DDX_Control(pDX, IDC_SLIDER_HDR_T1, m_HDR_T1_Slider);
	DDX_Control(pDX, IDC_SLIDER_HDR_T2, m_HDR_T2_Slider);
	DDX_Control(pDX, IDC_SLIDER_HDR_KP1, m_HDR_KP1_Slider);
	DDX_Control(pDX, IDC_SLIDER_HDR_KP2, m_HDR_KP2_Slider);
	DDX_Control(pDX, IDC_SLIDER_HDR_SIMPLE_KP, m_HDR_SIMPLE_KP_Slider);
	DDX_Control(pDX, IDC_SLIDER_HDR_SIMPLE_COMP, m_HDR_SIMPLE_COMP_Slider);
}


BEGIN_MESSAGE_MAP(CSetup, CDialog)
	//更新设置对话框-----------------------
	ON_REGISTERED_MESSAGE(WM_UPDATE_DIALOG, UpdateDialogMsg)
	//-------------------------------------
	ON_CBN_SELCHANGE(IDC_DS, &CSetup::OnCbnSelchangeDS)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_DRAW, &CSetup::OnBnClickedDraw)
	ON_BN_CLICKED(IDC_STARTSTOP, &CSetup::OnBnClickedStartstop)
	ON_EN_CHANGE(IDC_ROI_X0, &CSetup::OnEnChangeRoiX0)
	ON_EN_CHANGE(IDC_ROI_Y0, &CSetup::OnEnChangeRoiY0)
	ON_EN_CHANGE(IDC_ROI_CX, &CSetup::OnEnChangeRoiCx)
	ON_EN_CHANGE(IDC_ROI_CY, &CSetup::OnEnChangeRoiCy)
	ON_EN_KILLFOCUS(IDC_ROI_X0, &CSetup::OnEnKillfocusRoiX0)
	ON_EN_KILLFOCUS(IDC_ROI_Y0, &CSetup::OnEnKillfocusRoiY0)
	ON_EN_KILLFOCUS(IDC_ROI_CX, &CSetup::OnEnKillfocusRoiCx)
	ON_EN_KILLFOCUS(IDC_ROI_CY, &CSetup::OnEnKillfocusRoiCy)
	ON_WM_MOUSEWHEEL()
	ON_CBN_SELCHANGE(IDC_ZOOM, &CSetup::OnCbnSelchangeZoom)
	ON_CBN_SELCHANGE(IDC_DATA_FORMAT, &CSetup::OnCbnSelchangeDataFormat)
	ON_CBN_SELCHANGE(IDC_TRG_MODE, &CSetup::OnCbnSelchangeTrigger)
	ON_BN_CLICKED(IDC_AEAG, &CSetup::OnBnClickedAeag)
	ON_BN_CLICKED(IDC_AWB, &CSetup::OnBnClickedAwb)
	ON_BN_CLICKED(IDC_TAKEWB, &CSetup::OnBnClickedTakewb)
	ON_BN_CLICKED(IDC_COOLING, &CSetup::OnBnClickedCooling)
	ON_BN_CLICKED(IDC_RGB_EQ1, &CSetup::OnBnClickedRgbEq1)
	ON_BN_CLICKED(IDC_GPIO, &CSetup::OnBnClickedGpio)
	ON_BN_CLICKED(IDC_PING, &CSetup::OnBnClickedPing)
	ON_BN_CLICKED(IDC_DEF_WB, &CSetup::OnBnClickedDefWb)
	ON_BN_CLICKED(IDC_NOTE, &CSetup::OnBnClickedNote)
	ON_BN_CLICKED(IDC_CMS_OFF, &CSetup::OnBnClickedCmsOff)
	ON_BN_CLICKED(IDC_CMS_ON, &CSetup::OnBnClickedCmsOn)
	ON_BN_CLICKED(IDC_CMS_ON_FAST, &CSetup::OnBnClickedCmsOnFast)
	ON_BN_CLICKED(IDC_BUTTON_GROUP_GAMMA, &CSetup::OnBnClickedButtonGroupGamma)
	ON_BN_CLICKED(IDC_BUTTON_GROUP_AEAG, &CSetup::OnBnClickedButtonGroupAeag)
	ON_BN_CLICKED(IDC_BUTTON_GROUP_CCM, &CSetup::OnBnClickedButtonGroupCcm)
	ON_BN_CLICKED(IDC_BUTTON_GROUP_TRIG, &CSetup::OnBnClickedButtonGroupTrig)
	ON_BN_CLICKED(IDC_BUTTON_GROUP_TEMPER, &CSetup::OnBnClickedButtonGroupTemper)
	ON_BN_CLICKED(IDC_BUTTON_GROUP_ROI, &CSetup::OnBnClickedButtonGroupRoi)
	ON_BN_CLICKED(IDC_SAVE_IMAGE, &CSetup::OnBnClickedSaveImage)
	ON_BN_CLICKED(IDC_BADPIXEL, &CSetup::OnBnClickedBadpixel)
	ON_CBN_SELCHANGE(IDC_TRG_SELECTOR, &CSetup::OnTrigSelChange)
	ON_BN_CLICKED(IDC_FPS, &CSetup::OnBnClickedFps)
	ON_CBN_SELCHANGE(IDC_TAPS, &CSetup::OnCbnSelchangeTaps)
	ON_BN_CLICKED(IDC_HDR_ENABLE, &CSetup::OnBnClickedHdrEnable)
	ON_BN_CLICKED(IDC_BUTTON_GROUP_HDR, &CSetup::OnBnClickedButtonGroupHdr)
	ON_BN_CLICKED(IDC_HDR_SIMPLIFIED_ENABLE, &CSetup::OnBnClickedHdrSimplifiedEnable)
	ON_BN_CLICKED(IDC_KP_CNT1, &CSetup::OnBnClickedKpCnt1)
	ON_BN_CLICKED(IDC_KP_CNT2, &CSetup::OnBnClickedKpCnt2)
END_MESSAGE_MAP()


// CSetup message handlers

BOOL CSetup::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_ExpandDialog = new CExpandDialog(this);

	// Set WS_EX_LAYERED on this window 
	SetWindowLong(this->m_hWnd, GWL_EXSTYLE,
	GetWindowLong(this->m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	// Make this window 70% alpha
	SetLayeredWindowAttributes(0, (255 * 80) / 100, LWA_ALPHA);


	UpdateDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSetup::UpdateDownsamplingCombo(void)
{
	printf("CSetup::UpdateDialog scan downsampling started\n");
	
	int cnt = 0;
	int ds = 0;
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_DOWNSAMPLING XI_PRM_INFO_MAX, &ds);
	m_ds.ResetContent();
	for(int i=0; i <= ds; i++) {
		if(xiSetParamInt(theApp.wDlg->hMV, XI_PRM_DOWNSAMPLING, i) == XI_OK){
			printf("CSetup::UpdateDialog scan downsampling found type: %d\n",i);
			if(i == 1){
				pDSL[cnt].dsStr.Format("2048*2048");
				pDSL[cnt].dsVal = i;
				
			}else{
				pDSL[cnt].dsStr.Format("1/%d", i);
				pDSL[cnt].dsVal = i;
			}
			m_ds.InsertString(cnt, pDSL[cnt].dsStr);
			cnt++;
		}
	}

	lps->numDsModes = cnt;
	if((int)lps->uiDS >= lps->numDsModes) lps->uiDS = 0;
	m_ds.SetCurSel(lps->uiDS);

	printf("CSetup::UpdateDialog scan downsampling done\n");


	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_DOWNSAMPLING, pDSL[lps->uiDS].dsVal);
}

void CSetup::UpdateDialog(void)       //更新设置对话框
{
	float fL, fH;
	lps->numDsModes = 0;
	UINT cnt = 0;
	UINT	size = 0;
	BOOL found_setup = FALSE;

	if (theApp.wDlg->hMV == INVALID_HANDLE_VALUE) return;

	xiGetParamString(theApp.wDlg->hMV, XI_PRM_DEVICE_SN, &lps->devSerialNumber, 255);
	xiGetParamString(theApp.wDlg->hMV, XI_PRM_DEVICE_NAME, &lps->devName, 255);

	if(!isProfileLoaded)
	{
		lps->uiExposure = 10000;
		lps->uiDS = 0;
		lps->fGain = 0;		
		lps->zoom = 4;
		lps->bAEAG = TRUE;
		lps->bAWB = TRUE;
	}

	lps->bDrawImage = TRUE;
	lps->bCooling = TRUE;
	lps->fTargetTemp = 20;
	lps->bpl = TRUE;
	lps->display_refresh = 60;
	lps->bHDR = FALSE;
	lps->bHDR_simplified = TRUE;

	int isColor = 0;
	//设置相机支持的图像格式
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_IMAGE_IS_COLOR, &isColor);
	m_format.ResetContent();
	if(isColor) {
		lps->frm = XI_RGB32;
		if(xiSetParamInt(theApp.wDlg->hMV, XI_PRM_IMAGE_DATA_FORMAT, XI_RAW8) == XI_OK)
		{
			m_format.InsertString(XI_API_VW_DATA_FRM_RAW8, "RAW8");
			m_format.InsertString(XI_API_VW_DATA_FRM_MONO8, "MONO8");
			m_format.InsertString(XI_API_VW_DATA_FRM_RGB32, "RGB32");
			m_format.SetCurSel(XI_API_VW_DATA_FRM_RGB32);
		}
		else
		{
			m_format.InsertString(XI_API_VW_DATA_FRM_RAW8, "RGB32");
			m_format.SetCurSel(XI_API_VW_DATA_FRM_RAW8);
		}
#if SUPPORT_FOR_RAW16_ENABLED
		m_format.InsertString(XI_API_VW_DATA_FRM_RAW16, "RAW16");
#endif // SUPPORT_FOR_RAW16_ENABLED				
		if(found_setup){
			if(lps->frm == XI_RAW8) m_format.SetCurSel(XI_API_VW_DATA_FRM_RAW8);
			else if(lps->frm == XI_RAW16) m_format.SetCurSel(XI_API_VW_DATA_FRM_RAW16);
			else if(lps->frm == XI_MONO8) m_format.SetCurSel(XI_API_VW_DATA_FRM_MONO8);
			else if(lps->frm == XI_RGB32) m_format.SetCurSel(XI_API_VW_DATA_FRM_RGB32);
		}
	}else{
		lps->frm = XI_MONO8;
		m_format.InsertString(XI_API_VW_DATA_FRM_RAW8, "MONO8");		
		m_format.SetCurSel(XI_API_VW_DATA_FRM_RAW8);
#if SUPPORT_FOR_RAW16_ENABLED // SUPPORT_FOR_RAW16_ENABLED
		m_format.InsertString(XI_API_VW_DATA_FRM_RAW16, "RAW16");
#endif // SUPPORT_FOR_RAW16_ENABLED
		if(found_setup){
			if(lps->frm == XI_MONO8) m_format.SetCurSel(XI_API_VW_DATA_FRM_MONO8);
			else if(lps->frm == XI_RAW16) m_format.SetCurSel(XI_API_VW_DATA_FRM_RAW16);
		}
	}
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_IMAGE_DATA_FORMAT, lps->frm);
	UpdateDownsamplingCombo();
	// disable color groups
	m_ExpandDialog->DisableGroup(ID_GROUP_CCM,!isColor);
	m_ExpandDialog->DisableGroup(ID_GROUP_GAMMA,!isColor);
	
	dwExposureLUT[0] = 0;
	dwExposureLUT[1] = 10;
	double expl = 9.99999643;
	for(int i = 2; i < 256; i++)
	{
		expl = expl * 1.09854115;
		if(expl <= 5000) dwExposureLUT[i] = (DWORD)ceil(expl);
		else dwExposureLUT[i] = (DWORD)expl;
	}

	dwExposureLUT[124] = 1000000;
	m_ExposureSlider.SetRange(0, 124);
	m_ExposureSlider.SetPageSize(1);
	m_ExposureSlider.SetTic(50);  // 1ms
	m_ExposureSlider.SetTic(67);  // 5 ms
	m_ExposureSlider.SetTic(92); // 50 ms
	m_ExposureSlider.SetTic(99); // 100 ms
	m_ExposureSlider.SetTic(124); // 1 s

	SetExposureSlider();
	FillExposure();

	m_GainSlider.SetPageSize(10);
	m_GainSlider.SetTicFreq(10);

	if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
		//获取增益的最大、最小值
		xiGetParamFloat(theApp.wDlg->hMV, XI_PRM_GAIN XI_PRM_INFO_MIN, &fL);
		xiGetParamFloat(theApp.wDlg->hMV, XI_PRM_GAIN XI_PRM_INFO_MAX, &fH);
		//设置增益
		if(fL > lps->fGain) lps->fGain = fL;
		lps->fGain = RoundTo(lps->fGain, 1);
		m_GainSlider.SetRange((int)(fL*10), (int)(fH*10), TRUE);
		m_GainSlider.SetPos((int)(lps->fGain * 10));
		lps->gainPos = m_GainSlider.GetPos();
	}

	FillGain();

	// gammay control
	if(!found_setup) lps->fGammY = (float)0.47;
	xiSetParamFloat( theApp.wDlg->hMV, XI_PRM_GAMMAY, lps->fGammY);
	m_GammYSlider.SetRange( 0, 70, TRUE);
	m_GammYSlider.SetPos(17);
	FillGammaY();
	// gammac control
	if(!found_setup) lps->fGammC = 0.5;
	xiSetParamFloat( theApp.wDlg->hMV, XI_PRM_GAMMAC, lps->fGammC);
	m_GammCSlider.SetRange( 0, 100);
	m_GammCSlider.SetPos(50);
	FillGammaC();
	// sharpness control
	if(!found_setup) lps->fSharp = 0;
	xiSetParamFloat( theApp.wDlg->hMV, XI_PRM_SHARPNESS, lps->fSharp);
	m_SharpSlider.SetRange( 0, 800, TRUE);
	m_SharpSlider.SetPos(400);
	FillSharpness();
	
	m_draw = lps->bDrawImage; 

	SetDlgItemText(IDC_STARTSTOP, "Stop");

	int tmp = 0;
	m_roi_x0 = 0;
	m_roi_y0 = 0;
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_WIDTH, &tmp);
	m_roi_cx = tmp;
	lps->dwMaxCx = tmp;
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_HEIGHT, &tmp);
	m_roi_cy = tmp;
	lps->dwMaxCy = tmp;

	if(!found_setup){
		lps->dwROIx0 = 0;
		lps->dwROIy0 = 0;
		lps->dwROIcx = m_roi_cx;
		lps->dwROIcy = m_roi_cy;
	}

	int ivalue_min = 0;
	int ivalue_max = 0;
	int ivalue = 200000;
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_AE_MAX_LIMIT XI_PRM_INFO_MIN, &ivalue_min);
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_AE_MAX_LIMIT XI_PRM_INFO_MAX, &ivalue_max);
	m_ae_limit.SetRange(ivalue_min, ivalue_max);
	m_ae_limit.SetPos(ivalue);
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_AE_MAX_LIMIT, ivalue);

	FillExposureLimit(ivalue);

	float fvalue_min = 0;
	float fvalue_max = 0;
	float fvalue = 16;
	xiGetParamFloat(theApp.wDlg->hMV, XI_PRM_AG_MAX_LIMIT XI_PRM_INFO_MIN, &fvalue_min);
	xiGetParamFloat(theApp.wDlg->hMV, XI_PRM_AG_MAX_LIMIT XI_PRM_INFO_MAX, &fvalue_max);
	m_ag_limit.SetRange((int)(fvalue_min * 10), (int)(fvalue_max * 10));
	xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_AG_MAX_LIMIT, fvalue);
	m_ag_limit.SetPos((int)(fvalue * 10));

	FillGainLimit(fvalue);

	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_AEAG_LEVEL XI_PRM_INFO_MIN, &ivalue_min);
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_AEAG_LEVEL XI_PRM_INFO_MAX, &ivalue_max);
	m_aeag_level.SetRange(ivalue_min, ivalue_max);
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_AEAG_LEVEL, &ivalue);
	m_aeag_level.SetPos(ivalue);	
	m_aeag_level_txt.Format("%d%%", ivalue);

	xiGetParamFloat(theApp.wDlg->hMV, XI_PRM_EXP_PRIORITY XI_PRM_INFO_MIN, &fvalue_min);
	xiGetParamFloat(theApp.wDlg->hMV, XI_PRM_EXP_PRIORITY XI_PRM_INFO_MAX, &fvalue_max);
	m_eg_ratio.SetRange((int)(fvalue_min * 10), (int)(fvalue_max * 10));
	fvalue = 1;
	xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_EXP_PRIORITY, fvalue);
	m_eg_ratio.SetPos((int)(fvalue * 10));
	m_eg_ratio_txt.Format("%1.2f", fvalue);

	// color correction kR, kG, kB
	if(!found_setup) lps->fkR = 1;
	m_kR_txt.Format("%1.2f", lps->fkR);
	if(!found_setup) lps->fkG = 1;
	m_kG_txt.Format("%1.2f", lps->fkG);
	if(!found_setup) lps->fkB = 1;
	m_kB_txt.Format("%1.2f", lps->fkB);
	
	m_zoom.ResetContent();
	m_zoom.InsertString( ZOOM_25, "25%" );
	m_zoom.InsertString( ZOOM_50, "50%" );
	m_zoom.InsertString( ZOOM_100, "100%" );
	m_zoom.InsertString( ZOOM_200, "200%" );
	m_zoom.InsertString( ZOOM_STR, "Stretch" );
	m_zoom.SetCurSel(lps->zoom);

	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_SENSOR_TAPS XI_PRM_INFO_MAX, &ivalue_max);
	if(ivalue_max == 1) m_taps.EnableWindow(FALSE);
	m_taps.ResetContent();
	int i = 0;
	char buffer [32];
	for(int t = 1; t <= ivalue_max; t *= 2)
	{
		m_taps.InsertString( i, _itoa(t, buffer, 10));
		taps_lut[i] = t;
		i++;
	}
	m_taps.SetCurSel(0);

	if(lps->bAEAG){
		CButton *btnAeag = (CButton *) GetDlgItem(IDC_AEAG);
		btnAeag->SetCheck(BST_CHECKED);
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_AEAG, 1);
		m_ExposureSlider.EnableWindow(FALSE);
		m_GainSlider.EnableWindow(FALSE);
	}

	CButton *btnAwb = (CButton *) GetDlgItem(IDC_AWB);
	if(isColor){
		if(lps->bAWB){
			btnAwb->SetCheck(BST_CHECKED);
			xiSetParamInt(theApp.wDlg->hMV, XI_PRM_AUTO_WB, 1);
		}
	}else{
		lps->bAWB = FALSE;
		btnAwb->EnableWindow(FALSE);
	}

	// cooling defaults
	int ret = XI_OK;
	if(lps->bCooling){
		CButton *btnCool = (CButton *) GetDlgItem(IDC_COOLING);
		btnCool->SetCheck(BST_CHECKED);
		xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_TARGET_TEMP, lps->fTargetTemp);
		ret = xiSetParamInt(theApp.wDlg->hMV, XI_PRM_COOLING, 1);
	}else{
		ret = xiSetParamInt(theApp.wDlg->hMV, XI_PRM_COOLING, 0);
	}

	if(ret == XI_OK)
		GetDlgItem(IDC_COOLING)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_COOLING)->EnableWindow(FALSE);

	// color correction defaults
	if(!found_setup){
		lps->ccMtx[0][0] = 1;
		lps->ccMtx[1][1] = 1;
		lps->ccMtx[2][2] = 1;
		lps->ccMtx[3][2] = 1;
	}
	FillCCMatrix();

	m_SatSlider.SetRange(0, 100, TRUE);
	m_SatSlider.SetTicFreq(10);
	m_SatSlider.SetPos(0);

	// trigger control defaults
	m_trigger.SetCurSel(XI_TRG_OFF);

	// trigger selector defaults
	m_trigger_sel.SetCurSel(XI_TRG_SEL_FRAME_START);

	// frame burst count slider
	int max_b_cnt = 0;
	int b_cnt = 0;
	m_fr_b_cnt.EnableWindow(TRUE);
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_ACQ_FRAME_BURST_COUNT XI_PRM_INFO_MAX, &max_b_cnt);
	{
		// limit user interface burst count
		#define MAX_UI_BURST_COUNT 20
		if (max_b_cnt>MAX_UI_BURST_COUNT)
			max_b_cnt=MAX_UI_BURST_COUNT;
	}
	m_fr_b_cnt.SetRange(1, max_b_cnt, TRUE);
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_ACQ_FRAME_BURST_COUNT, &b_cnt);
	m_fr_b_cnt.SetPos(b_cnt);
	m_fr_b_cnt_str.Format("%d", b_cnt);
	
	// frame rate defaults
	m_fps.SetRange(0,1);
	m_fps.EnableWindow(FALSE);
	m_fps_str.Empty();
	GetDlgItem(IDC_FPS_TXT)->EnableWindow(FALSE);

	//CMS disabled by default
	CheckRadioButton(IDC_CMS_OFF, IDC_CMS_ON_FAST, IDC_CMS_OFF);

	//Bad pixel correction enabled by default
	CButton *btnBpl = (CButton *) GetDlgItem(IDC_BADPIXEL);
	if(xiSetParamInt(theApp.wDlg->hMV, XI_PRM_BPC, lps->bpl) == XI_OK){
		btnBpl->SetCheck(BST_CHECKED);
	}else{
		btnBpl->EnableWindow(FALSE);
	}


	//HDR is disabled by default

    int isHDR = 0;
	int model_id=0; 
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_DEVICE_MODEL_ID, &model_id);
	switch (model_id){
		case MODEL_ID_MQ003CG_CM:  
		case MODEL_ID_MQ003MG_CM:          
		case MODEL_ID_MQ022CG_CM:          
		case MODEL_ID_MQ022MG_CM:          
		case MODEL_ID_MQ042CG_CM:          
		case MODEL_ID_MQ042MG_CM:          
		case MODEL_ID_MQ042CG_CM_TG:       
		case MODEL_ID_MQ042MG_CM_TG:       
			isHDR=1;
			break;
		default:
			isHDR=0;
			break;
	}

	if(isHDR){		
		xiGetParamInt(theApp.wDlg->hMV, XI_PRM_HDR_KNEEPOINT_COUNT XI_PRM_INFO_MAX,&(lps->max_kp_count));
		xiGetParamInt(theApp.wDlg->hMV, XI_PRM_HDR_KNEEPOINT_COUNT,&(lps->kp_count));
		
		if(lps->kp_count==1){
			CheckRadioButton(IDC_KP_CNT1, IDC_KP_CNT2, IDC_KP_CNT1);
			if(lps->max_kp_count==1){
				GetDlgItem(IDC_KP_CNT1)->EnableWindow(FALSE);
				GetDlgItem(IDC_KP_CNT2)->EnableWindow(FALSE);
			}
		}
		else{
			CheckRadioButton(IDC_KP_CNT1, IDC_KP_CNT2, IDC_KP_CNT2);
		}

		CButton *btnHDR_Simplified = (CButton *) GetDlgItem(IDC_HDR_SIMPLIFIED_ENABLE);

		lps->bHDR=false;
		lps->bHDR_simplified=false;

#define DEFAULT_KP1Y 50
#define DEFAULT_COMP 300

		m_HDR_SIMPLE_KP_Slider.SetRange(1,99,TRUE);
		m_HDR_SIMPLE_KP_Slider.SetPos(DEFAULT_KP1Y);
		lps->hdr_simple_kp1=DEFAULT_KP1Y;
		m_HDR_SIMPLE_KP_txt.Format("%d", lps->hdr_simple_kp1);

		m_HDR_SIMPLE_COMP_Slider.SetRange(1,2000,TRUE);
		m_HDR_SIMPLE_COMP_Slider.SetPos(DEFAULT_COMP);
		lps->hdr_simple_comp=DEFAULT_COMP;
		m_HDR_SIMPLE_COMP_txt.Format("%d", lps->hdr_simple_comp);
		
		int HDR_slid_pos=0;

		m_HDR_T1_Slider.SetRange(1,100,TRUE);
		xiGetParamInt(theApp.wDlg->hMV,XI_PRM_HDR_T1,&HDR_slid_pos);
		m_HDR_T1_Slider.SetPos(HDR_slid_pos);
		lps->hdr_t1=HDR_slid_pos;
		m_HDR_T1_txt.Format("%d", lps->hdr_t1);
         
		m_HDR_T2_Slider.SetRange(1,100,TRUE);
		xiGetParamInt(theApp.wDlg->hMV,XI_PRM_HDR_T2,&HDR_slid_pos);
		m_HDR_T2_Slider.SetPos(HDR_slid_pos);
		lps->hdr_t2=HDR_slid_pos;
		m_HDR_T2_txt.Format("%d", lps->hdr_t2);
		
		m_HDR_KP1_Slider.SetRange(1,100,TRUE);
		xiGetParamInt(theApp.wDlg->hMV,XI_PRM_KNEEPOINT1,&HDR_slid_pos);
		m_HDR_KP1_Slider.SetPos(HDR_slid_pos);
		lps->hdr_kp1=HDR_slid_pos;
		m_HDR_KP1_txt.Format("%d", lps->hdr_kp1);

		m_HDR_KP2_Slider.SetRange(1,100,TRUE);
		xiGetParamInt(theApp.wDlg->hMV,XI_PRM_KNEEPOINT2,&HDR_slid_pos);
		m_HDR_KP2_Slider.SetPos(HDR_slid_pos);
		lps->hdr_kp2=HDR_slid_pos;
		m_HDR_KP2_txt.Format("%d", lps->hdr_kp2);
		

		UpdateHdrControls();
	
	}
	else{
		m_ExpandDialog->DisableGroup(ID_GROUP_HDR,!isHDR);
	}


	UpdateData(FALSE);
}

void CSetup::OnCbnSelchangeDS()
{
	XI_RETURN ret;
	float fL, fH;

	lps->dwNframes = 0;
	lps->dwPframes = 0;
	lps->dwTime = 0;
	lps->fFPS = 100;
	lps->fFPS_sum = 100;

	lps->uiDS = m_ds.GetCurSel();
	if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
		ret = xiSetParamInt(theApp.wDlg->hMV, XI_PRM_DOWNSAMPLING, pDSL[lps->uiDS].dsVal);
		if(ret == XI_OK){
			int tmp = 0;
			m_roi_x0 = 0;
			m_roi_y0 = 0;
			xiGetParamInt(theApp.wDlg->hMV, XI_PRM_WIDTH, &tmp);
			m_roi_cx = tmp;
			lps->dwMaxCx = tmp;
			xiGetParamInt(theApp.wDlg->hMV, XI_PRM_HEIGHT, &tmp);
			m_roi_cy = tmp;
			lps->dwMaxCy = tmp;
			lps->dwROIcx = m_roi_cx;
			lps->dwROIcy = m_roi_cy;
			lps->cLBDpoint.x = 0;
			lps->cLBDpoint.y = 0;
			lps->cDragPoint.x = 0;
			lps->cDragPoint.y = 0;
		}
		lps->bDataLocked = TRUE;
		//设置增益
		xiGetParamFloat(theApp.wDlg->hMV, XI_PRM_GAIN XI_PRM_INFO_MIN, &fL);
		xiGetParamFloat(theApp.wDlg->hMV, XI_PRM_GAIN XI_PRM_INFO_MAX, &fH);
		if(fL > lps->fGain) lps->fGain = fL;
		lps->fGain = RoundTo(lps->fGain, 1);
		m_GainSlider.SetRange((int)(fL*10), (int)(fH*10), TRUE);
		m_GainSlider.SetPos((int)(lps->fGain * 10));
		lps->gainPos = m_GainSlider.GetPos();
		FillGain();
		UpdateFpsRange();
	}
	UpdateData(FALSE);
}

void CSetup::OnClose()
{
}

void CSetup::OnCancel()
{
	//theApp.wDlg->DestroyWindow();
	//delete m_ExpandDialog;
}

void CSetup::OnDestroy()
{
	delete m_ExpandDialog;

	if(hImageSaveDlg) delete hImageSaveDlg;
}

void CSetup::SetExposureSlider(void)
{
	int i=0;
	for(i = 0; i < 256; i++) 
	{
		if(lps->uiExposure <= dwExposureLUT[i]) 
			break;
	}
	m_ExposureSlider.SetPos(i);
}

unsigned int CSetup::GetExposure(void)
{
	int pos = m_ExposureSlider.GetPos();
	return dwExposureLUT[pos];
}

void CSetup::FillExposure(void)
{
	if(lps->uiExposure < 1000) m_exp_txt.Format("%dus", lps->uiExposure);
	if(lps->uiExposure >= 1000 && lps->uiExposure < 5000) m_exp_txt.Format("%.1fms", (float)lps->uiExposure / 1000.);
	if(lps->uiExposure >= 5000 && lps->uiExposure < 1000000) m_exp_txt.Format("%dms", lps->uiExposure / 1000);
	if(lps->uiExposure >= 1000000) m_exp_txt.Format("%.1fs", lps->uiExposure / 1000000.);
	UpdateFpsRange();
}

void CSetup::FillExposureLimit(int exp)
{
	if(lps->uiExposure < 1000) m_ae_limit_txt.Format("%dus", exp);
	if(lps->uiExposure >= 1000 && lps->uiExposure < 5000) m_ae_limit_txt.Format("%.1fms", (float)exp / 1000.);
	if(lps->uiExposure >= 5000 && lps->uiExposure < 1000000) m_ae_limit_txt.Format("%dms", exp / 1000);
	if(lps->uiExposure >= 1000000) m_ae_limit_txt.Format("%.1fs", exp / 1000000.);
}

void CSetup::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch(pScrollBar->GetDlgCtrlID()) {
		case IDC_EXPOSURE:
			{
				unsigned int expos = GetExposure();
				if(lps->uiExposure != expos){
					lps->uiExposure = expos;
					if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
						xiSetParamInt(theApp.wDlg->hMV, XI_PRM_EXPOSURE, lps->uiExposure);
					}
					FillExposure();
					UpdateData(FALSE);
				}
			}
			break;
		case IDC_GAIN:
			if(lps->fGain != m_GainSlider.GetPos() / 10.){
				lps->fGain = (float)(m_GainSlider.GetPos() / 10.);
				lps->fGain = RoundTo(lps->fGain, 1);
				FillGain();
				if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
					xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_GAIN, lps->fGain);
				}
				lps->gainPos = m_GainSlider.GetPos();
				UpdateData(FALSE);
			}
			break;
		case IDC_GAMMY:
			if(lps->fGammY != (float)(m_GammYSlider.GetPos() +30 )/100){
				lps->fGammY = (float)(m_GammYSlider.GetPos() +30 )/100;
				xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_GAMMAY, lps->fGammY);
				FillGammaY();
			}
			break;
		case IDC_GAMMC:
			if(lps->fGammC != (float)m_GammCSlider.GetPos() /100){
				lps->fGammC = (float)m_GammCSlider.GetPos() /100;
				xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_GAMMAC, lps->fGammC);
				FillGammaC();
			}
			break;
		case IDC_SHARP:
			if(lps->fSharp != (float)(m_SharpSlider.GetPos() - 400) /100){
				lps->fSharp = (float)(m_SharpSlider.GetPos() - 400) /100;
				int ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_SHARPNESS, lps->fSharp);
				FillSharpness();
			}
			break;
		case IDC_AE_LIMIT:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				int pos = m_ae_limit.GetPos();	
				xiSetParamInt(theApp.wDlg->hMV, XI_PRM_AE_MAX_LIMIT, pos);
				FillExposureLimit(pos);
				UpdateData(FALSE);
			}
			break;
		case IDC_AG_LIMIT:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				int pos = m_ag_limit.GetPos();	
				xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_AG_MAX_LIMIT, (float)pos / 10);
				FillGainLimit((float)pos / 10);
				UpdateData(FALSE);
			}
			break;
		case IDC_AEAG_LEVEL:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				int pos = m_aeag_level.GetPos();	
				xiSetParamInt(theApp.wDlg->hMV, XI_PRM_AEAG_LEVEL, pos);
				m_aeag_level_txt.Format("%d%%", pos);
				UpdateData(FALSE);
			}
			break;
		case IDC_EG_RATIO:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				int pos = m_eg_ratio.GetPos();	
				xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_EXP_PRIORITY, (float)pos / 10);
				m_eg_ratio_txt.Format("%1.2f", (float)pos / 10);
				UpdateData(FALSE);
			}
			break;
		case IDC_CSATURATION:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				UpdateSaturation(m_SatSlider.GetPos());
			}
			break;
		case IDC_FRAME_BURST_COUNT:
				UpdateAcqFrameBurstCnt(m_fr_b_cnt.GetPos());
			break;
		case IDC_FPS_SLIDER:
				UpdateFpsValue(m_fps.GetPos());
			break;
		case IDC_SLIDER_HDR_T1:
			{int pos = m_HDR_T1_Slider.GetPos();
			if (pos > lps->hdr_t2)
				pos=lps->hdr_t2;	
			m_HDR_T1_Slider.SetPos(pos);
			m_HDR_T1_txt.Format("%d",pos);
		    Update_HDR_param(pos,IDC_SLIDER_HDR_T1);
			break;}
		case IDC_SLIDER_HDR_T2:
			{int pos = m_HDR_T2_Slider.GetPos();
			if (pos < lps->hdr_t1)
				pos=lps->hdr_t1;
			m_HDR_T2_Slider.SetPos(pos);
			m_HDR_T2_txt.Format("%d",pos);
		    Update_HDR_param(pos,IDC_SLIDER_HDR_T2);
			break;}
		case IDC_SLIDER_HDR_KP1:
			{int pos = m_HDR_KP1_Slider.GetPos();
			if (pos > lps->hdr_kp2)
				pos=lps->hdr_kp2;
			m_HDR_KP1_Slider.SetPos(pos);
			m_HDR_KP1_txt.Format("%d",pos);
			Update_HDR_param(pos,IDC_SLIDER_HDR_KP1);
			break;}
		case IDC_SLIDER_HDR_KP2:
			{int pos = m_HDR_KP2_Slider.GetPos();
			if (pos < lps->hdr_kp1)
				pos=lps->hdr_kp1;
			m_HDR_KP2_Slider.SetPos(pos);
			m_HDR_KP2_txt.Format("%d",pos);
			Update_HDR_param(pos,IDC_SLIDER_HDR_KP2);
			break;}
		case IDC_SLIDER_HDR_SIMPLE_KP:
			{int pos = m_HDR_SIMPLE_KP_Slider.GetPos();
			m_HDR_SIMPLE_KP_Slider.SetPos(pos);
			m_HDR_SIMPLE_KP_txt.Format("%d",pos);
			lps->hdr_simple_kp1=pos;
		    Update_simplified_HDR_param();
			break;}
		case IDC_SLIDER_HDR_SIMPLE_COMP:
			{int pos = m_HDR_SIMPLE_COMP_Slider.GetPos();
			if (pos < 200)
				pos=200;
			m_HDR_SIMPLE_COMP_Slider.SetPos(pos);
			m_HDR_SIMPLE_COMP_txt.Format("%d",pos);
			lps->hdr_simple_comp=pos;
			Update_simplified_HDR_param();
			break;}
	}

	lps->dwNframes = 0;
	lps->dwPframes = 0;
	lps->dwTime = 0;
	lps->fFPS = 100;
	lps->fFPS_sum = 100;
	if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSetup::FillCCMatrix()
{
	m_CC_00.Format("%1.2f", lps->ccMtx[0][0]);
	m_CC_01.Format("%1.2f", lps->ccMtx[0][1]);
	m_CC_02.Format("%1.2f", lps->ccMtx[0][2]);
	m_CC_03.Format("%1.2f", lps->ccMtx[0][3]);
	m_CC_10.Format("%1.2f", lps->ccMtx[1][0]);
	m_CC_11.Format("%1.2f", lps->ccMtx[1][1]);
	m_CC_12.Format("%1.2f", lps->ccMtx[1][2]);
	m_CC_13.Format("%1.2f", lps->ccMtx[1][3]);
	m_CC_20.Format("%1.2f", lps->ccMtx[2][0]);
	m_CC_21.Format("%1.2f", lps->ccMtx[2][1]);
	m_CC_22.Format("%1.2f", lps->ccMtx[2][2]);
	m_CC_23.Format("%1.2f", lps->ccMtx[2][3]);
	m_CC_30.Format("%1.2f", lps->ccMtx[3][0]);
	m_CC_31.Format("%1.2f", lps->ccMtx[3][1]);
	m_CC_32.Format("%1.2f", lps->ccMtx[3][2]);
	m_CC_33.Format("%1.2f", lps->ccMtx[2][3]);
	UpdateData(FALSE);
}

//Saturation饱和度
void CSetup::UpdateSaturation(int val)
{
	float k = (float)val/100;

	lps->ccMtx[0][0] = (float)(1.+2*k);
	lps->ccMtx[0][1] = -k;
	lps->ccMtx[0][2] = -k;

	lps->ccMtx[1][0] = -k;
	lps->ccMtx[1][1] = (float)(1.+2*k);
	lps->ccMtx[1][2] = -k;

	lps->ccMtx[2][0] = -k;
	lps->ccMtx[2][1] = -k;
	lps->ccMtx[2][2] = (float)(1.+2*k);

	xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_00, lps->ccMtx[0][0]);
	xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_00, lps->ccMtx[0][1]);
	xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_00, lps->ccMtx[0][2]);
	
	xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_00, lps->ccMtx[1][0]);
	xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_00, lps->ccMtx[1][1]);
	xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_00, lps->ccMtx[1][2]);
	
	xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_00, lps->ccMtx[2][0]);
	xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_00, lps->ccMtx[2][1]);
	xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_00, lps->ccMtx[2][2]);

	FillCCMatrix();
}

void CSetup::FillGammaY(void)
{
	m_gammy_txt.Format("%1.2f", lps->fGammY);
	UpdateData(FALSE);
}

void CSetup::FillGammaC(void)
{
	m_gmmc_txt.Format("%.0f%%", lps->fGammC*100);
	UpdateData(FALSE);
}

void CSetup::FillSharpness(void)
{
	m_sharp_txt.Format("%.0f%%", lps->fSharp*100);
	UpdateData(FALSE);
}

void CSetup::UpdateAcqFrameBurstCnt(int val)
{
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_ACQ_FRAME_BURST_COUNT, val);
	m_fr_b_cnt.SetPos(val);
	m_fr_b_cnt_str.Format("%d",m_fr_b_cnt.GetPos());
	UpdateData(FALSE);	
}

//设置帧速率
void CSetup::UpdateFpsRange()
{
	if(IsDlgButtonChecked(IDC_FPS))
	{
		float fps=0, fps_min=0, fps_max=0;

		xiGetParamFloat(theApp.wDlg->hMV, XI_PRM_FRAMERATE, &fps);
		xiGetParamFloat(theApp.wDlg->hMV, XI_PRM_FRAMERATE XI_PRM_INFO_MIN, &fps_min);
		xiGetParamFloat(theApp.wDlg->hMV, XI_PRM_FRAMERATE XI_PRM_INFO_MAX, &fps_max);
		
		#define FPS_RANGE_SAFE_MARGIN 1
		m_fps.SetRange((int)(fps_min*10+FPS_RANGE_SAFE_MARGIN), (int)(fps_max*10-FPS_RANGE_SAFE_MARGIN), TRUE);
		m_fps.SetPos((int)fps*10);
		m_fps_str.Format("%4.1f", fps);
		UpdateData(FALSE);
	}
#if PIXEL_CLOCK_FREQ_PRINT
	// print pixel clock
	if (theApp.wDlg->hMV)
	{
		DWORD freq=0;
		mmGetExtention(theApp.wDlg->hMV,EXT_FREQUENCY,&freq);
		printf("Pixel clock frequency %dMhz\n",freq);
	}
#endif // PIXEL_CLOCK_FREQ_PRINT
}

//更新帧速率
void CSetup::UpdateFpsValue(int val)
{
	if(IsDlgButtonChecked(IDC_FPS))
	{
		float fps= ((float)val)/10;
		int ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_FRAMERATE, fps);
		if(ret != XI_OK)
			printf("Error %d setting framerate %4.1f \n", ret, fps);
		m_fps_str.Format("%4.1f", fps);
		lps->fUserFrameRateFPS = fps;
		UpdateData(FALSE);
	}	
}

void CSetup::Update_HDR_param(int val, int  hdr_slider_idc)
{
	#define Check(f) {if (XI_OK!=f) return;}
	if(IsDlgButtonChecked(IDC_HDR_ENABLE))
	{
		int ret=XI_OK;
		int var_offset=0;
		switch (hdr_slider_idc){
			case IDC_SLIDER_HDR_T1:
				Check(xiSetParamInt(theApp.wDlg->hMV, XI_PRM_HDR_T1, val));
				lps->hdr_t1= val;
				break;
			case IDC_SLIDER_HDR_T2:
				Check(xiSetParamInt(theApp.wDlg->hMV, XI_PRM_HDR_T2, val));
				lps->hdr_t2 = val;
				break;
			case IDC_SLIDER_HDR_KP1:
				Check(xiSetParamInt(theApp.wDlg->hMV, XI_PRM_KNEEPOINT1, val));
				lps->hdr_kp1 = val;
				break;
			case IDC_SLIDER_HDR_KP2:
				Check(xiSetParamInt(theApp.wDlg->hMV, XI_PRM_KNEEPOINT2, val));
				lps->hdr_kp2 = val;
				break;}
		
		UpdateData(FALSE);
	}	
}

void CSetup::Update_simplified_HDR_param()
{

	// Simplified hdr parameters kp1x=kp1y,kp2x,kp2y define the kneepoints in the theoretical signal vs. light intensity plot (calculated from the PWLR parameters). The comp parameter defines the maximum amount of light intensity at 100 % of signal response

	float  kp1y=(float)lps->hdr_simple_kp1;
	float  kp2y=0;
	float  kp2x=0;
	float  comp=(float)lps->hdr_simple_comp;

	// Constraints due to 2 sliders available
	kp2y=50+kp1y/2;
	kp2x=(2*kp2y*kp1y-200*kp1y-kp2y*comp+kp1y*comp)/(kp1y+kp2y-200);
	
	//calculation of PWLR parameters from simplified hdr parameters

	float kp1=0;
	float kp2=0;
	float t1=0;
	float t2=0;
	
	
	if(lps->kp_count==2)
	{
		kp1=kp1y*(kp2x-kp2y)/(kp2x-kp1y);
		kp2=kp2x*(kp2y-kp2x+comp-100)/(comp-kp2x)+kp2y-kp2x;
		t1=100*(kp2x-kp2y)/(kp2x-kp1y);
		t2=100*(kp2y-kp2x+comp-100)/(comp-kp2x);
	}
	else
	{
		t1=0;
		t2=100*(comp-100)/(comp-kp1y);
		kp1=0;
		kp2=t2*kp1y/100;
	}
	

	if ((int)kp1 >= lps->hdr_kp2){
		Update_HDR_param(int(kp2),IDC_SLIDER_HDR_KP2);
		Update_HDR_param(int(kp1),IDC_SLIDER_HDR_KP1);}
	else{
		Update_HDR_param(int(kp1),IDC_SLIDER_HDR_KP1);
		Update_HDR_param(int(kp2),IDC_SLIDER_HDR_KP2);}

	if ((int)t1 >= lps->hdr_t2){
		Update_HDR_param(int(t2),IDC_SLIDER_HDR_T2);
		Update_HDR_param(int(t1),IDC_SLIDER_HDR_T1);}
	else{
		Update_HDR_param(int(t1),IDC_SLIDER_HDR_T1);
		Update_HDR_param(int(t2),IDC_SLIDER_HDR_T2);}


	m_HDR_T1_Slider.SetPos(lps->hdr_t1);
	m_HDR_T1_txt.Format("%d",lps->hdr_t1);

	m_HDR_T2_Slider.SetPos(lps->hdr_t2);
	m_HDR_T2_txt.Format("%d",lps->hdr_t2);

	m_HDR_KP1_Slider.SetPos(lps->hdr_kp1);
	m_HDR_KP1_txt.Format("%d",lps->hdr_kp1);

	m_HDR_KP2_Slider.SetPos(lps->hdr_kp2);
	m_HDR_KP2_txt.Format("%d",lps->hdr_kp2);

	UpdateData(FALSE);
		
}

void CSetup::UpdateHdrControls()
{
	
		int HDR_slid_pos=0;
		int API_set_value=0;
		
		xiGetParamInt(theApp.wDlg->hMV, XI_PRM_HDR_KNEEPOINT_COUNT XI_PRM_INFO_MAX,&(lps->max_kp_count));
		xiGetParamInt(theApp.wDlg->hMV, XI_PRM_HDR_KNEEPOINT_COUNT,&(lps->kp_count));
		
		if(lps->bHDR){
			GetDlgItem(IDC_KP_CNT1)->EnableWindow(TRUE);
			GetDlgItem(IDC_KP_CNT2)->EnableWindow(TRUE);
			m_HDR_SIMPLE_KP_Slider.EnableWindow(TRUE);
			m_HDR_SIMPLE_COMP_Slider.EnableWindow(TRUE);
			m_HDR_T1_Slider.EnableWindow(TRUE);
			m_HDR_T2_Slider.EnableWindow(TRUE);
			m_HDR_KP1_Slider.EnableWindow(TRUE);
			m_HDR_KP2_Slider.EnableWindow(TRUE);

			if(lps->kp_count==1){
				CheckRadioButton(IDC_KP_CNT1, IDC_KP_CNT2, IDC_KP_CNT1);
				m_HDR_T1_Slider.EnableWindow(FALSE);
				m_HDR_KP1_Slider.EnableWindow(FALSE);
				if(lps->max_kp_count==1){
					GetDlgItem(IDC_KP_CNT1)->EnableWindow(FALSE);
					GetDlgItem(IDC_KP_CNT2)->EnableWindow(FALSE);}}
			else{
				CheckRadioButton(IDC_KP_CNT1, IDC_KP_CNT2, IDC_KP_CNT2);}

			if (lps->bHDR_simplified){
				HDR_slid_pos=lps->hdr_simple_kp1;
				m_HDR_SIMPLE_KP_Slider.SetPos(HDR_slid_pos);
				m_HDR_SIMPLE_KP_txt.Format("%d", lps->hdr_simple_kp1);

				HDR_slid_pos=lps->hdr_simple_comp;
				m_HDR_SIMPLE_COMP_Slider.SetPos(HDR_slid_pos);
				m_HDR_SIMPLE_COMP_txt.Format("%d", lps->hdr_simple_comp);
				
				m_HDR_T1_Slider.EnableWindow(FALSE);
				m_HDR_T2_Slider.EnableWindow(FALSE);
				m_HDR_KP1_Slider.EnableWindow(FALSE);
				m_HDR_KP2_Slider.EnableWindow(FALSE);
                
				GetDlgItem(IDC_KP_CNT1)->EnableWindow(FALSE);
				GetDlgItem(IDC_KP_CNT2)->EnableWindow(FALSE);

				HDR_slid_pos=m_HDR_SIMPLE_KP_Slider.GetPos();
				lps->hdr_simple_kp1=HDR_slid_pos;
				HDR_slid_pos=m_HDR_SIMPLE_COMP_Slider.GetPos();
				lps->hdr_simple_comp=HDR_slid_pos;
				m_HDR_SIMPLE_KP_txt.Format("%d", lps->hdr_simple_kp1);
				m_HDR_SIMPLE_COMP_txt.Format("%d", lps->hdr_simple_comp);
				Update_simplified_HDR_param();}
			else{
				m_HDR_SIMPLE_KP_Slider.EnableWindow(FALSE);
				m_HDR_SIMPLE_COMP_Slider.EnableWindow(FALSE);
				
                xiGetParamInt(theApp.wDlg->hMV,XI_PRM_HDR_T1,&API_set_value);
				lps->hdr_t1=API_set_value;
				m_HDR_T1_Slider.SetPos(lps->hdr_t1);
				m_HDR_T1_txt.Format("%d", lps->hdr_t1);
			
				xiGetParamInt(theApp.wDlg->hMV,XI_PRM_HDR_T2,&API_set_value);
				HDR_slid_pos=m_HDR_T2_Slider.GetPos();
				lps->hdr_t2=API_set_value;
				m_HDR_T2_Slider.SetPos(lps->hdr_t2);
				m_HDR_T2_txt.Format("%d", lps->hdr_t2);
				
				xiGetParamInt(theApp.wDlg->hMV,XI_PRM_KNEEPOINT1,&API_set_value);
				HDR_slid_pos=m_HDR_KP1_Slider.GetPos();
				lps->hdr_kp1=API_set_value;
				m_HDR_KP1_Slider.SetPos(lps->hdr_kp1);
				m_HDR_KP1_txt.Format("%d", lps->hdr_kp1);
			
				xiGetParamInt(theApp.wDlg->hMV,XI_PRM_KNEEPOINT2,&API_set_value);
				HDR_slid_pos=m_HDR_KP2_Slider.GetPos();
				lps->hdr_kp2=API_set_value;
				m_HDR_KP2_Slider.SetPos(lps->hdr_kp2);
				m_HDR_KP2_txt.Format("%d", lps->hdr_kp2);
		
			}
		}
		else{

			m_HDR_T1_Slider.EnableWindow(FALSE);
			m_HDR_T2_Slider.EnableWindow(FALSE);
			m_HDR_KP1_Slider.EnableWindow(FALSE);
			m_HDR_KP2_Slider.EnableWindow(FALSE);

			m_HDR_SIMPLE_KP_Slider.EnableWindow(FALSE);
			m_HDR_SIMPLE_COMP_Slider.EnableWindow(FALSE);
			GetDlgItem(IDC_KP_CNT1)->EnableWindow(FALSE);
			GetDlgItem(IDC_KP_CNT2)->EnableWindow(FALSE);
		}

}

float CSetup::RoundTo(float n, int po)
{
	float p = (float)pow(10., (double)po);
    return ((float)(floor(n*p + .5)/p));
}

void CSetup::FillGain(void)
{
	m_gain_txt.Format("%.1f dB", lps->fGain);
	UpdateData(FALSE);
}

//更新设置对话框参数
void CSetup::UpdateControls(void)
{
	int ivalue = 0;
	float fvalue = 0;
	if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
		xiGetParamInt(theApp.wDlg->hMV, XI_PRM_EXPOSURE, &ivalue);
		lps->uiExposure = ivalue;
		SetExposureSlider();
		FillExposure();

		xiGetParamFloat(theApp.wDlg->hMV, XI_PRM_GAIN, &fvalue);
		lps->fGain = fvalue;
		m_GainSlider.SetPos((int)(lps->fGain * 10));
		lps->gainPos = m_GainSlider.GetPos();
		FillGain();

		if(lps->bAWB){
			xiGetParamFloat(theApp.wDlg->hMV, XI_PRM_WB_KR, &lps->fkR);
			m_kR_txt.Format("%1.2f", lps->fkR);
			xiGetParamFloat(theApp.wDlg->hMV, XI_PRM_WB_KG, &lps->fkG);
			m_kG_txt.Format("%1.2f", lps->fkG);
			xiGetParamFloat(theApp.wDlg->hMV, XI_PRM_WB_KB, &lps->fkB);
			m_kB_txt.Format("%1.2f", lps->fkB);
			UpdateData(FALSE);
		}
	}
}

//更新设置对话框
void CSetup::RefreshValues(void)
{
	if(lps->bRun) SetDlgItemText(IDC_STARTSTOP, "Stop");
	else SetDlgItemText(IDC_STARTSTOP, "Start");

	int val = 0;
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_WIDTH, &val);
	m_roi_cx = val;
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_HEIGHT, &val);
	m_roi_cy = val;
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_OFFSET_X, &val);
	m_roi_x0 = val;
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_OFFSET_Y, &val);
	m_roi_y0 = val;

	lps->dwROIcx = m_roi_cx;
	lps->dwROIcy = m_roi_cy;

	FillExposure();
	SetExposureSlider();
	FillGain();
	m_GainSlider.SetPos(lps->gainPos);
	m_zoom.SetCurSel(lps->zoom);
	m_ds.SetCurSel(lps->uiDS);

	if(lps->frm == XI_RAW8) m_format.SetCurSel(XI_API_VW_DATA_FRM_RAW8);
	else if(lps->frm == XI_MONO8) m_format.SetCurSel(XI_API_VW_DATA_FRM_MONO8);
	else if(lps->frm == XI_RAW16) m_format.SetCurSel(XI_API_VW_DATA_FRM_RAW16);
	else  m_format.SetCurSel(XI_API_VW_DATA_FRM_RGB32);

	CButton *btnAeag = (CButton *) GetDlgItem(IDC_AEAG);
	CButton *btnAwb = (CButton *) GetDlgItem(IDC_AWB);
	CButton *btnBpl = (CButton *) GetDlgItem(IDC_BADPIXEL);

	if(lps->bAEAG) btnAeag->SetCheck(BST_CHECKED);
	else btnAeag->SetCheck(BST_UNCHECKED); 

	if(lps->bAWB) btnAwb->SetCheck(BST_CHECKED);
	else btnAwb->SetCheck(BST_UNCHECKED);

	if(lps->bpl) btnBpl->SetCheck(BST_CHECKED);
	else btnBpl->SetCheck(BST_UNCHECKED);
}

void CSetup::FillGainLimit(float fGain)
{
	m_ag_limit_txt.Format("%.1f dB", fGain);
	UpdateData(FALSE);
}

void CSetup::OnBnClickedDraw()
{
	UpdateData(TRUE);
	lps->bDrawImage = m_draw; 
	theApp.wDlg->UpdateWindow();
	theApp.wDlg->InvalidateRect( NULL, FALSE );
}

//启动、暂停
void CSetup::OnBnClickedStartstop()
{
	if(lps->bRun)
		//启动
		StopLiveMode();
	else
		//暂停
		StartLiveMode();
}

void CSetup::OnEnChangeRoiX0()
{
}

void CSetup::OnEnChangeRoiY0()
{
}

void CSetup::OnEnChangeRoiCx()
{
}

void CSetup::OnEnChangeRoiCy()
{
}

void CSetup::OnEnKillfocusRoiX0()
{
	UpdateData(TRUE);
	if(m_roi_x0 & 1){
		m_roi_x0--;
	//	GetDlgItem(IDC_ROI_X0)->SetFocus();
	}
	if(m_roi_cx + m_roi_x0 > lps->dwMaxCx){
		m_roi_x0 = lps->dwMaxCx - m_roi_cx;
	//	GetDlgItem(IDC_ROI_X0)->SetFocus();
	}
	lps->dwROIx0 = m_roi_x0;
	lps->dwROIy0 = m_roi_y0;
	lps->dwROIcx = m_roi_cx;
	lps->dwROIcy = m_roi_cy;
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_OFFSET_X, lps->dwROIx0);
	UpdateData(FALSE);
}

void CSetup::OnEnKillfocusRoiY0()
{
	UpdateData(TRUE);
	if(m_roi_y0 & 1){
		m_roi_y0--;
	//	GetDlgItem(IDC_ROI_Y0)->SetFocus();
	}
	if(m_roi_cy + m_roi_y0 > lps->dwMaxCy){
		m_roi_y0 = lps->dwMaxCy - m_roi_cy;
	//	GetDlgItem(IDC_ROI_Y0)->SetFocus();
	}
	lps->dwROIx0 = m_roi_x0;
	lps->dwROIy0 = m_roi_y0;
	lps->dwROIcx = m_roi_cx;
	lps->dwROIcy = m_roi_cy;
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_OFFSET_Y, lps->dwROIy0);
	UpdateData(FALSE);
}

void CSetup::OnEnKillfocusRoiCx()
{
	UpdateData(TRUE);
	if(m_roi_cx % 4){
		m_roi_cx = m_roi_cx & 0xFFFFFFFC;
	//	GetDlgItem(IDC_ROI_CX)->SetFocus();
	}
	if(m_roi_cx < 4){
		m_roi_cx = 4;
	//	GetDlgItem(IDC_ROI_CX)->SetFocus();
	}
	if(m_roi_cx + m_roi_x0 > lps->dwMaxCx){
		m_roi_cx = lps->dwMaxCx - m_roi_x0;
	//	GetDlgItem(IDC_ROI_CX)->SetFocus();
	}
	lps->dwROIx0 = m_roi_x0;
	lps->dwROIy0 = m_roi_y0;
	lps->dwROIcx = m_roi_cx;
	lps->dwROIcy = m_roi_cy;
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_WIDTH, lps->dwROIcx);
	UpdateData(FALSE);
}

void CSetup::OnEnKillfocusRoiCy()
{
	UpdateData(TRUE);
	if(m_roi_cy % 2){
		m_roi_cy = m_roi_cy & 0xFFFFFFFE;
	//	GetDlgItem(IDC_ROI_CY)->SetFocus();
	}
	if(m_roi_cy < 2){
		m_roi_cy = 2;
	//	GetDlgItem(IDC_ROI_CY)->SetFocus();
	}
	if(m_roi_cy + m_roi_y0 > lps->dwMaxCy){
		m_roi_cy = lps->dwMaxCy - m_roi_y0;
		GetDlgItem(IDC_ROI_CY)->SetFocus();
	}
	lps->dwROIx0 = m_roi_x0;
	lps->dwROIy0 = m_roi_y0;
	lps->dwROIcx = m_roi_cx;
	lps->dwROIcy = m_roi_cy;
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_HEIGHT, lps->dwROIcy);
	UpdateData(FALSE);
}

BOOL CSetup::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CWnd *ff = GetFocus( );
	int	 iDelta = ((int)zDelta)/WHEEL_DELTA;
	float ccVal = 0;
	int ret = XI_OK;

	if      (nFlags & MK_SHIFT) iDelta *= 16;
	else if (nFlags & MK_CONTROL)  iDelta *= 128;
	else{
		if(ff->GetDlgCtrlID() == IDC_ROI_CX) iDelta *= 4;
		else iDelta *= 2;
	}

	UpdateData(TRUE);

	switch( ff->GetDlgCtrlID() ) {
		case IDC_EXP_TXT:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				int pos = m_ExposureSlider.GetPos();
				m_ExposureSlider.SetPos(pos += iDelta);
				unsigned int expos = GetExposure();
				if(lps->uiExposure != expos){
					lps->uiExposure = expos;
					xiSetParamInt(theApp.wDlg->hMV, XI_PRM_EXPOSURE, lps->uiExposure);
					FillExposure();
				}
			}
			break;
		case IDC_GAIN_TXT:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				m_GainSlider.SetPos(m_GainSlider.GetPos()+ iDelta);
				if(lps->fGain != m_GainSlider.GetPos() / 10.){
					lps->fGain = (float)(m_GainSlider.GetPos() / 10.);
					lps->fGain = RoundTo(lps->fGain, 1);
					xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_GAIN, lps->fGain);
					FillGain();
					lps->gainPos = m_GainSlider.GetPos();
				}
			}
			break;
		case IDC_GAMMY_TXT:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				m_GammYSlider.SetPos(m_GammYSlider.GetPos() + iDelta/2);
				lps->fGammY = (float)(m_GammYSlider.GetPos() + 30) /100;
				xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_GAMMAY, lps->fGammY);
				FillGammaY();
			}
			break;
		case IDC_GAMMC_TXT:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				m_GammCSlider.SetPos(m_GammCSlider.GetPos() + iDelta/2);
				lps->fGammC = (float)m_GammCSlider.GetPos() /100;
				xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_GAMMAC, lps->fGammC);
				FillGammaC();
			}
			break;
		case IDC_SHARP_TXT:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				m_SharpSlider.SetPos(m_SharpSlider.GetPos() + iDelta/2);
				lps->fSharp = (float)(m_SharpSlider.GetPos() - 400) /100;
				xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_SHARPNESS, lps->fSharp);
				FillSharpness();
			}
			break;
		case IDC_EXP_LIMIT:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				int pos = m_ae_limit.GetPos();	
				if(lps->uiExposure >= 1000 && lps->uiExposure < 50000) iDelta = iDelta* 500;
				if(lps->uiExposure >= 50000 && lps->uiExposure < 1000000) iDelta = iDelta* 5000;
				m_ae_limit.SetPos(pos + iDelta);
				xiSetParamInt(theApp.wDlg->hMV, XI_PRM_AE_MAX_LIMIT, pos + iDelta);
				FillExposureLimit(pos);
			}
			break;
		case IDC_GAIN_LIMIT:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				m_ag_limit.SetPos(m_ag_limit.GetPos() + iDelta);
				int pos = m_ag_limit.GetPos();	
				xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_AG_MAX_LIMIT, (float)pos / 10);
				FillGainLimit((float)pos / 10);
			}
			break;
		case IDC_AEAG_LEVEL_TXT:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				m_aeag_level.SetPos(m_aeag_level.GetPos()+ iDelta/2);
				int pos = m_aeag_level.GetPos();	
				xiSetParamInt(theApp.wDlg->hMV, XI_PRM_AEAG_LEVEL, pos);
				m_aeag_level_txt.Format("%d%%", pos);
			}
			break;
		case IDC_EG_RATIO_TXT:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				m_eg_ratio.SetPos(m_eg_ratio.GetPos()+ iDelta/2);
				int pos = m_eg_ratio.GetPos();	
				xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_EXP_PRIORITY, (float)pos / 10);
				m_eg_ratio_txt.Format("%1.2f", (float)pos / 10);
				UpdateData(FALSE);
			}
			break;
		case IDC_WB_KR:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				float fval = 0;
				fval = lps->fkR + (float)iDelta/200;
				if(xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_WB_KR, fval) == XI_OK){
					lps->fkR = fval;
					m_kR_txt.Format("%1.2f", lps->fkR);
				}
			}
			break;
		case IDC_WB_KG:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				float fval = 0;
				fval = lps->fkG + (float)iDelta/200;
				if(xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_WB_KG, fval) == XI_OK){
					lps->fkG = fval;
					m_kG_txt.Format("%1.2f", lps->fkG);
				}
			}
			break;
		case IDC_WB_KB:
			if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
				float fval = 0;
				fval = lps->fkB + (float)iDelta/200;
				if(xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_WB_KB, fval) == XI_OK){
					lps->fkB = fval;
					m_kB_txt.Format("%1.2f", lps->fkB);
				}
			}
			break;
		case IDC_ROI_X0:
			if((int)m_roi_x0 + iDelta >= 0){
				m_roi_x0 += iDelta;
				if(m_roi_cx + m_roi_x0 > lps->dwMaxCx) m_roi_x0 = lps->dwMaxCx - m_roi_cx;
				lps->dwROIx0 = m_roi_x0;
				lps->dwROIy0 = m_roi_y0;
				lps->dwROIcx = m_roi_cx;
				lps->dwROIcy = m_roi_cy;
				xiSetParamInt(theApp.wDlg->hMV, XI_PRM_OFFSET_X, lps->dwROIx0);
			}
			break;
		case IDC_ROI_Y0:
			if((int)m_roi_y0 + iDelta >= 0){
				m_roi_y0 += iDelta;
				if(m_roi_cy + m_roi_y0 > lps->dwMaxCy) m_roi_y0 = lps->dwMaxCy - m_roi_cy;
				lps->dwROIx0 = m_roi_x0;
				lps->dwROIy0 = m_roi_y0;
				lps->dwROIcx = m_roi_cx;
				lps->dwROIcy = m_roi_cy;
				xiSetParamInt(theApp.wDlg->hMV, XI_PRM_OFFSET_Y, lps->dwROIy0);
			}
			break;
		case IDC_ROI_CX:
			if((m_roi_cx + iDelta + m_roi_x0) <= lps->dwMaxCx && m_roi_cx + iDelta >= 4){
				m_roi_cx += iDelta;
				lps->dwROIx0 = m_roi_x0;
				lps->dwROIy0 = m_roi_y0;
				lps->dwROIcx = m_roi_cx;
				lps->dwROIcy = m_roi_cy;
				xiSetParamInt(theApp.wDlg->hMV, XI_PRM_WIDTH, lps->dwROIcx);
			}
			break;
		case IDC_ROI_CY:
			if((m_roi_cy + iDelta + m_roi_y0) <= lps->dwMaxCy && m_roi_cy + iDelta >= 2){
				m_roi_cy += iDelta;
				lps->dwROIx0 = m_roi_x0;
				lps->dwROIy0 = m_roi_y0;
				lps->dwROIcx = m_roi_cx;
				lps->dwROIcy = m_roi_cy;
				xiSetParamInt(theApp.wDlg->hMV, XI_PRM_HEIGHT, lps->dwROIcy);
			}
			break;
		case IDC_CC00:
			ccVal += lps->ccMtx[0][0] + (float)iDelta/200;
			ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_00, ccVal);
			if(ret == XI_OK){
				lps->ccMtx[0][0] = ccVal;
				m_CC_00.Format("%1.2f", lps->ccMtx[0][0]);
			}
			break;
		case IDC_CC01:
			ccVal += lps->ccMtx[0][1] + (float)iDelta/200;
			ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_01, ccVal);
			if(ret == XI_OK){
				lps->ccMtx[0][1] = ccVal;
				m_CC_01.Format("%1.2f", lps->ccMtx[0][1]);
			}
			break;
		case IDC_CC02:
			ccVal += lps->ccMtx[0][2] + (float)iDelta/200;
			ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_02, ccVal);
			if(ret == XI_OK){
				lps->ccMtx[0][2] = ccVal;
				m_CC_02.Format("%1.2f", lps->ccMtx[0][2]);
			}
			break;
		case IDC_CC03:
			ccVal += lps->ccMtx[0][3] + (float)iDelta/200;
			ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_03, ccVal);
			if(ret == XI_OK){
				lps->ccMtx[0][3] = ccVal;
				m_CC_03.Format("%1.2f", lps->ccMtx[0][3]);
			}
			break;
		case IDC_CC10:
			ccVal += lps->ccMtx[1][0] + (float)iDelta/200;
			ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_10, ccVal);
			if(ret == XI_OK){
				lps->ccMtx[1][0] = ccVal;
				m_CC_10.Format("%1.2f", lps->ccMtx[1][0]);
			}
			break;
		case IDC_CC11:
			ccVal += lps->ccMtx[1][1] + (float)iDelta/200;
			ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_11, ccVal);
			if(ret == XI_OK){
				lps->ccMtx[1][1] = ccVal;
				m_CC_11.Format("%1.2f", lps->ccMtx[1][1]);
			}
			break;
		case IDC_CC12:
			ccVal += lps->ccMtx[1][2] + (float)iDelta/200;
			ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_12, ccVal);
			if(ret == XI_OK){
				lps->ccMtx[1][2] = ccVal;
				m_CC_12.Format("%1.2f", lps->ccMtx[1][2]);
			}
			break;
		case IDC_CC13:
			ccVal += lps->ccMtx[1][3] + (float)iDelta/200;
			ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_13, ccVal);
			if(ret == XI_OK){
				lps->ccMtx[0][0] = ccVal;
				m_CC_13.Format("%1.2f", lps->ccMtx[1][3]);
			}
			break;
		case IDC_CC20:
			ccVal += lps->ccMtx[2][0] + (float)iDelta/200;
			ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_20, ccVal);
			if(ret == XI_OK){
				lps->ccMtx[2][0] = ccVal;
				m_CC_20.Format("%1.2f", lps->ccMtx[2][0]);
			}
			break;
		case IDC_CC21:
			ccVal += lps->ccMtx[2][1] + (float)iDelta/200;
			ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_21, ccVal);
			if(ret == XI_OK){
				lps->ccMtx[2][1] = ccVal;
				m_CC_21.Format("%1.2f", lps->ccMtx[2][1]);
			}
			break;
		case IDC_CC22:
			ccVal += lps->ccMtx[2][2] + (float)iDelta/200;
			ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_22, ccVal);
			if(ret == XI_OK){
				lps->ccMtx[2][2] = ccVal;
				m_CC_22.Format("%1.2f", lps->ccMtx[2][2]);
			}
			break;
		case IDC_CC23:
			ccVal += lps->ccMtx[2][3] + (float)iDelta/200;
			ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_23, ccVal);
			if(ret == XI_OK){
				lps->ccMtx[2][3] = ccVal;
				m_CC_23.Format("%1.2f", lps->ccMtx[2][3]);
			}
			break;
		case IDC_CC30:
			ccVal += lps->ccMtx[3][0] + (float)iDelta/200;
			ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_30, ccVal);
			if(ret == XI_OK){
				lps->ccMtx[3][0] = ccVal;
				m_CC_30.Format("%1.2f", lps->ccMtx[3][0]);
			}
			break;
		case IDC_CC31:
			ccVal += lps->ccMtx[3][1] + (float)iDelta/200;
			ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_31, ccVal);
			if(ret == XI_OK){
				lps->ccMtx[3][1] = ccVal;
				m_CC_31.Format("%1.2f", lps->ccMtx[3][1]);
			}
			break;
		case IDC_CC32:
			ccVal += lps->ccMtx[3][2] + (float)iDelta/200;
			ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_32, ccVal);
			if(ret == XI_OK){
				lps->ccMtx[3][2] = ccVal;
				m_CC_32.Format("%1.2f", lps->ccMtx[3][2]);
			}
			break;
		case IDC_CC33:
			ccVal += lps->ccMtx[3][3] + (float)iDelta/200;
			ret = xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_CC_MATRIX_33, ccVal);
			if(ret == XI_OK){
				lps->ccMtx[3][3] = ccVal;
				m_CC_33.Format("%1.2f", lps->ccMtx[3][3]);
			}
			break;
		case IDC_ACQ_FRAME_BURST_COUNT_TXT:
			{
				int pos = m_fr_b_cnt.GetPos() + iDelta/2;
				ret = xiSetParamInt(theApp.wDlg->hMV, XI_PRM_ACQ_FRAME_BURST_COUNT, pos);
				if(ret == XI_OK)
				{
					UpdateAcqFrameBurstCnt(pos);
				}
			}
		case IDC_FPS_TXT:
			{
				int pos = m_fps.GetPos() + iDelta/2;
				UpdateFpsValue(pos);
			}
			break;
	}

	UpdateData(FALSE);

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CSetup::OnCbnSelchangeZoom()
{
	lps->zoom = m_zoom.GetCurSel();
}

//设置图片格式
void CSetup::OnCbnSelchangeDataFormat()
{
	if(m_format.GetCurSel() == XI_API_VW_DATA_FRM_RAW8) {
		lps->frm = XI_RAW8;
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_IMAGE_DATA_FORMAT, lps->frm);
	}else if(m_format.GetCurSel() == XI_API_VW_DATA_FRM_MONO8) {
		lps->frm = XI_MONO8;
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_IMAGE_DATA_FORMAT, lps->frm);
	}else if(m_format.GetCurSel() == XI_API_VW_DATA_FRM_RAW16) {
		lps->frm = XI_RAW16;
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_IMAGE_DATA_FORMAT, lps->frm);
	}else{ 
		lps->frm = XI_RGB32;
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_IMAGE_DATA_FORMAT, lps->frm);
	}
     
	UpdateDownsamplingCombo();

	int val=0;

    xiGetParamInt(theApp.wDlg->hMV, XI_PRM_COLOR_FILTER_ARRAY, &val); 

	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_WIDTH, &val);    
	lps->dwROIcx = lps->dwMaxCx = m_roi_cx = val;
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_HEIGHT, &val);   
	lps->dwROIcy = lps->dwMaxCy = m_roi_cy = val;
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_OFFSET_X, &val); 
	lps->dwROIx0 = m_roi_x0 = val;
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_OFFSET_Y, &val); 
	lps->dwROIy0 = m_roi_y0 = val;
	UpdateFpsRange();
	UpdateData(FALSE);
}

void CSetup::StopLiveMode()
{
	if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE)
	{
		if(lps->bRun){
			shStopLive(theApp.wDlg->hMV);
			SetDlgItemText(IDC_STARTSTOP, "Start");
			lps->bHaveData = FALSE;
		}
	}
	ResetFrameCounters();
}

void CSetup::StartLiveMode()
{
	if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE)
	{
		if(!lps->bRun)
		{
			shStartLive(theApp.wDlg->hMV, theApp.wDlg);
			SetDlgItemText(IDC_STARTSTOP, "Stop");
		}
	}
	ResetFrameCounters();
}

void CSetup::ResetFrameCounters()
{
	lps->dwNframes = 0;
	lps->dwPframes = 0;
	lps->dwTime = 0;
	lps->fFPS = 100;
	lps->fFPS_sum = 100;
}

void CSetup::OnCbnSelchangeTrigger()
{
	BOOL live_mode_was_active = lps->bRun;
	StopLiveMode();
	xiSetParamInt( theApp.wDlg->hMV, XI_PRM_TRG_SOURCE, m_trigger.GetCurSel());
	if (live_mode_was_active)
		StartLiveMode();
	UpdateFpsRange();
}

void CSetup::OnBnClickedAeag()
{
	if(IsDlgButtonChecked(IDC_AEAG)){ 
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_AEAG, 1);
		lps->bAEAG = TRUE;
		m_ExposureSlider.EnableWindow(FALSE);
		m_GainSlider.EnableWindow(FALSE);
	}else{
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_AEAG, 0);
		lps->bAEAG = FALSE;
		m_ExposureSlider.EnableWindow(TRUE);
		m_GainSlider.EnableWindow(TRUE);
	}
}

void CSetup::OnBnClickedAwb()
{
	if(IsDlgButtonChecked(IDC_AWB)){
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_AUTO_WB, 1);
		lps->bAWB = TRUE;
	}else{
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_AUTO_WB, 0);
		lps->bAWB = FALSE;
	}
}

void CSetup::OnBnClickedTakewb()
{
	if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_MANUAL_WB, 0);
	}
}

void CSetup::OnBnClickedCooling()
{
	if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
		if(IsDlgButtonChecked(IDC_COOLING)){
			lps->bCooling = TRUE;
			xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_TARGET_TEMP, lps->fTargetTemp);
			xiSetParamInt(theApp.wDlg->hMV, XI_PRM_COOLING, 1);
		} else {
			lps->bCooling = FALSE;
			xiSetParamInt(theApp.wDlg->hMV, XI_PRM_COOLING, 0);
		}
	}
}

void CSetup::OnBnClickedFps()
{
	if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
		if(IsDlgButtonChecked(IDC_FPS))
		{
			xiSetParamInt(theApp.wDlg->hMV, XI_PRM_ACQ_TIMING_MODE, XI_ACQ_TIMING_MODE_FRAME_RATE);
			m_fps.EnableWindow(TRUE);
			GetDlgItem(IDC_FPS_TXT)->EnableWindow(TRUE);
			UpdateFpsRange();
		} 
		else 
		{
			m_fps.EnableWindow(FALSE);
			m_fps_str.Empty();
			GetDlgItem(IDC_FPS_TXT)->EnableWindow(FALSE);
			xiSetParamInt(theApp.wDlg->hMV, XI_PRM_ACQ_TIMING_MODE, XI_ACQ_TIMING_MODE_FREE_RUN);
		}
	}
}

void CSetup::OnBnClickedRgbEq1()
{
	// set default color matrix
	xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_DEFAULT_CC_MATRIX, 0);
	memset(&lps->ccMtx, 0, sizeof(lps->ccMtx));
	lps->ccMtx[0][0] = 1;
	lps->ccMtx[1][1] = 1;
	lps->ccMtx[2][2] = 1;
	lps->ccMtx[3][2] = 1;
	FillCCMatrix();
	m_SatSlider.SetPos(0);
}

void CSetup::OnBnClickedGpio()
{
	CGpio * gpioDlg = new CGpio(this);
	gpioDlg->DoModal();
	delete gpioDlg;
}

void CSetup::OnBnClickedPing()
{
	xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_TRG_SOFTWARE, 0);
}

void CSetup::OnBnClickedDefWb()
{
	MMFILTER mmf;
	mmApplyDefaultWhiteBalance(theApp.wDlg->hMV);
	mmGetFilter(theApp.wDlg->hMV, &mmf);
	lps->fkR = (float)mmf.rgbWhiteBal[0];
	lps->fkG = (float)mmf.rgbWhiteBal[1];
	lps->fkB = (float)mmf.rgbWhiteBal[2];
	xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_WB_KR, lps->fkR);
	xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_WB_KG, lps->fkG);
	xiSetParamFloat(theApp.wDlg->hMV, XI_PRM_WB_KB, lps->fkB);
	m_kR_txt.Format("%1.2f", lps->fkR);
	m_kG_txt.Format("%1.2f", lps->fkG);
	m_kB_txt.Format("%1.2f", lps->fkB);
	UpdateData(FALSE);
}

//更新设置
LRESULT CSetup::UpdateDialogMsg(WPARAM wParam, LPARAM lParam)
{
	MMFILTER mmf;
	mmGetFilter(theApp.wDlg->hMV, &mmf);
	lps->fkR = (float)mmf.rgbWhiteBal[0];
	lps->fkG = (float)mmf.rgbWhiteBal[1];
	lps->fkB =(float) mmf.rgbWhiteBal[2];
	m_kR_txt.Format("%1.2f", lps->fkR);
	m_kG_txt.Format("%1.2f", lps->fkG);
	m_kB_txt.Format("%1.2f", lps->fkB);
	UpdateData(FALSE);
	return FALSE;
}

void CSetup::OnBnClickedNote()
{
	UpdateData(TRUE);
}

void CSetup::OnBnClickedCmsOff()
{
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_APPLY_CMS, XI_OFF);
}

void CSetup::OnBnClickedCmsOn()
{
	int val = 0;
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_APPLY_CMS, XI_OFF);
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_CMS, &val);
	if(val != XI_CMS_EN){
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_CMS, XI_CMS_DIS);
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_CMS, XI_CMS_EN);
	}
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_APPLY_CMS, XI_ON);
}

void CSetup::OnBnClickedCmsOnFast()
{
	int val = 0;
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_APPLY_CMS, XI_OFF);
	xiGetParamInt(theApp.wDlg->hMV, XI_PRM_CMS, &val);
	if(val != XI_CMS_EN_FAST){
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_CMS, XI_CMS_DIS);
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_CMS, XI_CMS_EN_FAST);
	}
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_APPLY_CMS, XI_ON);
}

void CSetup::ToggleGroup(id_group_e group)
{
	// slow down displaying
	shSlowDownAcquisition(100);
	m_ExpandDialog->ToggleGroup(group);
	while (!m_ExpandDialog->IsExpandFinished())
	{
		Sleep(10);
	}
	shSlowDownAcquisition(1);
}

void CSetup::OnBnClickedButtonGroupGamma()
{
	ToggleGroup(ID_GROUP_GAMMA);
}

void CSetup::OnBnClickedButtonGroupAeag()
{
	ToggleGroup(ID_GROUP_AEAG);
}

void CSetup::OnBnClickedButtonGroupCcm()
{
	ToggleGroup(ID_GROUP_CCM);
}


void CSetup::OnBnClickedButtonGroupTrig()
{
	ToggleGroup(ID_GROUP_TRIGGER);
}

void CSetup::OnBnClickedButtonGroupTemper()
{
	ToggleGroup(ID_GROUP_TEMPER);
}

void CSetup::OnBnClickedButtonGroupRoi()
{
	ToggleGroup(ID_GROUP_ROI);
}

void CSetup::OnBnClickedSaveImage()
{
	char	szFilters[]="Bitmap image file (*.bmp)|*.bmp||";
	CFileDialog dlgFile(FALSE, "bmp", "*.bmp", OFN_OVERWRITEPROMPT | OFN_ENABLESIZING, szFilters );
	
	#define MAX_FILENAME 300
	CString fileName;
	dlgFile.GetOFN().lpstrFile = fileName.GetBuffer(MAX_FILENAME);
	dlgFile.GetOFN().nMaxFile = MAX_FILENAME;
	
	INT_PTR nResult = dlgFile.DoModal();
	
	if (nResult == IDOK)
	{
		// Get the internal buffer pointer of CString.
		LPTSTR pString = fileName.GetBuffer( 0 );
		if (!RequestSaveImage( pString ))
		{
			MessageBox("Error writing file","Error",MB_OK);
		}
	}
	fileName.ReleaseBuffer();
/*
	int temp = 0;
	// TODO: Add your control notification handler code here
	if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE){
		if(hImageSaveDlg == NULL){
			hImageSaveDlg = new CImageSave(this);
			hImageSaveDlg->Create(IDD_IMAGE_SAVE, this);
			hImageSaveDlg->ShowWindow(SW_SHOW);
		}
		if(hImageSaveDlg) hImageSaveDlg->ShowWindow(SW_SHOW);
	}
*/
}

void CSetup::OnBnClickedBadpixel()
{
	if(IsDlgButtonChecked(IDC_BADPIXEL)){ 
		if(xiSetParamInt(theApp.wDlg->hMV, XI_PRM_BPC, 1) != XI_OK){
			CheckDlgButton(IDC_BADPIXEL, FALSE);
			lps->bpl = FALSE;
			return;
		}
		lps->bpl = TRUE;
	}else{
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_BPC, 0);
		lps->bpl = FALSE;
	}
}

void CSetup::OnTrigSelChange()
{
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_TRG_SELECTOR, m_trigger_sel.GetCurSel());
}
void CSetup::OnCbnSelchangeTaps()
{
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_SENSOR_TAPS, taps_lut[m_taps.GetCurSel()]);
}

void CSetup::OnBnClickedHdrEnable()
{
	if(IsDlgButtonChecked(IDC_HDR_ENABLE)){ 
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_HDR, 1);
		lps->bHDR = true;}
	else{
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_HDR, 0);
		lps->bHDR = false;}
	UpdateHdrControls();
}


void CSetup::OnBnClickedButtonGroupHdr()
{
	ToggleGroup(ID_GROUP_HDR);
}

void CSetup::OnBnClickedHdrSimplifiedEnable()
{
	if(IsDlgButtonChecked(IDC_HDR_SIMPLIFIED_ENABLE)){ 
		lps->bHDR_simplified = true;
		lps->kp_count=1;
		xiSetParamInt(theApp.wDlg->hMV, XI_PRM_HDR_KNEEPOINT_COUNT, lps->kp_count);
	}else{
		lps->bHDR_simplified = false;
	}
	
	UpdateHdrControls();
}

void CSetup::OnBnClickedKpCnt1()
{
	lps->kp_count=1;
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_HDR_KNEEPOINT_COUNT, lps->kp_count);
	UpdateHdrControls();
}
void CSetup::OnBnClickedKpCnt2()
{
	lps->kp_count=2;
	xiSetParamInt(theApp.wDlg->hMV, XI_PRM_HDR_KNEEPOINT_COUNT, lps->kp_count);
	UpdateHdrControls();
}

