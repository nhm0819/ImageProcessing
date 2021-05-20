
// ImageProcessView.h: CImageProcessView 클래스의 인터페이스
//

#pragma once


class CImageProcessView : public CView
{
protected: // serialization에서만 만들어집니다.
	CImageProcessView() noexcept;
	DECLARE_DYNCREATE(CImageProcessView)

// 특성입니다.
public:
	CImageProcessDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	BITMAPINFO* BmInfo;
	virtual ~CImageProcessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
public:
	afx_msg void OnDownSampling();
	afx_msg void OnUpSampling();
	afx_msg void OnQuantization();
	afx_msg void OnSumConstant();
	afx_msg void OnSubConstant();
	afx_msg void OnMulConstant();
	afx_msg void OnDivConstant();
	afx_msg void OnAndOperate();
	afx_msg void OnOrOperate();
	afx_msg void OnXorOperate();
	afx_msg void OnNegaTransform();
	afx_msg void OnGammaCorrection();
	afx_msg void OnBinarization();
	afx_msg void OnStressTransform();
	afx_msg void OnHistoStretch();
	afx_msg void OnEndInSearch();
	afx_msg void OnHistogram();
	afx_msg void OnHistoEqual();
	afx_msg void OnHistoSpec();
	afx_msg void OnEmbossing();
	afx_msg void OnBlurr();
	afx_msg void OnGaussianFilter();
	afx_msg void OnSharpening();
	afx_msg void OnHpfSharpening();
	afx_msg void OnLpfSharpening();
	afx_msg void OnLaplacian();
	afx_msg void OnFrameComb();
	afx_msg void OnFrameAnd();
	afx_msg void OnFrameOr();
	afx_msg void OnTranslation();
	afx_msg void OnMirrorHor();
	afx_msg void OnMirrorVer();
	afx_msg void OnRotation();
	afx_msg void OnDiffOperatorHor();
	afx_msg void OnHomogenOperator();
	afx_msg void OnNearest();
	afx_msg void OnBilinear();
	afx_msg void OnMedianSub();
	afx_msg void OnMeanSub();
	afx_msg void OnFrameSum();
	afx_msg void OnFrameSubtraction();
	afx_msg void OnFrameMultiply();
	afx_msg void OnFrameDivision();
	afx_msg void OnBinaryErosion();
	afx_msg void OnBinaryDilation();
	afx_msg void OnGrayErosion();
	afx_msg void OnGrayDilation();
	afx_msg void OnLowPassFilter();
	afx_msg void OnHighPassFilter();
	afx_msg void OnMeanFilter();
	afx_msg void OnMedianFilter();
	afx_msg void OnMinFilter();
	afx_msg void OnMaxFilter();
	afx_msg void OnFft2d();
	afx_msg void OnIfft2d();
	afx_msg void OnLpfFrequency();
	afx_msg void OnHpfFrequency();
	
};

#ifndef _DEBUG  // ImageProcessView.cpp의 디버그 버전
inline CImageProcessDoc* CImageProcessView::GetDocument() const
   { return reinterpret_cast<CImageProcessDoc*>(m_pDocument); }
#endif

