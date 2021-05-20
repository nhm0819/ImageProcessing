
// ImageProcessDoc.h: CImageProcessDoc 클래스의 인터페이스
//


#pragma once


class CImageProcessDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CImageProcessDoc() noexcept;
	DECLARE_DYNCREATE(CImageProcessDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CImageProcessDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	RGBQUAD palRGB[256];
	unsigned char*m_InputImage;
	int m_width;
	int m_height;
	int m_size;
	unsigned char*m_OutputImage;
	int m_Re_width;
	int m_Re_height;
	int m_Re_size;
	void OnDownSampling();
	void OnUpSampling();
	void OnQuantization();
	afx_msg void OnMulConstant();
	afx_msg void OnSubConstant();
	afx_msg void OnSumConstant();
	afx_msg void OnDivConstant();
	afx_msg void OnAndOperate();
	afx_msg void OnOrOperate();
	afx_msg void OnXorOperate();
	afx_msg void OnBinarization();
	afx_msg void OnNegaTransform();
	afx_msg void OnGammaCorrection();
	afx_msg void OnStressTransform();
	afx_msg void OnHistoStretch();
	afx_msg void OnEndInSearch();
	afx_msg void OnHistogram();
	afx_msg void OnHistoEqual();
	afx_msg void OnHistoSpec();
	afx_msg void OnEmbossing();
	double** OnMaskProcess(unsigned char *Target, double Mask[3][3]);
	double** OnScale(double **Target, int height, int width);
	double** Image2DMem(int height, int width);
	afx_msg void OnBlurr();
	double** m_tempImage;
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
	double DoubleABS(double X);
	afx_msg void OnNearest();
	afx_msg void OnBilinear();
	void OnBubbleSort(double *A, int MAX);
	void OnSwap(double *a, double *b);
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
	struct Complex {
		double Re; // 실수를 위한 변수
		double Im; // 허수를 위한 변수
	};
	void OnFft1d(Complex * X, int N, int Log2N);
	void OnShuffle(Complex * X, int N, int Log2N);
	void OnButterfly(Complex * X, int N, int Log2N, int mode);
	int OnReverseBitOrder(int index, int Log2N);
	Complex **m_FFT;
	void OnIfft1d(Complex * X, int N, int Log2N);
	Complex **m_IFFT;
};
