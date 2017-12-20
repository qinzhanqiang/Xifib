
// XiFibView.cpp : CXiFibView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "XiFib.h"
#endif

#include "XiFibDoc.h"
#include "XiFibView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXiFibView

IMPLEMENT_DYNCREATE(CXiFibView, CView)

BEGIN_MESSAGE_MAP(CXiFibView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CXiFibView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CXiFibView 构造/析构

CXiFibView::CXiFibView()
{
	// TODO: 在此处添加构造代码

}

CXiFibView::~CXiFibView()
{
}

BOOL CXiFibView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CXiFibView 绘制

void CXiFibView::OnDraw(CDC* /*pDC*/)
{
	CXiFibDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CXiFibView 打印


void CXiFibView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CXiFibView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CXiFibView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CXiFibView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CXiFibView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CXiFibView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CXiFibView 诊断

#ifdef _DEBUG
void CXiFibView::AssertValid() const
{
	CView::AssertValid();
}

void CXiFibView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CXiFibDoc* CXiFibView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXiFibDoc)));
	return (CXiFibDoc*)m_pDocument;
}
#endif //_DEBUG


// CXiFibView 消息处理程序


BOOL CXiFibView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//设置brush为希望的背景颜色
	CBrush backBrush(RGB(70,70,70));
	//保存旧的brush
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);
	//画需要的区域
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;
		//CView::OnEraseBkgnd(pDC);
}

