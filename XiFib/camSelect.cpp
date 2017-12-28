// camSelect.cpp : implementation file
//文件--启动 之后选择相机

#include "stdafx.h"
#include "camSelect.h"


// camSelect dialog

IMPLEMENT_DYNAMIC(camSelect, CDialog)

camSelect::camSelect(CWnd* pParent /*=NULL*/)
	: CDialog(camSelect::IDD, pParent)
{

}

camSelect::~camSelect()
{
}

void camSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(camSelect, CDialog)
	ON_BN_CLICKED(BUTTON_OPEN, &camSelect::OnBnClickedOpen)
END_MESSAGE_MAP()



BOOL camSelect::OnInitDialog()
{
	int dwSerial = 0;
	// disable close option in dialog menu
	CMenu* pSM = GetSystemMenu(FALSE);
	if(pSM)
		pSM->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);

	// fill device selection combo
	DWORD numDevices=0;
	xiGetNumberDevices(&numDevices);
	CComboBox* pSelList=(CComboBox*)GetDlgItem(COMBO_SELECT);
	for(int i=0; i < (int)numDevices; i++)
	{
		char devEntry[MAX_PATH]="";
		char buff[MAX_PATH]="";
		
		xiGetDeviceInfoString(i,XI_PRM_DEVICE_NAME,buff,MAX_PATH);
		sprintf(devEntry, "%d : %s, ", i+1, buff);
		xiGetDeviceInfoString(i,XI_PRM_DEVICE_SN,buff,MAX_PATH);
		strcat(devEntry, buff);

		pSelList->AddString(devEntry);
	}
	
	pSelList->SetCurSel(0);
	return TRUE; 
}

void camSelect::OnBnClickedOpen()
{
	CComboBox* pSelList=(CComboBox*)GetDlgItem(COMBO_SELECT);
	*selected_index = pSelList->GetCurSel();
	camSelect::EndDialog(0);
}

void camSelect::GetIndex( PDWORD index)
{
	selected_index = index;
}