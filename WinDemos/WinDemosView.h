
// WinDemosView.h : CWinDemosView 类的接口
//

#pragma once


/*
1.新建对话框 IDD_DIALOG1，属性更改 Style 为“Child”，Border 为“None”
2.把所有可见的 CView 改为 CFormView
3.更改你的View类的构造函数：
	CYourView :: CYourView : CFormView( IDD_DIALOG1 )
	{
	}
*/
class CWinDemosView : public CFormView
{
protected: // 仅从序列化创建
	CWinDemosView();
	DECLARE_DYNCREATE(CWinDemosView)

// 特性
public:
	CWinDemosDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CWinDemosView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // WinDemosView.cpp 中的调试版本
inline CWinDemosDoc* CWinDemosView::GetDocument() const
   { return reinterpret_cast<CWinDemosDoc*>(m_pDocument); }
#endif

