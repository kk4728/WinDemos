
// WinDemosView.cpp : CWinDemosView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "WinDemos.h"
#endif

#include "WinDemosDoc.h"
#include "WinDemosView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinDemosView

IMPLEMENT_DYNCREATE(CWinDemosView, CView)

BEGIN_MESSAGE_MAP(CWinDemosView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CWinDemosView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CWinDemosView ����/����

CWinDemosView::CWinDemosView()
{
	// TODO: �ڴ˴���ӹ������

}

CWinDemosView::~CWinDemosView()
{
}

BOOL CWinDemosView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CWinDemosView ����

void CWinDemosView::OnDraw(CDC* /*pDC*/)
{
	CWinDemosDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CWinDemosView ��ӡ


void CWinDemosView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CWinDemosView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CWinDemosView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CWinDemosView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CWinDemosView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CWinDemosView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWinDemosView ���

#ifdef _DEBUG
void CWinDemosView::AssertValid() const
{
	CView::AssertValid();
}

void CWinDemosView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinDemosDoc* CWinDemosView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinDemosDoc)));
	return (CWinDemosDoc*)m_pDocument;
}
#endif //_DEBUG


// CWinDemosView ��Ϣ�������
