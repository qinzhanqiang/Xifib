#include "stdafx.h"
#include "DialogExpander.h"
#include "resource.h"

// -----------------------------------------------------------------------
// data types

struct group_item_id_t
{
	int id;
	CRect rect;
};

// list of groups
typedef struct group_data_t
{
	group_item_id_t* ids;
	int              ids_count;    // =number of identifiers
	char             is_visible;   // =1 if group is expanded
	char             will_be_visible;   // =1 if group is being expanded
	char             is_disabled;  // =1 for color groups in monochrome cameras
} group_data_t;

#define ADD_GROUP(group) group, sizeof(group)/sizeof(group_item_id_t), true, true, false

// -----------------------------------------------------------------------
// group definitions

#define ITEM_ID_BUTTON 0
#define ITEM_ID_GROUP 1

// group of controls for GAMMA

#define CA(id) id,NULL

static group_item_id_t group_items_ids_gamma[]=
{
	// button
	CA(IDC_BUTTON_GROUP_GAMMA),
	// group
	CA(IDC_GRP_GAMMA),
	// controls
	CA(IDC_LAB_GAMMA_Y), CA(IDC_GAMMY), CA(IDC_GAMMY_TXT),
	CA(IDC_LAB_GAMMA_C), CA(IDC_GAMMC), CA(IDC_GAMMC_TXT),
	CA(IDC_LAB_SHARP),   CA(IDC_SHARP), CA(IDC_SHARP_TXT)
};

static group_item_id_t group_items_ids_aeag[]=
{
	// button
	CA(IDC_BUTTON_GROUP_AEAG),
	// group
	CA(IDC_GRP_AEAG),
	// controls
	CA(IDC_LAB_EXPMAX),     CA(IDC_AE_LIMIT),   CA(IDC_EXP_LIMIT),
	CA(IDC_LAB_GAINMAX),    CA(IDC_AG_LIMIT),   CA(IDC_GAIN_LIMIT),
	CA(IDC_LAB_AEAG_LEVEL), CA(IDC_AEAG_LEVEL), CA(IDC_AEAG_LEVEL_TXT),
	CA(IDC_LAB_AEAG_EG),    CA(IDC_EG_RATIO),   CA(IDC_EG_RATIO_TXT),
};

static group_item_id_t group_items_ids_ccm[]=
{
	CA(IDC_BUTTON_GROUP_CCM),
	CA(IDC_GRP_CCM),
	CA(IDC_CMS_STATIC),CA(IDC_CMS_OFF),CA(IDC_CMS_ON),CA(IDC_CMS_ON_FAST),
	CA(IDC_LAB_CCM_COL_R), CA(IDC_LAB_CCM_COL_G), CA(IDC_LAB_CCM_COL_B), CA(IDC_LAB_CCM_COL_OFF),
	CA(IDC_LAB_CCM_ROW_R),
	CA(IDC_LAB_CCM_ROW_G),
	CA(IDC_LAB_CCM_ROW_B),
	CA(IDC_LAB_CCM_ROW_S),
	CA(IDC_CC00),CA(IDC_CC01),CA(IDC_CC02),CA(IDC_CC03),
	CA(IDC_CC10),CA(IDC_CC11),CA(IDC_CC12),CA(IDC_CC13),
	CA(IDC_CC20),CA(IDC_CC21),CA(IDC_CC22),CA(IDC_CC23),
	CA(IDC_CC30),CA(IDC_CC31),CA(IDC_CC32),CA(IDC_CC33),
	CA(IDC_LAB_CCM_SATUR),CA(IDC_CSATURATION),CA(IDC_RGB_EQ1),
	// controls
	CA(IDC_TAKEWB),      CA(IDC_DEF_WB),
	CA(IDC_LAB_AEAG_KR), CA(IDC_WB_KR),
	CA(IDC_LAB_AEAG_KG), CA(IDC_WB_KG),
	CA(IDC_LAB_AEAG_KB), CA(IDC_WB_KB),
};

static group_item_id_t group_items_ids_trig[]=
{
	CA(IDC_BUTTON_GROUP_TRIG),
	CA(IDC_GRP_TRIG),
	CA(IDC_LAB_TRIG_MODE), CA(IDC_TRG_MODE), CA(IDC_PING), CA(IDC_GPIO),
	CA(IDC_LAB_TRIG_SELECTOR),CA(IDC_TRG_SELECTOR),
	CA(IDC_LAB_ACQ_FRAME_BURST_COUNT),
	CA(IDC_FRAME_BURST_COUNT),CA(IDC_ACQ_FRAME_BURST_COUNT_TXT)
};

static group_item_id_t group_items_ids_temper[]=
{
	CA(IDC_BUTTON_GROUP_TEMPER),
	CA(IDC_GRP_TEMPER),
	CA(IDC_COOLING),
	CA(IDC_LAB_TEMPER_SENS), CA(IDC_TEMP_CHIP),
	CA(IDC_LAB_TEMPER_HOUS), CA(IDC_TEMP_HOUS),
};

static group_item_id_t group_items_ids_roi[]=
{
	CA(IDC_BUTTON_GROUP_ROI),
	CA(IDC_GRP_ROI),
	CA(IDC_DRAW),
	CA(IDC_NOTE),
	CA(IDC_STARTSTOP),
	CA(IDC_SAVE_IMAGE),
	CA(IDC_BADPIXEL),
	CA(IDC_LAB_ROI_X), CA(IDC_ROI_X0), CA(IDC_LAB_ROI_CX), CA(IDC_ROI_CX),
	CA(IDC_LAB_ROI_Y), CA(IDC_ROI_Y0), CA(IDC_LAB_ROI_CY), CA(IDC_ROI_CY),
	CA(IDC_FPS),CA(IDC_FPS_SLIDER),CA(IDC_FPS_TXT),

};

static group_item_id_t group_items_ids_hdr[]=
{
	CA(IDC_BUTTON_GROUP_HDR),
	CA(IDC_GRP_HDR),
	CA(IDC_HDR_ENABLE),
	CA(IDC_HDR_SIMPLIFIED_ENABLE),

	CA(IDC_GRP_HDR_SIMPLIFIED),            
	CA(IDC_LAB_HDR_SIMPLE_KP),CA(IDC_SLIDER_HDR_SIMPLE_KP),CA(IDC_TXT_HDR_SIMPLE_KP),
	CA(IDC_LAB_HDR_SIMPLE_COMP),CA(IDC_TXT_HDR_SIMPLE_COMP),CA(IDC_SLIDER_HDR_SIMPLE_COMP),

	CA(IDC_GRP_HDR_PWLR),
	CA(IDC_KP_COUNT_LABEL),CA(IDC_KP_CNT1),CA(IDC_KP_CNT2),
	CA(IDC_LAB_HDR_T1),CA(IDC_SLIDER_HDR_T1),CA(IDC_TXT_HDR_T1),
	CA(IDC_LAB_HDR_T2),CA(IDC_SLIDER_HDR_T2),CA(IDC_TXT_HDR_T2),
	CA(IDC_LAB_HDR_KP1),CA(IDC_SLIDER_HDR_KP1),CA(IDC_TXT_HDR_KP1),
	CA(IDC_LAB_HDR_KP2),CA(IDC_SLIDER_HDR_KP2),CA(IDC_TXT_HDR_KP2),
};

static group_data_t groups_lists[]=
{
	ADD_GROUP(group_items_ids_gamma),   // ID_GROUP_GAMMA
	ADD_GROUP(group_items_ids_aeag),    // ID_GROUP_AEAG
	ADD_GROUP(group_items_ids_ccm),     // ID_GROUP_CCM
	ADD_GROUP(group_items_ids_trig),    // ID_GROUP_TRIGGER
	ADD_GROUP(group_items_ids_temper),  // ID_GROUP_TEMPER
	ADD_GROUP(group_items_ids_roi),     // ID_GROUP_ROI
	ADD_GROUP(group_items_ids_hdr),     // ID_GROUP_HDR
};

#define TOTAL_GROUPS (sizeof(groups_lists)/sizeof(group_data_t))

// -----------------------------------------------------------------------
// implementation

#define GROUP_HEIGHT_MINIMIZED 15
#define GROUP_HEIGHT_DISABLED 24

#define PROGRESS_STEP 40
#define PROGRESS_STEP_TIME_MS 20
#define PROGRESS_MAX 100

void CExpandDialog::DoGroupOperation(oper_e operation,id_group_e selected_group,bool show_group)
{
	int current_group_y_shift=0;
	bool phase_done=true;
	// iterate all groups
	for (int gid=0;gid<TOTAL_GROUPS;gid++)
	{
		group_data_t* group=&groups_lists[gid];
		int items_count=group->ids_count;
		if (gid == selected_group)
		{
			group->will_be_visible = show_group;
			if (m_progress>=PROGRESS_MAX)
			{
				group->is_visible = show_group;
			}
		}
		group_item_id_t* p_group_items=group->ids;
		CWnd* ctrl=NULL;
		int this_group_height=0;
		// iterate all controls in group
		for (int i=0;i<items_count;i++)
		{
			CRect rect;
			ctrl = m_hDialog->GetDlgItem(p_group_items[i].id);
			if (!ctrl) continue;
			// do operation with this control
			switch (operation)
			{
			case OPER_READ:
				{
					// store designed rectangle
					ctrl->GetWindowRect(p_group_items[i].rect);
					m_hDialog->ScreenToClient(p_group_items[i].rect);
				}
				break;
			case OPER_SET:
				ctrl = m_hDialog->GetDlgItem(p_group_items[i].id);
				if (!ctrl) break;
				// copy original rect position
				memcpy(&rect,p_group_items[i].rect,sizeof(rect));
				bool hide=false;
				if (group->is_disabled) hide=true;
				switch (i)
				{
				case ITEM_ID_BUTTON:
					break;
				case ITEM_ID_GROUP:
					this_group_height = rect.Height();
				default:
					hide=!group->is_visible;
				}
				if (hide)
					ctrl->ShowWindow(SW_HIDE);
				else
					ctrl->ShowWindow(SW_SHOW);
				
				// place
				rect.top += current_group_y_shift;
				rect.bottom += current_group_y_shift;
				ctrl->MoveWindow(rect,true);
				break;
			}
		}
		// adjust shift for next groups
		#define ALPHA_MAX 1000
		#define CalcPhase(x) (((x)*(ALPHA_MAX-alpha))/ALPHA_MAX)
		{
			int alpha1=group->is_visible?ALPHA_MAX:0;
			int alpha2=group->will_be_visible?ALPHA_MAX:0;
			int alpha=((alpha2)*m_progress+(alpha1)*(PROGRESS_MAX-m_progress))/PROGRESS_MAX;
			if (group->is_visible ^ group->will_be_visible)
			{
				phase_done = false;
				m_finished=false;
			}
			current_group_y_shift -= CalcPhase(this_group_height-GROUP_HEIGHT_MINIMIZED);
			if (group->is_disabled) current_group_y_shift -= CalcPhase(GROUP_HEIGHT_DISABLED);
		}
	}
	// final
	switch (operation)
	{
	case OPER_READ:
		{
			// resize dialog
			CRect rect;
			m_hDialog->GetWindowRect(rect);
			m_original_dialog_height = rect.bottom-rect.top;
		}
		break;
	case OPER_SET:
		{
			// resize dialog
			CRect rect;
			m_hDialog->GetWindowRect(rect);
			//m_hDialog->ScreenToClient(rect);
			rect.bottom = rect.top+m_original_dialog_height+current_group_y_shift-5;
			m_hDialog->MoveWindow(rect);
			if (phase_done)
			{
				m_hDialog->Invalidate();
				m_finished=true;
			}
		}
		break;
	}
}

CExpandDialog::CExpandDialog(CWnd *pDialog)
{
	m_hDialog=pDialog;
	m_original_dialog_height=0;
	// initialization
	m_progress=PROGRESS_MAX;
	m_finished=true;
	// get designed position of all group controls
	DoGroupOperation(OPER_READ,ID_GROUP_GAMMA,false);
	for (int gid=0;gid<TOTAL_GROUPS;gid++)
	{
		DoGroupOperation(OPER_SET,(id_group_e)gid,false);
	}
}

bool CExpandDialog::IsExpandFinished()
{
	return m_finished;
}

static bool DoWindowsEvents()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}
		if (!AfxGetApp()->PreTranslateMessage(&msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	return true;
}

void CExpandDialog::ToggleGroup(id_group_e id)
{
	m_progress=0;
	while (m_progress<PROGRESS_MAX)
	{
		// go progress
		m_progress += PROGRESS_STEP;
		if (m_progress>=PROGRESS_MAX)
			m_progress=PROGRESS_MAX;
		SetGroup(id,!groups_lists[id].is_visible);
		// wait
		DoWindowsEvents();
		Sleep(PROGRESS_STEP_TIME_MS);
	}
};

void CExpandDialog::SetGroup(id_group_e id_group,bool show_group)
{
	DoGroupOperation(OPER_SET,id_group,show_group);
}

void CExpandDialog::DisableGroup(id_group_e id_group,bool disable)
{
	groups_lists[id_group].is_disabled = disable;
	// refresh
	SetGroup(id_group,groups_lists[id_group].is_visible>0);
}
