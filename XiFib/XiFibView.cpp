
// XiFibView.cpp : CXiFibView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CXiFibView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CXiFibView ����/����

CXiFibView::CXiFibView()
{
	// TODO: �ڴ˴���ӹ������

}

CXiFibView::~CXiFibView()
{
}

BOOL CXiFibView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CXiFibView ����

void CXiFibView::OnDraw(CDC* /*pDC*/)
{
	CXiFibDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CXiFibView ��ӡ


void CXiFibView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CXiFibView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CXiFibView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CXiFibView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CXiFibView ���

#ifdef _DEBUG
void CXiFibView::AssertValid() const
{
	CView::AssertValid();
}

void CXiFibView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CXiFibDoc* CXiFibView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXiFibDoc)));
	return (CXiFibDoc*)m_pDocument;
}
#endif //_DEBUG


// CXiFibView ��Ϣ�������


BOOL CXiFibView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//����brushΪϣ���ı�����ɫ
	CBrush backBrush(RGB(70,70,70));
	//����ɵ�brush
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);
	//����Ҫ������
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;
		//CView::OnEraseBkgnd(pDC);
}

