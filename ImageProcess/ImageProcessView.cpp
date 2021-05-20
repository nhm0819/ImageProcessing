
// ImageProcessView.cpp: CImageProcessView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageProcess.h"
#endif

#include "ImageProcessDoc.h"
#include "ImageProcessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageProcessView

IMPLEMENT_DYNCREATE(CImageProcessView, CView)

BEGIN_MESSAGE_MAP(CImageProcessView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImageProcessView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_DOWN_SAMPLING, &CImageProcessView::OnDownSampling)
	ON_COMMAND(ID_UP_SAMPLING, &CImageProcessView::OnUpSampling)
	ON_COMMAND(ID_QUANTIZATION, &CImageProcessView::OnQuantization)
	ON_COMMAND(ID_SUM_CONSTANT, &CImageProcessView::OnSumConstant)
	ON_COMMAND(ID_SUB_CONSTANT, &CImageProcessView::OnSubConstant)
	ON_COMMAND(ID_MUL_CONSTANT, &CImageProcessView::OnMulConstant)
	ON_COMMAND(ID_DIV_CONSTANT, &CImageProcessView::OnDivConstant)
	ON_COMMAND(ID_AND_OPERATE, &CImageProcessView::OnAndOperate)
	ON_COMMAND(ID_OR_OPERATE, &CImageProcessView::OnOrOperate)
	ON_COMMAND(ID_XOR_OPERATE, &CImageProcessView::OnXorOperate)
	ON_COMMAND(ID_NEGA_TRANSFORM, &CImageProcessView::OnNegaTransform)
	ON_COMMAND(ID_GAMMA_CORRECTION, &CImageProcessView::OnGammaCorrection)
	ON_COMMAND(ID_BINARIZATION, &CImageProcessView::OnBinarization)
	ON_COMMAND(ID_STRESS_TRANSFORM, &CImageProcessView::OnStressTransform)
	ON_COMMAND(ID_HISTO_STRETCH, &CImageProcessView::OnHistoStretch)
	ON_COMMAND(ID_END_IN_SEARCH, &CImageProcessView::OnEndInSearch)
	ON_COMMAND(ID_HISTOGRAM, &CImageProcessView::OnHistogram)
	ON_COMMAND(ID_HISTO_EQUAL, &CImageProcessView::OnHistoEqual)
	ON_COMMAND(ID_HISTO_SPEC, &CImageProcessView::OnHistoSpec)
	ON_COMMAND(ID_EMBOSSING, &CImageProcessView::OnEmbossing)
	ON_COMMAND(ID_BLURR, &CImageProcessView::OnBlurr)
	ON_COMMAND(ID_GAUSSIAN_FILTER, &CImageProcessView::OnGaussianFilter)
	ON_COMMAND(ID_SHARPENING, &CImageProcessView::OnSharpening)
	ON_COMMAND(ID_HPF_SHARPENING, &CImageProcessView::OnHpfSharpening)
	ON_COMMAND(ID_LPF_SHARPENING, &CImageProcessView::OnLpfSharpening)
	ON_COMMAND(ID_LAPLACIAN, &CImageProcessView::OnLaplacian)
	ON_COMMAND(ID_FRAME_COMB, &CImageProcessView::OnFrameComb)
	ON_COMMAND(ID_FRAME_AND, &CImageProcessView::OnFrameAnd)
	ON_COMMAND(ID_FRAME_OR, &CImageProcessView::OnFrameOr)
	ON_COMMAND(ID_TRANSLATION, &CImageProcessView::OnTranslation)
	ON_COMMAND(ID_MIRROR_HOR, &CImageProcessView::OnMirrorHor)
	ON_COMMAND(ID_MIRROR_VER, &CImageProcessView::OnMirrorVer)
	ON_COMMAND(ID_ROTATION, &CImageProcessView::OnRotation)
	ON_COMMAND(ID_DIFF_OPERATOR_HOR, &CImageProcessView::OnDiffOperatorHor)
	ON_COMMAND(ID_HOMOGEN_OPERATOR, &CImageProcessView::OnHomogenOperator)
	ON_COMMAND(ID_NEAREST, &CImageProcessView::OnNearest)
	ON_COMMAND(ID_BILINEAR, &CImageProcessView::OnBilinear)
	ON_COMMAND(ID_MEDIAN_SUB, &CImageProcessView::OnMedianSub)
	ON_COMMAND(ID_MEAN_SUB, &CImageProcessView::OnMeanSub)
	ON_COMMAND(ID_FRAME_SUM, &CImageProcessView::OnFrameSum)
	ON_COMMAND(ID_FRAME_SUBTRACTION, &CImageProcessView::OnFrameSubtraction)
	ON_COMMAND(ID_FRAME_MULTIPLY, &CImageProcessView::OnFrameMultiply)
	ON_COMMAND(ID_FRAME_DIVISION, &CImageProcessView::OnFrameDivision)
	ON_COMMAND(ID_BINARY_EROSION, &CImageProcessView::OnBinaryErosion)
	ON_COMMAND(ID_BINARY_DILATION, &CImageProcessView::OnBinaryDilation)
	ON_COMMAND(ID_GRAY_EROSION, &CImageProcessView::OnGrayErosion)
	ON_COMMAND(ID_GRAY_DILATION, &CImageProcessView::OnGrayDilation)
	ON_COMMAND(ID_LOW_PASS_FILTER, &CImageProcessView::OnLowPassFilter)
	ON_COMMAND(ID_HIGH_PASS_FILTER, &CImageProcessView::OnHighPassFilter)
	ON_COMMAND(ID_MEAN_FILTER, &CImageProcessView::OnMeanFilter)
	ON_COMMAND(ID_MEDIAN_FILTER, &CImageProcessView::OnMedianFilter)
	ON_COMMAND(ID_MIN_FILTER, &CImageProcessView::OnMinFilter)
	ON_COMMAND(ID_MAX_FILTER, &CImageProcessView::OnMaxFilter)
	ON_COMMAND(ID_FFT_2D, &CImageProcessView::OnFft2d)
	ON_COMMAND(ID_IFFT_2D, &CImageProcessView::OnIfft2d)
	ON_COMMAND(ID_LPF_FREQUENCY, &CImageProcessView::OnLpfFrequency)
	ON_COMMAND(ID_HPF_Frequency, &CImageProcessView::OnHpfFrequency)
END_MESSAGE_MAP()

// CImageProcessView 생성/소멸

CImageProcessView::CImageProcessView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));

	for (int i = 0; i < 256; i++) {
		BmInfo->bmiColors[i].rgbRed = BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbReserved = 0;
	}
}

CImageProcessView::~CImageProcessView()
{
	if (BmInfo) delete BmInfo;
}

BOOL CImageProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CImageProcessView 그리기

void CImageProcessView::OnDraw(CDC* pDC)
{
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->m_InputImage == NULL)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	int i, j;
	unsigned char R, G, B;

	
	#define WIDTHBYTES(bits)	(((bits)+31)/32*4);        //이미지 가로 바이트 길이는 4바이트의 배수
	//BmInfo;
	int height;
	int width;
	int rwsize;


	//24비트 비트맵 파일의 영상출력
	if (pDoc->hInfo.biBitCount == 24) {
		height = pDoc->hInfo.biHeight;
		width = pDoc->hInfo.biWidth;
		rwsize = WIDTHBYTES(pDoc->hInfo.biBitCount*pDoc->hInfo.biWidth);
		BmInfo->bmiHeader = pDoc->hInfo;
		SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, width, height, 0, 0, 0, height, pDoc->m_InputImage, BmInfo, DIB_RGB_COLORS);
	}

	else if (pDoc->hInfo.biBitCount == 8) { 	//8비트 컬러일 경우
		int index;
		rwsize = WIDTHBYTES(pDoc->hInfo.biBitCount*pDoc->hInfo.biWidth);
		//팔레트를 읽어들이며 반복출력

		for (int i = 0; i < pDoc->hInfo.biHeight;i++)
			for (int j = 0; j < pDoc->hInfo.biWidth;j++) {
				index = pDoc->m_InputImage[i*rwsize + j];
				BYTE R = pDoc->palRGB[index].rgbRed;
				BYTE G = pDoc->palRGB[index].rgbGreen;
				BYTE B = pDoc->palRGB[index].rgbBlue;

				//pDC->SetPixel(j,i,RGB(R,G,B));
				//영상 반전출력
				pDC->SetPixel(j, pDoc->hInfo.biHeight - i - 1, RGB(R, G, B));
			}
	}

	else {
		// 입력 영상 출력
		for (i = 0; i < pDoc->m_height; i++) {
			for (j = 0; j < pDoc->m_width; j++) {
				R = pDoc->m_InputImage[i*pDoc->m_width + j];
				G = B = R;
				pDC->SetPixel(j + 5, i + 5, RGB(R, G, B));
			}
		}

		// 축소된 영상 출력
		for (i = 0; i < pDoc->m_Re_height; i++) {
			for (j = 0; j < pDoc->m_Re_width; j++) {
				R = pDoc->m_OutputImage[i*pDoc->m_Re_width + j];
				G = B = R;
				pDC->SetPixel(j + pDoc->m_width + 10, i + 5, RGB(R, G, B));
			}
		}
	}

}


// CImageProcessView 인쇄


void CImageProcessView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImageProcessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CImageProcessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CImageProcessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CImageProcessView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImageProcessView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImageProcessView 진단

#ifdef _DEBUG
void CImageProcessView::AssertValid() const
{
	CView::AssertValid();
}

void CImageProcessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageProcessDoc* CImageProcessView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessDoc)));
	return (CImageProcessDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageProcessView 메시지 처리기


void CImageProcessView::OnDownSampling()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument(); // Doc 클래스 참조
	ASSERT_VALID(pDoc);

	pDoc->OnDownSampling(); // Doc 클래스에 OnDownSampling 함수 호출

	Invalidate(TRUE); // 화면 갱신

}


void CImageProcessView::OnUpSampling()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument(); // Doc 클래스 참조
	ASSERT_VALID(pDoc);

	pDoc->OnUpSampling(); // Doc 클래스에 OnUpSampling 함수 호출

	Invalidate(TRUE); // 화면 갱신

}


void CImageProcessView::OnQuantization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument(); // Doc 클래스 참조
	ASSERT_VALID(pDoc);

	pDoc->OnQuantization(); // Doc 클래스에 OnQuantization 함수 호출

	Invalidate(TRUE); // 화면 갱신

}


void CImageProcessView::OnSumConstant()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	// 도큐먼트 클래스 참조
	ASSERT_VALID(pDoc); // 인스턴스 주소를 가져옴

	pDoc->OnSumConstant();

	Invalidate(TRUE);
}


void CImageProcessView::OnSubConstant()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();// 도큐먼트 클래스 참조
	ASSERT_VALID(pDoc); // 인스턴스 주소를 가져옴

	pDoc->OnSubConstant();

	Invalidate(TRUE);

}


void CImageProcessView::OnMulConstant()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument(); // 도큐먼트 클래스 참조
	ASSERT_VALID(pDoc); // 인스턴스 주소를 가져옴

	pDoc->OnMulConstant();

	Invalidate(TRUE);

}


void CImageProcessView::OnDivConstant()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument(); // 도큐먼트 클래스 참조
	ASSERT_VALID(pDoc); // 인스턴스 주소를 가져옴

	pDoc->OnDivConstant();

	Invalidate(TRUE);

}


void CImageProcessView::OnAndOperate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnAndOperate();

	Invalidate(TRUE);

}


void CImageProcessView::OnOrOperate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnOrOperate();

	Invalidate(TRUE);

}


void CImageProcessView::OnXorOperate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->OnXorOperate();
	Invalidate(TRUE);
}


void CImageProcessView::OnNegaTransform()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnNegaTransform();

	Invalidate(TRUE);

}


void CImageProcessView::OnGammaCorrection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnGammaCorrection();

	Invalidate(TRUE);

}


void CImageProcessView::OnBinarization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBinarization();

	Invalidate(TRUE);

}


void CImageProcessView::OnStressTransform()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnStressTransform();

	Invalidate(TRUE);

}


void CImageProcessView::OnHistoStretch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistoStretch();

	Invalidate(TRUE);

}


void CImageProcessView::OnEndInSearch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEndInSearch();

	Invalidate(TRUE);


}


void CImageProcessView::OnHistogram()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProcessDoc*pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistogram();
	Invalidate(TRUE);


}


void CImageProcessView::OnHistoEqual()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistoEqual();

	Invalidate(TRUE);


}


void CImageProcessView::OnHistoSpec()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHistoSpec();

	Invalidate(TRUE);

}


void CImageProcessView::OnEmbossing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnEmbossing();

	Invalidate(TRUE);

}


void CImageProcessView::OnBlurr()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBlurr();

	Invalidate(TRUE);

}


void CImageProcessView::OnGaussianFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnGaussianFilter();

	Invalidate(TRUE);

}


void CImageProcessView::OnSharpening()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnSharpening();

	Invalidate(TRUE);

}


void CImageProcessView::OnHpfSharpening()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHpfSharpening();

	Invalidate(TRUE);

}


void CImageProcessView::OnLpfSharpening()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLpfSharpening();

	Invalidate(TRUE);

}


void CImageProcessView::OnLaplacian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLaplacian();

	Invalidate(TRUE);

}


void CImageProcessView::OnFrameComb()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnFrameComb();

	Invalidate(TRUE);

}


void CImageProcessView::OnFrameAnd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnFrameAnd();

	Invalidate(TRUE);

}


void CImageProcessView::OnFrameOr()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnFrameOr();

	Invalidate(TRUE);

}


void CImageProcessView::OnTranslation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnTranslation();

	Invalidate(TRUE);

}


void CImageProcessView::OnMirrorHor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMirrorHor();

	Invalidate(TRUE);

}


void CImageProcessView::OnMirrorVer()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMirrorVer();

	Invalidate(TRUE);

}


void CImageProcessView::OnRotation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnRotation();

	Invalidate(TRUE);

}


void CImageProcessView::OnDiffOperatorHor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnDiffOperatorHor();

	Invalidate(TRUE);

}


void CImageProcessView::OnHomogenOperator()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHomogenOperator();

	Invalidate(TRUE);

}


void CImageProcessView::OnNearest()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnNearest();

	Invalidate(TRUE);

}


void CImageProcessView::OnBilinear()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBilinear();

	Invalidate(TRUE);

}


void CImageProcessView::OnMedianSub()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMedianSub();

	Invalidate(TRUE);

}


void CImageProcessView::OnMeanSub()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMeanSub();

	Invalidate(TRUE);
}


void CImageProcessView::OnFrameSum()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnFrameSum();

	Invalidate(TRUE);

}


void CImageProcessView::OnFrameSubtraction()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnFrameSubtraction();

	Invalidate(TRUE);

}


void CImageProcessView::OnFrameMultiply()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnFrameMultiply();

	Invalidate(TRUE);

}


void CImageProcessView::OnFrameDivision()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnFrameDivision();

	Invalidate(TRUE);

}


void CImageProcessView::OnBinaryErosion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBinaryErosion();

	Invalidate(TRUE);

}


void CImageProcessView::OnBinaryDilation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnBinaryDilation();

	Invalidate(TRUE);

}


void CImageProcessView::OnGrayErosion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnGrayErosion();

	Invalidate(TRUE);

}


void CImageProcessView::OnGrayDilation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnGrayDilation();

	Invalidate(TRUE);
}


void CImageProcessView::OnLowPassFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLowPassFilter();

	Invalidate(TRUE);
}


void CImageProcessView::OnHighPassFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHighPassFilter();

	Invalidate(TRUE);
}


void CImageProcessView::OnMeanFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMeanFilter();

	Invalidate(TRUE);
}


void CImageProcessView::OnMedianFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMedianFilter();

	Invalidate(TRUE);
}


void CImageProcessView::OnMinFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMinFilter();

	Invalidate(TRUE);
}


void CImageProcessView::OnMaxFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnMaxFilter();

	Invalidate(TRUE);

}


void CImageProcessView::OnFft2d()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnFft2d();

	Invalidate(TRUE);
}


void CImageProcessView::OnIfft2d()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnIfft2d();

	Invalidate(TRUE);
}


void CImageProcessView::OnLpfFrequency()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnLpfFrequency();

	Invalidate(TRUE);
}


void CImageProcessView::OnHpfFrequency()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->OnHpfFrequency();

	Invalidate(TRUE);

}
