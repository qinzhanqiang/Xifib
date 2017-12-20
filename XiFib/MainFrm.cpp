
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "XiFib.h"
#include "AdjustDialog.h"
#include "MainFrm.h"
#include "acquisition.h"
#include "afxwin.h"


extern LPSETUP lps;
extern bool flag_start;
//�ٽ��������߳�ͬ��
extern CRITICAL_SECTION C_lock;  

OVERLAPPED Eol = {0};
OVERLAPPED Wol = {0};	//д�����ṹ����
OVERLAPPED Rol = {0};//�������ṹ����



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_32772, &CMainFrame::OnAdjust)
	ON_COMMAND(ID_DISTANCE, &CMainFrame::OnDistance)
	ON_COMMAND(ID_PDIS, &CMainFrame::OnPdis)
	ON_COMMAND(ID_START, &CMainFrame::OnStart)
	ON_UPDATE_COMMAND_UI(ID_START, &CMainFrame::OnUpdateStart)
	ON_COMMAND(ID_STOP, &CMainFrame::OnStop)
	ON_UPDATE_COMMAND_UI(ID_STOP, &CMainFrame::OnUpdateStop)
	ON_WM_CLOSE()
	ON_COMMAND(ID_SAFE_BMP, &CMainFrame::OnSafeBmp)
	ON_COMMAND(ID_COLORV, &CMainFrame::OnColorv)
	ON_UPDATE_COMMAND_UI(ID_COLORV, &CMainFrame::OnUpdateColorv)
	ON_COMMAND(ID_FOMAT, &CMainFrame::OnFomat)
	ON_COMMAND(ID_EXPOSE, &CMainFrame::OnExpose)
	ON_COMMAND(ID_GAIN, &CMainFrame::OnGain)
	ON_COMMAND(ID_AUTO, &CMainFrame::OnAuto)
	ON_COMMAND(ID_PARA, &CMainFrame::OnPara)
	ON_COMMAND(ID_U, &CMainFrame::OnU)
	ON_COMMAND(ID_T, &CMainFrame::OnT)
	ON_COMMAND(ID_P, &CMainFrame::OnP)
	ON_COMMAND(ID_SCI, &CMainFrame::OnSci)
	ON_COMMAND(ID_OPEN_COM, &CMainFrame::OnOpenCom)
	ON_UPDATE_COMMAND_UI(ID_OPEN_COM, &CMainFrame::OnUpdateOpenCom)
	ON_COMMAND(ID_CLOSE_COM, &CMainFrame::OnCloseCom)
	ON_UPDATE_COMMAND_UI(ID_CLOSE_COM, &CMainFrame::OnUpdateCloseCom)
	ON_COMMAND(ID_COLLIMAT, &CMainFrame::OnCollimat)
	ON_UPDATE_COMMAND_UI(ID_SCI, &CMainFrame::OnUpdateSci)
	ON_MESSAGE(WM_MYMSG,OnReceiveEvent)//�Զ�����Ϣӳ��
	ON_COMMAND(ID_STOP_COLLIMAT, &CMainFrame::OnStopCollimat)
	ON_UPDATE_COMMAND_UI(ID_STOP_COLLIMAT, &CMainFrame::OnUpdateStopCollimat)
	ON_UPDATE_COMMAND_UI(ID_COLLIMAT, &CMainFrame::OnUpdateCollimat)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
	:m_Distance(5)
	, m_P(0)
	,max(0)
	,Epoint(0,0)
	//,m_Ellipse(m_wndEllipse.Epse)
	//,m_GaussLine(m_wndGauss.gaussLine)
	//,m_LGaussLine(m_wndGaussL.gaussLine)
	,bcolor (FALSE)
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
	TZDlg = NULL;
	CCDlg = NULL;
	CPDlg = NULL;
	//SDlg = NULL;
	ColDlg = NULL;
	pDataList = &m_wndData.dataList;
}

CMainFrame::~CMainFrame()
{
	if (TZDlg!=NULL)
	{
		delete TZDlg;
		TZDlg = NULL;
	}
	if (CCDlg!=NULL)
	{
		delete CCDlg;
		CCDlg = NULL;
	}
	if (CPDlg!=NULL)
	{
		delete CPDlg;
		CPDlg = NULL;
	}
	/*if (SDlg!=NULL)
	{
		delete SDlg;
		SDlg = NULL;
	}*/
	if (ColDlg!=NULL)
	{
		delete ColDlg;
		ColDlg = NULL;
	}
	
	if(lps->bRun)
	{
		//ֹͣ
		if (theApp.wDlg->hMV != INVALID_HANDLE_VALUE)
		{
			if(lps->bRun){
				shStopLive(theApp.wDlg->hMV);
				//SetDlgItemText(IDC_STARTSTOP, "Start");
				lps->bHaveData = FALSE;
			}
		}
	}
	/*if (lps->bRun == TRUE ) {
		lps->bRun = FALSE;
		Sleep(100);
	}*/
	//if(lps->childv->sdlg)
	//{
	//	delete lps->childv->sdlg;
	//	lps->childv->sdlg = NULL;
	//}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // ����������ʽ...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // ����Ϊ FALSE �Ὣ�رհ�ť������ѡ�������Ҳ�
	mdiTabParams.m_bTabIcons = FALSE;    // ����Ϊ TRUE ���� MDI ѡ��������ĵ�ͼ��
	mdiTabParams.m_bAutoColor = TRUE;    // ����Ϊ FALSE ������ MDI ѡ����Զ���ɫ
	mdiTabParams.m_bDocumentMenu = TRUE; // ��ѡ�������ұ�Ե�����ĵ��˵�
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("δ�ܴ����˵���\n");
		return -1;      // δ�ܴ���
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// ��ֹ�˵����ڼ���ʱ��ý���
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: �������ϣ���������Ͳ˵�����ͣ������ɾ��������
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���ز˵���ͼ��(�����κα�׼��������):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}

	//������
/******************************************************************************************/

	m_wndEllipse.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndEllipse);
	m_wndGauss.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndGauss);
	m_wndGaussL.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndGaussL);
	m_wndPlay.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndPlay);
	m_wndData.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndData);
/******************************************************************************************/

	// ������ǿ�Ĵ��ڹ���Ի���
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// ���ù�������ͣ�����ڲ˵��滻
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ���ÿ���(��ס Alt �϶�)�������Զ���
	CMFCToolBar::EnableQuickCustomization();

	// ���ò˵����Ի�(���ʹ�õ�����)
	// TODO: �������Լ��Ļ������ȷ��ÿ�������˵�������һ���������
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);

	/*lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	*/
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	//CMFCToolBar::SetBasicCommands(lstBasicCommands);

	// ���ĵ�����Ӧ�ó��������ڴ��ڱ������ϵ�˳����н�������
	// ���Ľ��������Ŀ����ԣ���Ϊ��ʾ���ĵ�����������ͼ��
	ModifyStyle(0, FWS_PREFIXTITLE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	//�������
/******************************************************************************************/
	if (!m_wndEllipse.Create(_T("Ellipse"), this, CRect(0, 0, 100, 100), TRUE, 1004, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE; // δ�ܴ���
	}
	if (!m_wndGauss.Create(_T("Gauss"), this, CRect(0, 0, 100, 100), TRUE, 1005, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE; // δ�ܴ���
	}
	if (!m_wndGaussL.Create(_T("GaussL"), this, CRect(0, 0, 100, 100), TRUE, 1006, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE; // δ�ܴ���
	}
	if (!m_wndPlay.Create(_T("XiPlay"), this, CRect(0, 0, 100, 100), TRUE, 1007, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE; // δ�ܴ���
	}
	if (!m_wndData.Create(_T("DataPane"), this, CRect(0, 0, 100, 100), TRUE, 1008, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE; // δ�ܴ���
	}
	/******************************************************************************************/

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{

	UpdateMDITabbedBarsIcons();
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
}

void CMainFrame::OnAdjust()
{
	// TODO: �ڴ���������������
	// TODO: �ڴ���������������

	if(TZDlg==NULL)
	{
		TZDlg = new CAdjustDialog(TZDlg);
		TZDlg->Create(IDD_TZ_DIALOG,this); 
	}
	 
	TZDlg->ShowWindow(SW_SHOW);
	return;   
}


void CMainFrame::OnDistance()
{
	// TODO: �ڴ���������������
	if(CCDlg==NULL)
	{
		CCDlg = new CChangeDDiolag(CCDlg);
		CCDlg->Create(IDD_CD_DIALOG,this); 
	}

	CCDlg->ShowWindow(SW_SHOW);
	return;   
}


void CMainFrame::OnPdis()
{
	// TODO: �ڴ���������������
	if(CPDlg==NULL)
	{
		CPDlg = new CChangePDialog(CPDlg);
		CPDlg->Create(IDD_CP_DIALOG,this); 
	}

	CPDlg->ShowWindow(SW_SHOW);
	return;   
}


void CMainFrame::OnStart()
{
	// TODO: �ڴ���������������
	
	if(!lps->bRun) 
	{
		CString str("���δ����");
		MessageBox(str, _T("����"), MB_OK | MB_ICONWARNING); 
		return;
	}
	
	if (!flag_start)
	{
		flag_start = true;
	}

	//����mat����
	//rows �� ���� �൱�� height ��Ӧ y
	//cols �� ���� �൱�� width ��Ӧ x
	//cv::Mat src(lps->imgs.height, lps->imgs.width, CV_8UC1);

	//��������ֵ
	/*int valofc = 0;
	double maxval=0;
	double minval=0;
	cv::Point maxpoint(0,0);
	double LhalfW=0;
	double halfW=0;
	double dev=0;
	double Ldev=0;

	CEllipseFit LSellipse;*/
	//CEllipse tem_Ellipse;
	//CGaussLine tem_GaussLine;
	//CGaussLine tem_LGaussLine;
}

void CMainFrame::OnUpdateStart(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(!flag_start)
	 pCmdUI->Enable(TRUE); //���������˵���
	else
		pCmdUI->Enable(FALSE); 
}


void CMainFrame::OnStop()
{
	// TODO: �ڴ���������������
	if (flag_start)
	{
		flag_start = false;
	}
}


void CMainFrame::OnUpdateStop(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(flag_start)
		pCmdUI->Enable(TRUE); 
	else
		pCmdUI->Enable(FALSE); //����ֹͣ�˵���
}


void CMainFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	flag_start = false;
	CMDIFrameWndEx::OnClose();
}

int CMainFrame::findHalf(cv::Mat& in, const int & maxval, std::vector<int> &Xp, std::vector<int> &Yp)  
{   
	if(in.channels()>1)
		return -1;

	int k=0;
	//cv::Point halfpoint;
	double halfmax=0.5*maxval;

	int rows = in.rows;  
	int cols = in.cols;

	Xp.clear();
	Yp.clear();

	switch (in.elemSize())          //�ж�ͼƬ��8λ����16λ
	{
	case 1:

		if(maxval<50)
			return -1;

		for( int i = 0;i < rows;i++)     //iΪ�����꣬jΪ������
		{  
			unsigned char* datain = in.ptr<unsigned char>(i);   //��ȡĳһ�е�ָ��
			for(int j = 0;j < cols;j++)  
			{  
				if(datain[j]<(halfmax+2)&&datain[j]>(halfmax-2))
				{
					//((CSeries)m_Chart.Series(0)).AddXY(rows-j, i, NULL, RGB(255,0,255)); 
					k++;
					Xp.push_back(j);
					Yp.push_back(i);
				}
			}  
		}
		//((CCanvas)(m_Chart.get_Canvas())).EllipseWithZ(100, 100, 200, 200, 100);
		break;

	case 2:

		if(maxval<25000)
			return -1;

		for( int i = 0;i < rows;i++)  
		{  
			unsigned short* datain = in.ptr<unsigned short>(i);   
			for(int j = 0;j < cols;j++)  
			{  
				if(datain[j]<(halfmax+10)&&datain[j]>(halfmax-10))
				{
					//((CSeries)m_Chart.Series(0)).AddXY(rows-i, j, NULL, RGB(255,0,255)); 
					k++;
					Xp.push_back(j);
					Yp.push_back(i);
				}
			}  
		}
		break;

	default:
		return -1;
	}
	return k;
}

int CMainFrame::linePoint(cv::Mat &src, cv::Point p, double ang, std::vector<double> &vx, std::vector<double> &value)
{
	int i=0;
	cv::Point linepoint[2];
	double xielv = std::tan(ang);

	double x1 = p.x-p.y/xielv;
	if( x1>0 && x1<src.cols )
	{
		linepoint[i].x = x1;
		linepoint[i].y = 0;
		++i;
	}
	double x2 = p.x-(p.y-src.rows)/xielv;
	if( x2>0 && x2<src.cols )
	{
		linepoint[i].x = x2;
		linepoint[i].y = src.rows;
		++i;
	}

	double y1 = p.y-p.x*xielv;
	if( i<2 && y1>0 && y1<src.rows )
	{
		linepoint[i].x = 0;
		linepoint[i].y = y1;
		++i;
	}

	double y2 = p.y-(p.x-src.cols)*xielv;
	if( i<2 && y2>0 && y2<src.rows )
	{
		linepoint[i].x = src.cols;
		linepoint[i].y = y2;
		//++i;
	}

	vx.clear();
	value.clear();

	cv::LineIterator it(src, linepoint[0], linepoint[1], 8, true);
	double lx =0;
	double val =0;
	int x = 0;
	int y = 0;
	switch (src.elemSize())
	{
	case 1:
		for(int j = 0; j < it.count; j++, ++it)
		{
			val = src.at<unsigned char>(it.pos());
			if(val>2)
			{
				x = it.pos().x-p.x;
				y = it.pos().y-p.y;
				if (x < 0)
					lx = -std::sqrt((double)(x*x + y*y));
				else
					lx = std::sqrt((double)(x*x + y*y));
				vx.push_back(lx);
				value.push_back(val);
			}
		}
		break;

	case 2:
		for(int j = 0; j < it.count; j++, ++it)
		{
			val = src.at<unsigned short>(it.pos());
			if (val>2)
			{
				x = it.pos().x-p.x;
				y = it.pos().y-p.y;
				if (x < 0)
					lx = -std::sqrt((double)(x*x + y*y));
				else
					lx = std::sqrt((double)(x*x + y*y));
				vx.push_back(lx);
				value.push_back(val);
			}
		}
		break;

	default:
		return -1;
		//cv::line(src, linepoint[0], linepoint[1], cv::Scalar(0), 1, 8, 0 );���������� 
		auto k = it.count;
		return k;
	}
}

double CMainFrame::round(double r)
{
	return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}

// �����ع�ʱ��
int CMainFrame::cExpose()
{
	int maxExpose = 20000;//����ع�ʱ��
	int minExpose = 500;//��С�ع�ʱ��
	int num  = 0;
	XI_RETURN xret;
	cv::Mat src(lps->imgs.height, lps->imgs.width, CV_8UC1);
	double maxval, minval;
	cv::Point maxpoint;
	while(1)
	{
		getMat(src);
		cv::minMaxLoc(src, &minval, &maxval, NULL, &maxpoint);    //��ȡ�����Сֵ��������

		if (lps->bpp>2)
		{
			return -1;
		}

		if (lps->bpp = 2)
		{
			maxval = maxval/255;
		}

		if (maxval<250&&maxval>160)
		{
			m_wndPlay.sdlg->UpdateControls();
			return 0;
		}
		else
		{
			if (maxval<20)
			{
				if (lps->uiExposure==maxExpose)
				{
					m_wndPlay.sdlg->UpdateControls();
					return -2;
				}
			}
			else if(maxval=255)
			{
				if (lps->uiExposure == minExpose)
				{
					m_wndPlay.sdlg->UpdateControls();
					return -3;
				}
			}
			else
			{
				if (maxExpose>minExpose)
				{
					if (maxval > 250)
					{
						maxExpose = (maxExpose+minExpose)/2;
						lps->uiExposure = maxExpose;
						xret = xiSetParamInt(lps->childv->hMV,"XI_PRM_EXPOSURE",lps->uiExposure);
					}
					else
					{
						minExpose = (maxExpose+minExpose)/2;
						lps->uiExposure = minExpose;
						xret = xiSetParamInt(lps->childv->hMV,"XI_PRM_EXPOSURE",lps->uiExposure);
					}
				}
			}
		}
	}
	m_wndPlay.sdlg->UpdateControls();
	return -1;
}

int CMainFrame::getMat(cv::Mat &src)
{
	EnterCriticalSection(&C_lock);
	//����ͼ���С�����
	if (lps->img.width != src.cols || lps->img.height!= src.rows || lps->bpp != src.elemSize())
	{
		src.release();
		switch(lps->bpp)
		{
		case 1:
			src.create(lps->imgs.height, lps->imgs.width, CV_8UC1);
			break;
		case 2:
			src.create(lps->imgs.height, lps->imgs.width, CV_16UC1);
			break;
		default:
			src.create(lps->imgs.height, lps->imgs.width, CV_8UC1);
		}	
	}

	//�ڴ濽��
	IplImage* image = cvCreateImage(cvSize( 2048, 2048), IPL_DEPTH_8U, 1); //��XI_IMGת��ΪIplImage
	memcpy( image->imageData, lps->img.bp, (lps->img.width)*(lps->img.height)*lps->bpp);
	//cvSaveImage("D:\\saveImage3.jpg",image);
	src = cv::cvarrToMat(image);

	LeaveCriticalSection(&C_lock);
	Sleep(1);
	return lps->bpp;
}

void CMainFrame::OnSafeBmp()
{
	// TODO: �ڴ���������������

	if(!lps->bRun) 
	{
		CString str("���δ����");
		MessageBox(str, _T("����"), MB_OK | MB_ICONWARNING); 
		return;
	}

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
}


void CMainFrame::OnColorv()
{
	// TODO: �ڴ���������������
	if (bcolor == FALSE)
	{
		bcolor = TRUE;
		int		i, j;
		int nUsableColors;
		struct {
			WORD Version;
			WORD NumberOfEntries;
			PALETTEENTRY aEntries[COLORS];
		} Palette = { 0x300, COLORS };

		//*** Fill in the entries from the given color table
		nUsableColors = COLORS;
		for (j=0,i=0; i<nUsableColors; i++,j++) {
			//Palette.aEntries[i].peRed   = (BYTE)((COLORS*(255-j)) / nUsableColors);
			//Palette.aEntries[i].peGreen = 0;//(BYTE)((COLORS*j) / nUsableColors);
			//Palette.aEntries[i].peBlue  = (BYTE)((COLORS*j) / nUsableColors);
			if (j<64)
			{
				Palette.aEntries[i].peRed   = 0;//(BYTE)((COLORS*255) / nUsableColors);
				Palette.aEntries[i].peGreen = (BYTE)((COLORS*j*4) / nUsableColors);
				Palette.aEntries[i].peBlue  = (BYTE)((COLORS*255) / nUsableColors);
			}
			else if (j<128)
			{
				Palette.aEntries[i].peRed   = 0;//(BYTE)((COLORS*(j-85)*3) / nUsableColors);
				Palette.aEntries[i].peGreen = (BYTE)((COLORS*255) / nUsableColors);
				Palette.aEntries[i].peBlue  = (BYTE)((COLORS*(255-4*(j-64))) / nUsableColors);
			}
			else if (j<192)
			{
				Palette.aEntries[i].peRed   = (BYTE)((COLORS*(j-128)*4) / nUsableColors);
				Palette.aEntries[i].peGreen = (BYTE)((COLORS*255) / nUsableColors);
				Palette.aEntries[i].peBlue  = 0;//(BYTE)((COLORS*(255-3*(j-85))) / nUsableColors);
			}
			else
			{
				Palette.aEntries[i].peRed   = (BYTE)((COLORS*255) / nUsableColors);
				Palette.aEntries[i].peGreen = (BYTE)((COLORS*(255-4*(j-192))) / nUsableColors);
				Palette.aEntries[i].peBlue  = 0; //(BYTE)((COLORS*j) / nUsableColors);
			}
			
			Palette.aEntries[i].peFlags = PC_NOCOLLAPSE | PC_RESERVED;
		}
		DeleteObject(lps->hPalGray);
		lps->hPalGray = CreatePalette ((LOGPALETTE *)&Palette);
	}
	else
	{
		bcolor = FALSE;
		int		i, j;
		int nUsableColors;
		struct {
			WORD Version;
			WORD NumberOfEntries;
			PALETTEENTRY aEntries[COLORS];
		} Palette = { 0x300, COLORS };

		//*** Fill in the entries from the given color table
		nUsableColors = COLORS;
		for (j=0,i=0; i<nUsableColors; i++,j++) {
			Palette.aEntries[i].peRed   = (BYTE)((COLORS*j) / nUsableColors);
			Palette.aEntries[i].peGreen = (BYTE)((COLORS*j) / nUsableColors);
			Palette.aEntries[i].peBlue  = (BYTE)((COLORS*j) / nUsableColors);
			Palette.aEntries[i].peFlags = PC_NOCOLLAPSE | PC_RESERVED;
		}
		DeleteObject(lps->hPalGray);
		lps->hPalGray = CreatePalette ((LOGPALETTE *)&Palette);
	}
}


void CMainFrame::OnUpdateColorv(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if (bcolor == TRUE)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

//׼ֱ����
void CMainFrame::OnCollimat()
{
	// TODO: �ڴ���������������
	if(ColDlg==NULL)
	{
		ColDlg = new CCollimatDialog(ColDlg);
		ColDlg->Create(IDD_COLLIMAT,this); 
	}

	ColDlg->ShowWindow(SW_SHOW);
	return;  

}


void CMainFrame::OnUpdateCollimat(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if (!m_cRun)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}



void CMainFrame::OnStopCollimat()
{
	// TODO: �ڴ���������������
	m_cRun = FALSE;
	AfxMessageBox(_T("�ر�׼ֱ�ɹ���"));
}


void CMainFrame::OnUpdateStopCollimat(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if (m_cRun)
	{
		pCmdUI->Enable(true);
	}
	else
	{

		pCmdUI->Enable(false);
	}
}




//����ͨ�Ų�������
void CMainFrame::OnSci()
{
	// TODO: �ڴ���������������
	CString strStatus;
	CSCIDialog mysetup;
	if(mysetup.DoModal() == IDOK)
	{
		switch(mysetup.m_Com)
		{
		case 0:
			{
				strStatus = "COM1";
				myCom = "COM1";
				break;
			}
		case 1:
			{
				strStatus = "COM2";
				myCom = "COM2";
				break;
			}
		case 2:
			{
				strStatus = "COM3";
				myCom = "COM3";
				break;
			}
		case 3:
			{
				strStatus = "COM4";
				myCom = "COM4";
				break;
			}
		}
		strStatus+=",";
		switch(mysetup.m_BaudRate)
		{
		case 0:
			{
				strStatus +="19200";
				myBaudRate = CBR_19200;
				break;
			}
		case 1:
			{
				strStatus +="9600";
				myBaudRate = CBR_9600;
				break;
			}
		case 2:
			{
				strStatus +="4800";
				myBaudRate = CBR_4800;
				break;
			}
		case 3:
			{
				strStatus +="2400";
				myBaudRate = CBR_2400;
				break;
			}
		}
		strStatus+=",";
		switch(mysetup.m_Parity)
		{
		case 0:
			{
				strStatus +="N";
				myfParity = false;
				myParity = NOPARITY;
				break;
			}
		case 1:
			{
				strStatus +="O";
				myfParity = true;
				myParity = ODDPARITY;
				break;
			}
		case 2:
			{
				strStatus +="E";
				myfParity = true;
				myParity = EVENPARITY;
				break;
			}

		}
		strStatus+=",8,1";
		AfxMessageBox(strStatus);
	}

}



void CMainFrame::OnUpdateSci(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	/*if (!blnOpened)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}*/
}


//�򿪴���
void CMainFrame::OnOpenCom()
{
	// TODO: �ڴ���������������
	hCom = CreateFile(myCom,	//��������
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
		NULL);
	if(hCom != INVALID_HANDLE_VALUE)
	{
		SetupComm(hCom,1024,512);
		DCB myDCB;
		GetCommState(hCom,&myDCB);

		myDCB.BaudRate = myBaudRate;
		myDCB.fBinary = true;
		myDCB.fParity = myfParity;
		myDCB.Parity = myParity;
		myDCB.ByteSize = 8;
		myDCB.StopBits = ONESTOPBIT;

		SetCommState(hCom,&myDCB);

		MessageBox("���ڴ򿪳ɹ�");

		bIsOpened = true;
		blnOpened = true;
	}
	else
	{
		MessageBox("���ڴ�ʧ��");
		bIsOpened = false;
		blnOpened = false;
	}

	hWnd = GetSafeHwnd();


	if(!SetCommMask(hCom,EV_RXCHAR|EV_TXEMPTY)){
		//MessageBox("�����¼�����ʧ�ܡ���");
	}
	DWORD dwParam;
	hThreadEvent = CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadProcEvent,
		&dwParam,
		0,
		&dwThreadID);
	if(hThreadEvent == INVALID_HANDLE_VALUE)
	{
		MessageBox("�¼��̴߳���ʧ�ܡ���");
	}
	fEventRun = true;
	fStopMsg = false;
}


void CMainFrame::OnUpdateOpenCom(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if (!blnOpened)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

//�رմ���
void CMainFrame::OnCloseCom()
{
	// TODO: �ڴ���������������
	if((bIsOpened == true)&&(hCom !=NULL))
	{
		fEventRun = false;

		CloseHandle(hThreadEvent);
		SetCommMask(hCom,0);
		EscapeCommFunction(hCom, CLRDTR ) ;
		PurgeComm( hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
		CloseHandle(hCom);	
		hCom = NULL;
		MessageBox(_T("���ڹرճɹ�"));
		bIsOpened = false;
		blnOpened = false;
	}
	else
	{
		MessageBox(_T("���ڹر�ʧ��"));
	}
}


void CMainFrame::OnUpdateCloseCom(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if (blnOpened)
	{
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}









void CMainFrame::OnFomat()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnExpose()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnGain()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnAuto()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnPara()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnU()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnT()
{
	// TODO: �ڴ���������������
}


void CMainFrame::OnP()
{
	// TODO: �ڴ���������������
}




//////////////���ڷ�����Ϣ
void CMainFrame::onSendMessage(int SData[6])
{
	DWORD dwRes;
	DWORD dwWrite;
	BOOL fRes;
	//sendData = "010500010000";
	//UpdateData(TRUE);
	//AfxMessageBox(sendData);
	//int arr[10] = {1,2,3,4,5,60,6,7,8,9};
	//myWByte[1024] = SData[1024];
	for(int i=0;i<6;++i)
	{
		myWByte[i] = SData[i];
	}
	//sendData =str;
	//UpdateData(TRUE);
	//AfxMessageBox(sendData);
	//myWByte = (BYTE*)sendData.GetBuffer(sendData.GetLength());
	//memcpy(myWByte,sendData.GetBuffer(sendData.GetLength()),sendData.GetLength()*2);  //��cstring����byte����
	//memcpy(myWByte+32,sendData.GetBuffer(sendData.GetLength())+16,32);  //��cstring����byte����
	int i = sendData.GetLength();
	int j = sizeof(myWByte);
	//dataWLen = sendData.GetLength();
	dataWLen = 6;

	Wol.hEvent = CreateEvent(NULL,
		TRUE,
		FALSE,
		NULL);		
	if(WriteFile(hCom,&myWByte,dataWLen,NULL,&Wol))
	{
		//���ͳɹ�
	}
	else
	{
		dwRes = WaitForSingleObject(Wol.hEvent,INFINITE);
		switch(dwRes)
		{
		case WAIT_OBJECT_0:
			if(!GetOverlappedResult(hCom,
				&Wol,
				&dwWrite,
				TRUE))
			{
				//success
			}
			else
			{
				//fail
			}
			break;
		case WAIT_TIMEOUT:
			break;
		default:
			break;
		}
	}
	CloseHandle(Wol.hEvent);

	PurgeComm(hCom, PURGE_TXABORT|PURGE_TXCLEAR); 
}


//////////////////////�߳�/////////////////////////////////
DWORD WINAPI ThreadProcEvent(LPVOID pParam){
	DWORD dwEvtMask,dwRes;

	Eol.hEvent = CreateEvent(NULL,
		TRUE,
		FALSE,
		NULL);
	while(fEventRun){
		WaitCommEvent(hCom,
			&dwEvtMask,
			&Eol);
		dwRes = WaitForSingleObject(Eol.hEvent,INFINITE);
		switch(dwRes)
		{
		case WAIT_OBJECT_0:
			switch(dwEvtMask)
			{
			case EV_RXCHAR:
				if(!fStopMsg)
				{
					//AfxMessageBox(_T("��⵽������Ϣ����"));
					fStopMsg = true;
					::PostMessageA(hWnd,
						WM_MYMSG,
						0,

						(LPARAM)EV_RXCHAR);
				}
				break;
			case EV_TXEMPTY:
				//AfxMessageBox(_T("��⵽������Ϣ����"));
				break;
			}
			break;
		}
	}

	return 0;
}


//////////////////////////������Ϣ
long CMainFrame::OnReceiveEvent(WPARAM wParam, LPARAM lParam)
{
	fStopMsg = true;
	//	MessageBox(_T("bbbbbbb"));
	receiveData="";


	//AfxMessageBox(_T("������ն�Ӧ����Ϣ��Ӧ��������"));
	//PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR );
	CString temp;
	//AfxMessageBox(L"���ˡ���");
	DWORD dwRes;
	DWORD dwRead;
	DWORD dwErrors;
	COMSTAT Rcs;
	ClearCommError(hCom,
		&dwErrors,
		&Rcs);
	if(ReadFile(hCom,
		&myRByte,
		Rcs.cbInQue,
		NULL,
		&Rol))
	{
		//���������ڴ˳ɹ�����
	}
	else
	{
		Rol.hEvent = CreateEvent(NULL,
			TRUE,
			FALSE,
			NULL);

		dwRes = WaitForSingleObject(Rol.hEvent,INFINITE);
		switch(dwRes)
		{
		case WAIT_OBJECT_0:
			if(!GetOverlappedResult(hCom,
				&Rol,
				&dwRead,
				TRUE))
			{
				AfxMessageBox(_T("ʹ��GetOverlappedResult����������"));
			}
			else
			{
				//AfxMessageBox(L"������ɣ����ݶ�������");
				//�ӳٴ��������ڴ˳ɹ�����
			}
			break;
		case WAIT_TIMEOUT:
			AfxMessageBox(_T("��ȡ��ʱ����"));
			break;
		default:
			AfxMessageBox(_T("�����ԭ�򡣡�"));
			break;
		}
	}

	int i;
	for(i=0;i<Rcs.cbInQue;i++)
	{
		temp.Format(_T("%c"),myRByte[i]);
		receiveData += temp;
	}
	CString S=receiveData;
	//AfxMessageBox(receiveData);
	if(myRByte[0]==10)
	{
		//AfxMessageBox(_T("���յ����������Ϣ��"));
		next_ok=true;
		myRByte[0]=0;
		Sleep(100);
	}
	//UpdateData(FALSE);
	PurgeComm(hCom,PURGE_RXABORT|PURGE_RXCLEAR);
	fStopMsg = false;
	/*if (conPicSign == true)
	{
		onContinue();
	}*/

	return 0;
}







