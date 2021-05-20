
// ImageProcessDoc.cpp: CImageProcessDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageProcess.h"
#endif

#include "ImageProcessDoc.h"

#include <propkey.h>
#include "CDownSampleDlg.h" 
#include "CUpSampleDlg.h"
#include "CQuantizationDlg.h" // 대화상자 사용을 위한 헤더 선언
#include "math.h" // 수학 함수 사용을 위한 헤더 선언
#include "CConstantDlg.h" // 상수 입력 대화상자 사용을 위한 헤더 선언
#include "CStressTransformDlg.h" // 범위 강조 대화상자를 위한 헤더 선언




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

double m_HIST[256];
double m_Sum_Of_HIST[256];
unsigned char m_Scale_HIST[256];

// CImageProcessDoc

IMPLEMENT_DYNCREATE(CImageProcessDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageProcessDoc, CDocument)
//ON_COMMAND(ID_DIV_CONSTANT, &CImageProcessDoc::OnDivConstant)
ON_COMMAND(ID_MUL_CONSTANT, &CImageProcessDoc::OnMulConstant)
ON_COMMAND(ID_SUB_CONSTANT, &CImageProcessDoc::OnSubConstant)
ON_COMMAND(ID_SUM_CONSTANT, &CImageProcessDoc::OnSumConstant)
ON_COMMAND(ID_DIV_CONSTANT, &CImageProcessDoc::OnDivConstant)
ON_COMMAND(ID_AND_OPERATE, &CImageProcessDoc::OnAndOperate)
ON_COMMAND(ID_OR_OPERATE, &CImageProcessDoc::OnOrOperate)
ON_COMMAND(ID_XOR_OPERATE, &CImageProcessDoc::OnXorOperate)
ON_COMMAND(ID_BINARIZATION, &CImageProcessDoc::OnBinarization)
ON_COMMAND(ID_NEGA_TRANSFORM, &CImageProcessDoc::OnNegaTransform)
ON_COMMAND(ID_GAMMA_CORRECTION, &CImageProcessDoc::OnGammaCorrection)
ON_COMMAND(ID_STRESS_TRANSFORM, &CImageProcessDoc::OnStressTransform)
ON_COMMAND(ID_HISTO_STRETCH, &CImageProcessDoc::OnHistoStretch)
ON_COMMAND(ID_END_IN_SEARCH, &CImageProcessDoc::OnEndInSearch)
ON_COMMAND(ID_HISTOGRAM, &CImageProcessDoc::OnHistogram)
ON_COMMAND(ID_HISTO_EQUAL, &CImageProcessDoc::OnHistoEqual)
ON_COMMAND(ID_HISTO_SPEC, &CImageProcessDoc::OnHistoSpec)
ON_COMMAND(ID_EMBOSSING, &CImageProcessDoc::OnEmbossing)
ON_COMMAND(ID_BLURR, &CImageProcessDoc::OnBlurr)
ON_COMMAND(ID_GAUSSIAN_FILTER, &CImageProcessDoc::OnGaussianFilter)
ON_COMMAND(ID_SHARPENING, &CImageProcessDoc::OnSharpening)
ON_COMMAND(ID_HPF_SHARPENING, &CImageProcessDoc::OnHpfSharpening)
ON_COMMAND(ID_LPF_SHARPENING, &CImageProcessDoc::OnLpfSharpening)
ON_COMMAND(ID_LAPLACIAN, &CImageProcessDoc::OnLaplacian)
ON_COMMAND(ID_FRAME_COMB, &CImageProcessDoc::OnFrameComb)
ON_COMMAND(ID_FRAME_AND, &CImageProcessDoc::OnFrameAnd)
ON_COMMAND(ID_FRAME_OR, &CImageProcessDoc::OnFrameOr)
ON_COMMAND(ID_TRANSLATION, &CImageProcessDoc::OnTranslation)
ON_COMMAND(ID_MIRROR_HOR, &CImageProcessDoc::OnMirrorHor)
ON_COMMAND(ID_MIRROR_VER, &CImageProcessDoc::OnMirrorVer)
ON_COMMAND(ID_ROTATION, &CImageProcessDoc::OnRotation)
ON_COMMAND(ID_DIFF_OPERATOR_HOR, &CImageProcessDoc::OnDiffOperatorHor)
ON_COMMAND(ID_HOMOGEN_OPERATOR, &CImageProcessDoc::OnHomogenOperator)
ON_COMMAND(ID_NEAREST, &CImageProcessDoc::OnNearest)
ON_COMMAND(ID_BILINEAR, &CImageProcessDoc::OnBilinear)
ON_COMMAND(ID_MEDIAN_SUB, &CImageProcessDoc::OnMedianSub)
ON_COMMAND(ID_MEAN_SUB, &CImageProcessDoc::OnMeanSub)
ON_COMMAND(ID_FRAME_SUM, &CImageProcessDoc::OnFrameSum)
ON_COMMAND(ID_FRAME_SUBTRACTION, &CImageProcessDoc::OnFrameSubtraction)
ON_COMMAND(ID_FRAME_MULTIPLY, &CImageProcessDoc::OnFrameMultiply)
ON_COMMAND(ID_FRAME_DIVISION, &CImageProcessDoc::OnFrameDivision)
ON_COMMAND(ID_BINARY_EROSION, &CImageProcessDoc::OnBinaryErosion)
ON_COMMAND(ID_BINARY_DILATION, &CImageProcessDoc::OnBinaryDilation)
ON_COMMAND(ID_GRAY_EROSION, &CImageProcessDoc::OnGrayErosion)
ON_COMMAND(ID_GRAY_DILATION, &CImageProcessDoc::OnGrayDilation)
ON_COMMAND(ID_LOW_PASS_FILTER, &CImageProcessDoc::OnLowPassFilter)
ON_COMMAND(ID_HIGH_PASS_FILTER, &CImageProcessDoc::OnHighPassFilter)
ON_COMMAND(ID_MEAN_FILTER, &CImageProcessDoc::OnMeanFilter)
ON_COMMAND(ID_MEDIAN_FILTER, &CImageProcessDoc::OnMedianFilter)
ON_COMMAND(ID_MIN_FILTER, &CImageProcessDoc::OnMinFilter)
ON_COMMAND(ID_MAX_FILTER, &CImageProcessDoc::OnMaxFilter)
ON_COMMAND(ID_FFT_2D, &CImageProcessDoc::OnFft2d)
ON_COMMAND(ID_IFFT_2D, &CImageProcessDoc::OnIfft2d)
ON_COMMAND(ID_LPF_FREQUENCY, &CImageProcessDoc::OnLpfFrequency)
ON_COMMAND(ID_HPF_Frequency, &CImageProcessDoc::OnHpfFrequency)
END_MESSAGE_MAP()


// CImageProcessDoc 생성/소멸

CImageProcessDoc::CImageProcessDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CImageProcessDoc::~CImageProcessDoc()
{
}

BOOL CImageProcessDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CImageProcessDoc serialization

void CImageProcessDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CImageProcessDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CImageProcessDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageProcessDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageProcessDoc 진단

#ifdef _DEBUG
void CImageProcessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProcessDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageProcessDoc 명령


BOOL CImageProcessDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	CFile File; // 파일 객체 선언

	File.Open(lpszPathName, CFile::modeRead | CFile::typeBinary);
	// 파일 열기 대화상자에서 선택한 파일을 지정하고 읽기 모드 선택
	


	File.Read(&hf, sizeof(BITMAPFILEHEADER));	//이 파일이 BMP파일인지 검사 
		
	if (hf.bfType == 0x4D42) {
		//if (hf.bfType != 0x4D42) {
		//	AfxMessageBox(L"Not BMP file!!");
		//	return FALSE;
		//}
		File.Read(&hInfo, sizeof(BITMAPINFOHEADER));	//영상정보의 header를 읽기

		if (hInfo.biBitCount != 8 && hInfo.biBitCount != 24) { //8,24비트가 아닐경우
			AfxMessageBox(L"Gray/True Color Possible!!");
			return FALSE;
		}

		if (hInfo.biBitCount == 8) {
			//8비트의 경우 팔레트를 생성해 주어야 한다. 총 256가지 색이므로 그 길이만큼 읽어들인다
			//이미지의 크기는 파일 총 길이에서, 두 헤드와 팔레트의 사이즈를 제외한다.
			File.Read(palRGB, sizeof(RGBQUAD) * 256);
			m_size = File.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER) - 256 * sizeof(RGBQUAD);
		}

		else if (hInfo.biBitCount == 24) {	//컬러영상
			m_size = File.GetLength() - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
		}

		//이미지를 저장, 출력할 배열생성.
		m_InputImage = new unsigned char[m_size];
		m_OutputImage = new unsigned char[m_size];
		File.Read(m_InputImage, m_size);
		File.Close();

		//이미지의 길이정보
		m_height = hInfo.biHeight;
		m_width = hInfo.biWidth;


		// Reversed Image
		for (int i = 0; i < hInfo.biHeight; i++) {
			for (int j = 0; j < hInfo.biWidth; j++)
				m_InputImage[i*m_width + j] = 255 - m_InputImage[i*m_width + j];
		}

		return TRUE;
	}


	else {
		// 이 책에서는 영상의 크기 256*256, 512*512, 640*480만을 사용한다.
		if (File.GetLength() == 256 * 256) { // RAW 파일의 크기 결정

			m_height = 256;
			m_width = 256;
		}
		else if (File.GetLength() == 512 * 512) { // RAW 파일의 크기 결정
			m_height = 512;
			m_width = 512;
		}
		else if (File.GetLength() == 640 * 480) { // RAW 파일의 크기 결정
			m_height = 480;
			m_width = 640;
		}
		else {
			AfxMessageBox(L"Not Support Image Size"); // 해당 크기가 없는 경우
			return 0;
		}
		m_size = m_width * m_height; // 영상의 크기 계산

		m_InputImage = new unsigned char[m_size];
		// 입력 영상의 크기에 맞는 메모리 할당

		for (int i = 0; i < m_size; i++)
			m_InputImage[i] = 255; // 초기화
		File.Read(m_InputImage, m_size); // 입력 영상 파일 읽기
		File.Close(); // 파일 닫기

		return TRUE;
	}




}


BOOL CImageProcessDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CFile File; // 파일 객체 선언
	CFileDialog SaveDlg(FALSE, L"raw", NULL, OFN_HIDEREADONLY);
	// raw 파일을 다른 이름으로 저장하기를 위한 대화상자 객체 선언

	if (SaveDlg.DoModal() == IDOK) {
		// DoModal 멤버 함수에서 저장하기 수행
		File.Open(SaveDlg.GetPathName(), CFile::modeCreate |
			CFile::modeWrite);
		// 파일 열기
		File.Write(m_InputImage, m_size); // 파일 쓰기
		File.Close(); // 파일 닫기
	}


	if (!File.Open(lpszPathName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))

		return FALSE;

	//정보저장

	File.Write(&hf, sizeof(BITMAPFILEHEADER));

	File.Write(&hInfo, sizeof(BITMAPINFOHEADER));

	if (hInfo.biBitCount == 8)

		File.Write(palRGB, sizeof(RGBQUAD) * 256);

	File.Write(m_InputImage, hInfo.biSizeImage);
	File.Close();

	return TRUE;

	return TRUE;
}


void CImageProcessDoc::OnDownSampling()
{
	// TODO: 여기에 구현 코드 추가.
	int i, j;
	CDownSampleDlg dlg;
	if (dlg.DoModal() == IDOK) // 대화상자의 활성화 여부
	{
		m_Re_height = m_height / dlg.m_DownSampleRate;
		// 축소 영상의 세로 길이를 계산
		m_Re_width = m_width / dlg.m_DownSampleRate;
		// 축소 영상의 가로 길이를 계산
		m_Re_size = m_Re_height * m_Re_width;
		// 축소 영상의 크기를 계산

		m_OutputImage = new unsigned char[m_Re_size];
		// 축소 영상을 위한 메모리 할당

		for (i = 0; i < m_Re_height; i++) {
			for (j = 0; j < m_Re_width; j++) {
				m_OutputImage[i*m_Re_width + j]
					= m_InputImage[(i*dlg.m_DownSampleRate*m_width) + dlg.m_DownSampleRate*j];
				// 축소 영상을 생성
			}
		}
	}

}


void CImageProcessDoc::OnUpSampling()
{
	// TODO: 여기에 구현 코드 추가.
	int i, j;

	CUpSampleDlg dlg;
	if (dlg.DoModal() == IDOK) { // DoModal 대화상자의 활성화 여부
		
		int rate = dlg.m_UpSampleRate;

		m_Re_height = m_height * rate;
		// 확대 영상의 세로 길이 계산
		m_Re_width = m_width * rate;
		// 확대 영상의 가로 길이 계산
		m_Re_size = m_Re_height * m_Re_width;
		// 확대 영상의 크기 계산
		m_OutputImage = new unsigned char[m_Re_size];
		// 확대 영상을 위한 메모리 할당

		//for (i = 0; i < m_Re_size; i++)
		//	m_OutputImage[i] = 0; // 초기화
		
		for (i = 0; i < m_height; i++) {
			for (j = 0; j < m_width; j++) {
				m_OutputImage[i*m_Re_width*rate + j*rate] = m_InputImage[i*m_width + j];

				int dx = 0;
				int dy = 0;
				int dxy = 0;

				if ( (i == m_height - 1) || (j == m_height - 1) ) {
					dx, dy, dxy = 0;
				}
				else {				
					dx = (m_InputImage[i*m_width + j + 1] - m_InputImage[i*m_width + j] / (rate-1));
					dy = (m_InputImage[(i+1)*m_width + j] - m_InputImage[i*m_width + j] / (rate-1));
					dxy = (m_InputImage[(i+1)*m_width + j + 1] - m_InputImage[i*m_width + j] / (rate-1));
				}

				for (int k = 0; k < rate; k++) {
					for (int l = 0; l < rate; l++) {

						if (k == 0 && l == 0) {
							continue;
						}

						else if (k < l) {
							m_OutputImage[i*m_Re_width*rate + k * m_Re_width + j * rate + l] =
								m_InputImage[i*m_width + j];//+((k*dxy + l * dx) / (k + l));
						}

						else if (k == l) {
							m_OutputImage[i*m_Re_width*rate + k * m_Re_width + j * rate + l] =
								m_InputImage[i*m_width + j];// +dxy;
						}

						else if (k > l) {
							m_OutputImage[i*m_Re_width*rate + k * m_Re_width + j * rate + l] =
								m_InputImage[i*m_width + j];// +((l*dxy + k * dy) / (k + l));
						}
					}  
				}	
			}
		}

		//int i, j, x, y;
		//double rx, ry;
		//for (i = 0; i < m_Re_height; i++) { //가장 가까운 픽셀의 값을 참조해서 보간하는 방법
		//	for (j = 0; j < m_Re_width; j++) {
		//		rx = (m_width - 1) * i / (m_Re_width - 1);
		//		ry = (m_height - 1) * j / (m_Re_height - 1);
		//		x = (rx + 0.5);
		//		y = (ry + 0.5);

		//		if (x >= m_width) x = m_width - 1;
		//		if (y >= m_height) y = m_height - 1;

		//		m_OutputImage[i*m_Re_width + j] = m_InputImage[x*m_width + y];
		//		//재배치를 통한 영상의 확대
		//	}
		//}
	}
}


void CImageProcessDoc::OnQuantization()
{
	// TODO: 여기에 구현 코드 추가.
	CQuantizationDlg dlg;
	if (dlg.DoModal() == IDOK)
		// 양자화 비트 수를 결정하는 대화상자의 활성화 여부
	{
		int i, j, value, LEVEL;
		double HIGH, *TEMP;

		m_Re_height = m_height;
		m_Re_width = m_width;
		m_Re_size = m_Re_height * m_Re_width;

		m_OutputImage = new unsigned char[m_Re_size];
		// 양자화 처리된 영상을 출력하기 위한 메모리 할당

		TEMP = new double[m_size];
		// 입력 영상 크기(m_size)와 동일한 메모리 할당

		LEVEL = 256; // 입력 영상의 양자화 단계(28=256)
		HIGH = 256.;

		value = (int)pow(2, dlg.m_QuantBit);
		// 양자화 단계 결정(예 : 24=16)

		for (i = 0; i < m_size; i++) {
			for (j = 0; j < value; j++) {
				if (m_InputImage[i] >= (LEVEL / value)*j &&
					m_InputImage[i] < (LEVEL / value)*(j + 1)) {
					TEMP[i] = (double)(HIGH / value)*j; // 양자화 수행
				}
			}
		}
		for (i = 0; i < m_size; i++) {
			m_OutputImage[i] = (unsigned char)TEMP[i];
			// 결과 영상 생성
		}
	}

}


void CImageProcessDoc::OnSumConstant()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CConstantDlg dlg; // 상수 값을 입력받는 대화상자

	int i;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (dlg.DoModal() == IDOK) {
		for (i = 0; i < m_size; i++) {
			if (m_InputImage[i] + dlg.m_Constant >= 255)
				m_OutputImage[i] = 255;
			// 출력 값이 255보다 크면 255 출력
			else
				m_OutputImage[i] = (unsigned char)(m_InputImage[i] + dlg.m_Constant);
			// 상수 값과 화소 값과의 덧셈
		}
	}

}


void CImageProcessDoc::OnSubConstant()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CConstantDlg dlg;

	int i;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (dlg.DoModal() == IDOK) {
		for (i = 0; i < m_size; i++) {
			if (m_InputImage[i] - dlg.m_Constant < 0)
				m_OutputImage[i] = 0; // 출력 값이 255보다 크면 255를 출력
			else
				m_OutputImage[i] = (unsigned char)(m_InputImage[i] - dlg.m_Constant);
			// 상수 값과 화소 값과의 뺄셈
		}
	}

}


void CImageProcessDoc::OnMulConstant()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CConstantDlg dlg;

	int i;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (dlg.DoModal() == IDOK) {
		for (i = 0; i < m_size; i++) {
			if (m_InputImage[i] * dlg.m_Constant > 255)
				m_OutputImage[i] = 255;
			// 곱의 값이 255보다 크면 255를 출력
			else if (m_InputImage[i] * dlg.m_Constant < 0)
				m_OutputImage[i] = 0;
			// 곱의 값이 0보다 작으면 0을 출력
			else
				m_OutputImage[i]
				= (unsigned char)(m_InputImage[i] * dlg.m_Constant);
			// 상수 값과 화소 값 곱셈
		}
	}

}

void CImageProcessDoc::OnDivConstant()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CConstantDlg dlg;

	int i;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (dlg.DoModal() == IDOK) {
		for (i = 0; i < m_size; i++) {
			if (m_InputImage[i] / dlg.m_Constant > 255)
				m_OutputImage[i] = 255;
			// 나눗셈의 값이 255보다 크면 255를 출력
			else if (m_InputImage[i] / dlg.m_Constant < 0)
				m_OutputImage[i] = 0;
			// 나눗셈의 값이 0보다 작으면 0을 출력
			else
				m_OutputImage[i]
				= (unsigned char)(m_InputImage[i] / dlg.m_Constant);
			// 상수 값과 화소 값 나눗셈
		}
	}

}


void CImageProcessDoc::OnAndOperate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CConstantDlg dlg;
	int i;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (dlg.DoModal() == IDOK) {
		for (i = 0; i < m_size; i++) {
			// 비트 단위 AND 연산
			if ((m_InputImage[i] & (unsigned char)dlg.m_Constant) >= 255)
			{
				m_OutputImage[i] = 255;
			}
			else if ((m_InputImage[i] & (unsigned char)dlg.m_Constant) < 0)
			{
				m_OutputImage[i] = 0;
			}
			else {
				m_OutputImage[i] = (m_InputImage[i]
					& (unsigned char)dlg.m_Constant);
			}
		}
	}

}


void CImageProcessDoc::OnOrOperate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CConstantDlg dlg;
	int i;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (dlg.DoModal() == IDOK) {
		for (i = 0; i < m_size; i++) {
			// 비트 단위 OR 연산
			if ((m_InputImage[i] | (unsigned char)dlg.m_Constant) >= 255) {
				m_OutputImage[i] = 255;
			}
			else if ((m_InputImage[i] | (unsigned char)dlg.m_Constant) < 0) {
				m_OutputImage[i] = 0;
			}
			else {
				m_OutputImage[i] = (m_InputImage[i] |
					(unsigned char)dlg.m_Constant);
			}
		}
	}

}


void CImageProcessDoc::OnXorOperate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CConstantDlg dlg;
	int i;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (dlg.DoModal() == IDOK) {
		for (i = 0; i < m_size; i++) {
			// 비트 단위 XOR 연산
			if ((m_InputImage[i] ^ (unsigned char)dlg.m_Constant) >= 255) {
				m_OutputImage[i] = 255;
			}
			else if ((m_InputImage[i] ^ (unsigned char)dlg.m_Constant) < 0) {
				m_OutputImage[i] = 0;
			}
			else {
				m_OutputImage[i] = (m_InputImage[i]
					^ (unsigned char)dlg.m_Constant);
			}
		}
	}

}


void CImageProcessDoc::OnBinarization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CConstantDlg dlg;

	int i;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (dlg.DoModal() == IDOK) {
		for (i = 0; i < m_size; i++) {
			if (m_InputImage[i] >= dlg.m_Constant)
				m_OutputImage[i] = 255; // 임계 값보다 크면 255 출력
			else
				m_OutputImage[i] = 0; // 임계 값보다 작으면 0 출력
		}
	}

}


void CImageProcessDoc::OnNegaTransform()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	for (i = 0; i < m_size; i++)
		m_OutputImage[i] = 255 - m_InputImage[i]; // 영상 반전을 수행

}


void CImageProcessDoc::OnGammaCorrection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CConstantDlg dlg;

	int i;
	double temp;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (dlg.DoModal() == IDOK) {
		for (i = 0; i < m_size; i++) {
			temp = pow(m_InputImage[i], 1 / dlg.m_Constant);
			// 감마 값 계산
			if (temp < 0)
				m_OutputImage[i] = 0;
			else if (temp > 255)
				m_OutputImage[i] = 255;
			else
				m_OutputImage[i] = (unsigned char)temp;
		}
	}

}


void CImageProcessDoc::OnStressTransform()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CStressTransformDlg dlg;

	int i;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (dlg.DoModal() == IDOK) {
		for (i = 0; i < m_size; i++) {
			// 입력 값이 강조 시작 값과 강조 종료 값 사이에 위치하면 255 출력
			if (m_InputImage[i] >= dlg.m_StartPoint &&
				m_InputImage[i] <= dlg.m_EndPoint)
				m_OutputImage[i] = 255;
			else
				m_OutputImage[i] = m_InputImage[i];
		}
	}

}


void CImageProcessDoc::OnHistoStretch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i;
	unsigned char LOW, HIGH, MAX, MIN;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	LOW = 0;
	HIGH = 255;

	MIN = m_InputImage[0]; // 최소값을 찾기 위한 초기값
	MAX = m_InputImage[0]; // 최대값을 찾기 위한 초기값

	// 입력 영상의 최소값 찾기
	for (i = 0; i < m_size; i++) {
		if (m_InputImage[i] < MIN)
			MIN = m_InputImage[i];
	}

	// 입력 영상의 최대값 찾기
	for (i = 0; i < m_size; i++) {
		if (m_InputImage[i] > MAX)
			MAX = m_InputImage[i];
	}

	m_OutputImage = new unsigned char[m_Re_size];

	// 히스토그램 stretch
	for (i = 0; i < m_size; i++)
		m_OutputImage[i] = (unsigned char)((m_InputImage[i] - MIN)*HIGH / (MAX - MIN));

}


void CImageProcessDoc::OnEndInSearch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i;
	unsigned char LOW, HIGH, MAX, MIN;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	LOW = 0;
	HIGH = 255;

	MIN = m_InputImage[0];
	MAX = m_InputImage[0];

	for (i = 0; i < m_size; i++) {
		if (m_InputImage[i] < MIN)
			MIN = m_InputImage[i];
	}

	for (i = 0; i < m_size; i++) {
		if (m_InputImage[i] > MAX)
			MAX = m_InputImage[i];
	}

	m_OutputImage = new unsigned char[m_Re_size];

	for (i = 0; i < m_size; i++) {
		// 원본 영상의 최소값보다 작은 값은 0
		if (m_InputImage[i] <= MIN) {
			m_OutputImage[i] = 0;
		}

		// 원본 영상의 최대값보다 큰 값은 255
		else if (m_InputImage[i] >= MAX) {
			m_OutputImage[i] = 255;
		}
		else
			m_OutputImage[i] = (unsigned char)((m_InputImage[i] -
				MIN)*HIGH / (MAX - MIN));
	}

}


void CImageProcessDoc::OnHistogram()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	
	// 히스토그램의 값은 0~255
	// 히스토그램의 크기 값을 MAX=255로 정규화하여 출력
	// 히스트그램의 크기 : 256*256 지정

	int i, j, value;
	unsigned char LOW, HIGH;
	double MAX, MIN, DIF;

	m_Re_height = 256;
	m_Re_width = 256;
	m_Re_size = m_Re_height * m_Re_width;

	LOW = 0;
	HIGH = 255;

	// 초기화
	for (i = 0; i < 256; i++)
		m_HIST[i] = LOW;

	// 빈도 수 조사
	for (i = 0; i < m_size; i++) {
		value = (int)m_InputImage[i];
		m_HIST[value]++;
	}

	// 정규화
	MAX = m_HIST[0];
	MIN = m_HIST[0];

	for (i = 0; i < 256; i++) {
		if (m_HIST[i] > MAX)
			MAX = m_HIST[i];
	}

	for (i = 0; i < 256; i++) {
		if (m_HIST[i] < MIN)
			MIN = m_HIST[i];
	}

	DIF = MAX - MIN;

	// 정규화된 히스토그램
	for (i = 0; i < 256; i++)
		m_Scale_HIST[i] = (unsigned char)((m_HIST[i] - MIN) * HIGH / DIF);

	// 정규화된 히스토그램 출력
	m_OutputImage = new unsigned char[m_Re_size + (256 * 20)];

	for (i = 0; i < m_Re_size; i++)
		m_OutputImage[i] = 255;

	// 정규화된 히스토그램의 값은 출력 배열에 검은 점(0)으로 표현
	for (i = 0; i < 256; i++) {
		for (j = 0; j < m_Scale_HIST[i]; j++) {
			m_OutputImage[m_Re_width*(m_Re_height - j - 1) + i] = 0;
		}
	}

	// 히스토그램을 출력하고 그 아래 부분에 히스토그램의 색을 표시
	for (i = m_Re_height; i < m_Re_height + 5; i++) {
		for (j = 0; j < 256; j++) {
			m_OutputImage[m_Re_height * i + j] = 255;
		}
	}

	for (i = m_Re_height + 5; i < m_Re_height + 20; i++) {
		for (j = 0; j < 256; j++) {
			m_OutputImage[m_Re_height * i + j] = j;
		}
	}

	m_Re_height = m_Re_height + 20;
	m_Re_size = m_Re_height * m_Re_width;
}


void CImageProcessDoc::OnHistoEqual()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, value;
	unsigned char LOW, HIGH, Temp;
	double SUM = 0.0;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	LOW = 0;
	HIGH = 255;

	// 초기화
	for (i = 0; i < 256; i++)
		m_HIST[i] = LOW;

	// 빈도 수 조사
	for (i = 0; i < m_size; i++) {
		value = (int)m_InputImage[i];
		m_HIST[value]++;
	}

	// 누적 히스토그램 생성
	for (i = 0; i < 256; i++) {
		SUM += m_HIST[i];
		m_Sum_Of_HIST[i] = SUM;
	}

	m_OutputImage = new unsigned char[m_Re_size];

	// 입력 영상을 평활화된 영상으로 출력
	for (i = 0; i < m_size; i++) {
		Temp = m_InputImage[i];
		m_OutputImage[i] = (unsigned char)(m_Sum_Of_HIST[Temp] * HIGH / m_size);
	}

}


void CImageProcessDoc::OnHistoSpec()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, value, Dvalue, top, bottom, DADD;
	unsigned char *m_DTEMP, m_Sum_Of_ScHIST[256], m_TABLE[256];
	unsigned char LOW, HIGH, Temp, *m_Org_Temp;
	double m_DHIST[256], m_Sum_Of_DHIST[256], SUM = 0.0, DSUM = 0.0;
	double DMAX, DMIN;

	top = 255;
	bottom = top - 1;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];
	m_Org_Temp = new unsigned char[m_size];

	CFile File;
	CFileDialog OpenDlg(TRUE);

	// 원하는 히스토그램이 있는 영상을 입력받음
	if (OpenDlg.DoModal() == IDOK) {
		File.Open(OpenDlg.GetFileName(), CFile::modeRead);

		if (File.GetLength() == (unsigned)m_size) {
			m_DTEMP = new unsigned char[m_size];
			File.Read(m_DTEMP, m_size);
			File.Close();
		}
		else {
			AfxMessageBox(L"Image size not matched");
			// 같은 크기의 영상을 대상으로 함
			return;
		}
	}

	LOW = 0;
	HIGH = 255;
	
	m_DTEMP = new unsigned char[m_size]; /////////////

	// 초기화
	for (i = 0; i < 256; i++) {
		m_HIST[i] = LOW;
		m_DHIST[i] = LOW;
		m_TABLE[i] = LOW;
	}

	// 빈도 수 조사
	for (i = 0; i < m_size; i++) {
		value = (int)m_InputImage[i];
		m_HIST[value]++;
		Dvalue = (int)m_DTEMP[i];
		m_DHIST[Dvalue]++;
	}

	// 누적 히스토그램 조사
	for (i = 0; i < 256; i++) {
		SUM += m_HIST[i];
		m_Sum_Of_HIST[i] = SUM;
		DSUM += m_DHIST[i];
		m_Sum_Of_DHIST[i] = DSUM;
	}
	// 원본 영상의 평활화
	for (i = 0; i < m_size; i++) {
		Temp = m_InputImage[i];
		m_Org_Temp[i] = (unsigned char)(m_Sum_Of_HIST[Temp] * HIGH / m_size);
	}

	// 누적 히스토그램에서 최소값과 최대값 지정
	DMIN = m_Sum_Of_DHIST[0];
	DMAX = m_Sum_Of_DHIST[255];


	// 원하는 영상을 평활화
	for (i = 0; i < 256; i++) {
		m_Sum_Of_ScHIST[i] = (unsigned char)((m_Sum_Of_DHIST[i]
			- DMIN)*HIGH / (DMAX - DMIN));
	}

	// 룩업테이블을 이용한 명세화
	for (; ; ) {
		for (i = m_Sum_Of_ScHIST[bottom];
			i <= m_Sum_Of_ScHIST[top]; i++) {
			m_TABLE[i] = top;
		}
		top = bottom;
		bottom = bottom - 1;

		if (bottom < -1)
			break;
	}

	for (i = 0; i < m_size; i++) {
		DADD = (int)m_Org_Temp[i];
		m_OutputImage[i] = m_TABLE[DADD];
	}

}


void CImageProcessDoc::OnEmbossing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j;
	//double EmboMask[3][3] = { {-1., 0., 0.}, {0., 0., 0.}, {0., 0., 1.} };
	// 마스크 선택
	 //double EmboMask[3][3] = {{0., 0., 0.}, {0., 1., 0.}, {0., 0., 0.}};
	 double EmboMask[3][3] = {{1., 1., 1.}, {1., -8.,1.}, {1., 1., 1.}};

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];
	m_tempImage = OnMaskProcess(m_InputImage, EmboMask);
	// OnMaskProcess 함수를 호출하여 회선 처리를 한다.

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255.)
				m_tempImage[i][j] = 255.;
			if (m_tempImage[i][j] < 0.)
				m_tempImage[i][j] = 0.;
		}
	} // 회선 처리 결과가 0~255 사이 값이 되도록 한다.

	// m_tempImage = OnScale(m_tempImage, m_Re_height, m_Re_width);
    // 정규화 함수를 사용할 때

    // 회선 처리 결과나 정규화 처리 결과는 2차원 배열 값이 되므로
    // 2차원 배열을 1차원 배열로 바꾸어 출력하도록 한다.
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i*m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}

}


double** CImageProcessDoc::OnMaskProcess(unsigned char *Target, double Mask[3][3])
{
	// TODO: 여기에 구현 코드 추가.
	// 회선 처리가 일어나는 함수
	int i, j, n, m;
	double **tempInputImage, **tempOutputImage, S = 0.0;

	tempInputImage = Image2DMem(m_height + 2, m_width + 2);
	// 입력 값을 위한 메모리 할당
	tempOutputImage = Image2DMem(m_height, m_width);
	// 출력 값을 위한 메모리 할당

	// 1차원 입력 영상의 값을 2차원 배열에 할당한다.
	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			tempInputImage[i + 1][j + 1]
				= (double)Target[i * m_width + j];
		}
	}

	// 회선연산
	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			for (n = 0; n < 3; n++) {
				for (m = 0; m < 3; m++) {
					S += Mask[n][m] * tempInputImage[i + n][j + m];
				}
			} // 회선 마스크의 크기 만큼 이동하면서 값을 누적
			tempOutputImage[i][j] = S; // 누적된 값을 출력 메모리에 저장
			S = 0.0; // 다음 블록으로 이동하면 누적 값을 초기화
		}
	}
	return tempOutputImage; // 결과 값 반환
}



double** CImageProcessDoc::OnScale(double **Target, int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	// 정규화를 위한 함수
	int i, j;
	double min, max;

	min = max = Target[0][0];

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			if (Target[i][j] <= min)
				min = Target[i][j];
		}
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			if (Target[i][j] >= max)
				max = Target[i][j];
		}
	}

	max = max - min;

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			Target[i][j] = (Target[i][j] - min) * (255. / max);
		}
	}

	return Target;
}


double** CImageProcessDoc::Image2DMem(int height, int width)
{
	// TODO: 여기에 구현 코드 추가.
	// 2차원 메모리 할당을 위한 함수
	double** temp;
	int i, j;
	temp = new double *[height];
	for (i = 0; i < height; i++) {
		temp[i] = new double[width];
	}
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			temp[i][j] = 0.0;
		}
	} // 할당된 2차원 메모리를 초기화
	return temp;

}


void CImageProcessDoc::OnBlurr()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j;
	double BlurrMask[3][3] = { {1. / 9., 1. / 9., 1. / 9.},
	   {1. / 9., 1. / 9., 1. / 9.}, {1. / 9., 1. / 9., 1. / 9.} };

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	m_tempImage = OnMaskProcess(m_InputImage, BlurrMask);
	// 블러링 처리
	// m_tempImage = OnScale(m_tempImage, m_Re_height, m_Re_width);

	// 정규화
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255.)
				m_tempImage[i][j] = 255.;
			if (m_tempImage[i][j] < 0.)
				m_tempImage[i][j] = 0.;
		}
	}
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i*m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}
}


void CImageProcessDoc::OnGaussianFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j;
	double GaussianMask[3][3] = { {1. / 16., 1. / 8., 1. / 16.},
		  {1. / 8., 1. / 4., 1. / 8.}, {1. / 16., 1. / 8., 1. / 16.} };

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	m_tempImage = OnMaskProcess(m_InputImage, GaussianMask);
	// m_tempImage = OnScale(m_tempImage, m_Re_height, m_Re_width);

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255.)
				m_tempImage[i][j] = 255.;
			if (m_tempImage[i][j] < 0.)
				m_tempImage[i][j] = 0.;
		}
	}

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i*m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}

}


void CImageProcessDoc::OnSharpening()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j;
	//double SharpeningMask[3][3] = {{-1., -1., -1.}, {-1., 9., -1.}, { -1., -1., -1. }
	double SharpeningMask[3][3] = { {0., -1., 0.}, {-1., 5., -1.}, {0., -1., 0.} };


	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	m_tempImage = OnMaskProcess(m_InputImage, SharpeningMask);
	// m_tempImage = OnScale(m_tempImage, m_Re_height, m_Re_width);

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255.)
				m_tempImage[i][j] = 255.;
			if (m_tempImage[i][j] < 0.)
				m_tempImage[i][j] = 0.;
		}
	}

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i*m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}

}


void CImageProcessDoc::OnHpfSharpening()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j;
	double HpfSharpMask[3][3] = { {-1. / 9., -1. / 9., -1. / 9.},
	   {-1. / 9., 8. / 9., -1. / 9.}, {-1. / 9., -1. / 9., -1. / 9.} };

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	m_tempImage = OnMaskProcess(m_InputImage, HpfSharpMask);
	// m_tempImage = OnScale(m_tempImage, m_Re_height, m_Re_width);

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255.)
				m_tempImage[i][j] = 255.;
			if (m_tempImage[i][j] < 0.)
				m_tempImage[i][j] = 0.;
		}
	}

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i*m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}

}


void CImageProcessDoc::OnLpfSharpening()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CConstantDlg dlg; // 상수를 입력받으려고 대화상자 선언

	int i, j, alpha;
	double LpfSharpMask[3][3] = { {1. / 9., 1. / 9., 1. / 9.},
	   {1. / 9., 1. / 9., 1. / 9.}, {1. / 9., 1. / 9., 1. / 9.} };

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (dlg.DoModal() == IDOK) {
		alpha = (int)dlg.m_Constant;
		// 대화상자를 이용하여 상수를 입력받는다.
	}

	m_tempImage = OnMaskProcess(m_InputImage, LpfSharpMask);

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			m_tempImage[i][j] = (alpha * m_InputImage
				[i*m_width + j]) - (unsigned char)m_tempImage[i][j];
		}
	}
	// m_tempImage = OnScale(m_tempImage, m_Re_height, m_Re_width);

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255.)
				m_tempImage[i][j] = 255.;
			if (m_tempImage[i][j] < 0.)
				m_tempImage[i][j] = 0.;
		}
	}

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i*m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}

}


void CImageProcessDoc::OnLaplacian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j;
	double LaplacianMask[3][3] = { {0., 1., 0.}, {1., -4., 1.}, {0., 1., 0.} };

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];

	m_tempImage = OnMaskProcess(m_InputImage, LaplacianMask);

	// m_tempImage = OnScale(m_tempImage, m_Re_height, m_Re_width);

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255.)
				m_tempImage[i][j] = 255.;
			if (m_tempImage[i][j] < 0.)
				m_tempImage[i][j] = 0.;
		}
	}

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i* m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}

}


void CImageProcessDoc::OnFrameComb()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CFile File;
	CFileDialog OpenDlg(TRUE);
	int i;
	unsigned char *temp, *masktemp, maskvalue;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	AfxMessageBox(L"합성할 영상을 입력하시오");

	if (OpenDlg.DoModal() == IDOK) { // 합성할 영상을 입력
		File.Open(OpenDlg.GetFileName(), CFile::modeRead);
		temp = new unsigned char[m_size];
		File.Read(temp, m_size);

		if ((unsigned)m_width * m_height != File.GetLength()) {
			AfxMessageBox(L"Image size not matched");
			// 영상의 크기가 같을 때
			return;
		}
		File.Close();
	}
	// 입력 영상, 합성할 영상, 마스크 영상의 크기가 같아야 한다.
	
	temp = new unsigned char[m_size];

	AfxMessageBox(L"입력 영상의 마스크 영상을 입력하시오");
	if (OpenDlg.DoModal() == IDOK) { // 입력 영상의 마스크 영상
		File.Open(OpenDlg.GetFileName(), CFile::modeRead);
		masktemp = new unsigned char[m_size];
		File.Read(masktemp, m_size);
		File.Close();
	}

	masktemp = new unsigned char[m_size];

	for (i = 0; i < m_size; i++) {
		maskvalue = 255 - masktemp[i];
		// 영상의 최대값에서 마스크 영상의 값을 뺀다.
		m_OutputImage[i]
			= (m_InputImage[i] & masktemp[i]) | (temp[i] & maskvalue);
		// 입력 영상과 마스크 영상은 AND 연산을 하고, 합성할 영상은
		// (255-마스크 영상) 값과 AND 연산을 실행한 후 두 값을 더한다.
	}


}


void CImageProcessDoc::OnFrameAnd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFile File;
	CFileDialog OpenDlg(TRUE);
	int i;
	unsigned char *temp;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (OpenDlg.DoModal() == IDOK) {
		File.Open(OpenDlg.GetFileName(), CFile::modeRead);

		if (File.GetLength() == (unsigned)m_width * m_height) {
			temp = new unsigned char[m_size];
			File.Read(temp, m_size);
			File.Close();

			// 프레임 간에 픽셀 대 픽셀로 AND 연산 실행
			for (i = 0; i < m_size; i++) {
				m_OutputImage[i]
					= (unsigned char)(m_InputImage[i] & temp[i]);
			}
		}
		else {
			AfxMessageBox(L"Image size not matched");
			return;
		}
	}

}


void CImageProcessDoc::OnFrameOr()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFile File;
	CFileDialog OpenDlg(TRUE);
	int i;
	unsigned char *temp;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (OpenDlg.DoModal() == IDOK) {
		File.Open(OpenDlg.GetFileName(), CFile::modeRead);
		if (File.GetLength() == (unsigned)m_width * m_height) {
			temp = new unsigned char[m_size];

			File.Read(temp, m_size);
			File.Close();

			// 프레임 간에 픽셀 대 픽셀로 OR 연산 실행
			for (i = 0; i < m_size; i++) {
				m_OutputImage[i]
					= (unsigned char)(m_InputImage[i] | temp[i]);
			}
		}
		else {
			AfxMessageBox(L"Image size not matched");
			return;
		}
	}

}

void CImageProcessDoc::OnTranslation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j;
	int h_pos = 30, w_pos = 130;
	double **tempArray;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	m_tempImage = Image2DMem(m_height, m_width);
	tempArray = Image2DMem(m_Re_height, m_Re_width);

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			m_tempImage[i][j] = (double)m_InputImage[i*m_width + j];
		}
	}

	for (i = 0; i < m_height - h_pos; i++) {
		for (j = 0; j < m_width - w_pos; j++) {
			tempArray[i + h_pos][j + w_pos] = m_tempImage[i][j];
			// 입력 영상을 h_pos, w_pos만큼 이동
		}
	}

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i* m_Re_width + j]
				= (unsigned char)tempArray[i][j];
		}
	}

	delete[] m_tempImage;
	delete[] tempArray;

}


void CImageProcessDoc::OnMirrorHor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			m_OutputImage[i*m_width + m_width - j - 1] =
				m_InputImage[i*m_width + j];
			// 입력 영상의 배열 값을 출력 영상을 위한 
			// 배열의 수평축 뒷자리부터 저장
		}
	}

}


void CImageProcessDoc::OnMirrorVer()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			m_OutputImage[(m_height - i - 1)*m_width + j]
				= m_InputImage[i*m_width + j];
			// 입력 영상의 값을 출력 영상을 위한 배열의 수직축 뒷자리부터 저장
		}
	}

}


void CImageProcessDoc::OnRotation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, CenterH, CenterW, newH, newW, degree = 45;
	// degree = 회전할 각도
	double Radian, PI, **tempArray, Value;

	m_Re_height = m_height; // 회전된 영상의 높이
	m_Re_width = m_width; // 회전된 영상의 너비
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];
	PI = 3.14159265358979; // 회전각을 위한 PI 값

	Radian = (double)degree*PI / 180.0;
	// degree 값을 radian으로 변경
	CenterH = m_height / 2; // 영상의 중심 좌표
	CenterW = m_width / 2; // 영상의 중심 좌표

	m_tempImage = Image2DMem(m_height, m_width);
	tempArray = Image2DMem(m_Re_height, m_Re_width);

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			m_tempImage[i][j] = (double)m_InputImage[i*m_width + j];
		}
	}

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			// 회전 변환 행렬을 이용하여 회전하게 될 좌표 값 계산
			newH = (int)((i - CenterH)*cos(Radian)
				- (j - CenterW)*sin(Radian) + CenterH);
			newW = (int)((i - CenterH)*sin(Radian)
				+ (j - CenterW)*sin(Radian) + CenterW);

			if (newH < 0 || newH >= m_height) {
				// 회전된 좌표가 출력 영상을 위한 배열 값을 넘어갈 때
				Value = 0;
			}
			else if (newW < 0 || newW >= m_width) {
				// 회전된 좌표가 출력 영상을 위한 배열 값을 넘어갈 때
				Value = 0;
			}
			else {
				Value = m_tempImage[newH][newW];
			}
			tempArray[i][j] = Value;
		}
	}
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i* m_Re_width + j]
				= (unsigned char)tempArray[i][j];
		}
	}

	delete[] m_tempImage;
	delete[] tempArray;


}


void CImageProcessDoc::OnDiffOperatorHor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j;
	double DiffHorMask[3][3]
		= { {0., -1., 0.}, {0., 1., 0.}, {0., 0., 0.} };
	// 수평 필터 선택

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];

	m_tempImage = OnMaskProcess(m_InputImage, DiffHorMask);
	// m_tempImage = OnScale(m_tempImage, m_Re_height, m_Re_width);
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255.)
				m_tempImage[i][j] = 255.;
			if (m_tempImage[i][j] < 0.)
				m_tempImage[i][j] = 0.;
		}
	}

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i* m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}


}


void CImageProcessDoc::OnHomogenOperator()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, n, m;
	double max, **tempOutputImage;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];

	m_tempImage = Image2DMem(m_height + 2, m_width + 2);
	tempOutputImage = Image2DMem(m_Re_height, m_Re_width);

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			m_tempImage[i + 1][j + 1] = (double)m_InputImage[i * m_width + j];
		}
	}

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			max = 0.0; // 블록이 이동할 때마다 최대값 초기화
			for (n = 0; n < 3; n++) {
				for (m = 0; m < 3; m++) {
					if (DoubleABS(m_tempImage[i + 1][j + 1] -
						m_tempImage[i + n][j + m]) >= max)
						// 블록의 가운데 값 - 블록의 주변 픽셀 값의 절대 값
						// 중에서 최대값을 찾는다.

						max = DoubleABS(m_tempImage[i + 1]
							[j + 1] - m_tempImage[i + n][j + m]);
				}
			}
			tempOutputImage[i][j] = max; // 찾은 최대값을 출력 값으로 지정
		}
	}

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (tempOutputImage[i][j] > 255.)
				tempOutputImage[i][j] = 255.;
			if (tempOutputImage[i][j] < 0.)
				tempOutputImage[i][j] = 0.;
		}
	}

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i* m_Re_width + j]
				= (unsigned char)tempOutputImage[i][j];
		}
	}

}


double CImageProcessDoc::DoubleABS(double X)
{
	// TODO: 여기에 구현 코드 추가.
	if (X >= 0)
		return X;
	else
		return -X;

}


void CImageProcessDoc::OnNearest()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j;
	int ZoomRate = 2; // 영상 확대 배율
	double **tempArray;

	m_Re_height = int(ZoomRate*m_height); // 확대된 영상의 높이
	m_Re_width = int(ZoomRate*m_width); // 확대된 영상의 너비
	m_Re_size = m_Re_height * m_Re_width;

	m_tempImage = Image2DMem(m_height, m_width);
	tempArray = Image2DMem(m_Re_height, m_Re_width);

	m_OutputImage = new unsigned char[m_Re_size];

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			m_tempImage[i][j] = (double)m_InputImage[i*m_width + j];
		}
	}

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			tempArray[i][j] = m_tempImage[i / ZoomRate][j / ZoomRate];
			// 이웃한 화소를 이용한 보간
		}
	}

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			m_OutputImage[i* m_Re_width + j] = (unsigned char)tempArray[i][j];
		}
	}

}


void CImageProcessDoc::OnBilinear()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, point, i_H, i_W;
	unsigned char newValue;
	double ZoomRate = 2.0, r_H, r_W, s_H, s_W;
	double C1, C2, C3, C4;

	m_Re_height = (int)(m_height * ZoomRate); // 확대된 영상의 높이
	m_Re_width = (int)(m_width * ZoomRate); // 확대된 영상의 너비
	m_Re_size = m_Re_height * m_Re_width;

	m_tempImage = Image2DMem(m_height, m_width);
	m_OutputImage = new unsigned char[m_Re_size];

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			m_tempImage[i][j] = (double)m_InputImage[i*m_width + j];
		}
	}
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			r_H = i / ZoomRate;
			r_W = j / ZoomRate;

			i_H = (int)floor(r_H);
			i_W = (int)floor(r_W);

			s_H = r_H - i_H;
			s_W = r_W - i_W;

			if (i_H < 0 || i_H >= (m_height - 1) || i_W < 0
				|| i_W >= (m_width - 1))
			{
				point = i * m_Re_width + j;
				m_OutputImage[point] = 255;
			}

			else
			{
				C1 = (double)m_tempImage[i_H][i_W];
				C2 = (double)m_tempImage[i_H][i_W + 1];
				C3 = (double)m_tempImage[i_H + 1][i_W + 1];
				C4 = (double)m_tempImage[i_H + 1][i_W];

				newValue = (unsigned char)(C1*(1 - s_H)*(1 - s_W)
					+ C2 * s_W*(1 - s_H) + C3 * s_W*s_H + C4 * (1 - s_W)*s_H);
				point = i * m_Re_width + j;
				m_OutputImage[point] = newValue;
			}
		}
	}


}


void CImageProcessDoc::OnMedianSub()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, n, m, M = 2, index = 0; // M = 서브 샘플링 비율
	double *Mask, Value;

	Mask = new    double[M*M]; // 마스크의 크기 결정

	m_Re_height = (m_height + 1) / M;
	m_Re_width = (m_width + 1) / M;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];
	m_tempImage = Image2DMem(m_height + 1, m_width + 1);

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			m_tempImage[i][j] = (double)m_InputImage[i*m_width + j];
		}
	}
	for (i = 0; i < m_height - 1; i = i + M) {
		for (j = 0; j < m_width - 1; j = j + M) {
			for (n = 0; n < M; n++) {
				for (m = 0; m < M; m++) {
					Mask[n*M + m] = m_tempImage[i + n][j + m];
					// 입력 영상을 블록으로 잘라 마스크 배열에 저장
				}
			}
			OnBubbleSort(Mask, M*M); // 마스크에 저장된 값을 정렬
			Value = Mask[(int)(M*M / 2)]; // 정렬된 값 중 가운데 값을 선택
			m_OutputImage[index] = (unsigned char)Value;
			// 가운데 값을 출력
			index++;
		}
	}
}

void CImageProcessDoc::OnSwap(double *a, double *b)
{
	// 데이터 교환 함수
	double temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void CImageProcessDoc::OnBubbleSort(double *A, int MAX)
{
	// 데이터의 정렬을 처리하는 함수
	int i, j;
	for (i = 0; i < MAX; i++) {
		for (j = 0; j < MAX - 1; j++) {
			if (A[j] > A[j + 1]) {
				OnSwap(&A[j], &A[j + 1]);
			}
		}
	}
}




void CImageProcessDoc::OnMeanSub()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, n, m, M = 3, index = 0, k; // M = 서브 샘플링 비율
	double *Mask, Value, Sum = 0.0;

	Mask = new double[M*M];

	m_Re_height = (m_height + 1) / M;
	m_Re_width = (m_width + 1) / M;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];
	m_tempImage = Image2DMem(m_height + 1, m_width + 1);

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			m_tempImage[i][j] = (double)m_InputImage[i*m_width + j];
		}
	}
	for (i = 0; i < m_height - 1; i = i + M) {
		for (j = 0; j < m_width - 1; j = j + M) {
			for (n = 0; n < M; n++) {
				for (m = 0; m < M; m++) {
					Mask[n*M + m] = m_tempImage[i + n][j + m];
				}
			}
			for (k = 0; k < M*M; k++)
				Sum = Sum + Mask[k];
			// 마스크에 저장된 값을 누적
			Value = (Sum / (M*M)); // 평균을 계산
			m_OutputImage[index] = (unsigned char)Value;
			// 평균값을 출력
			index++;
			Sum = 0.0;
		}
	}
}


void CImageProcessDoc::OnFrameSum()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFile File;
	CFileDialog OpenDlg(TRUE);

	int i;
	unsigned char *temp;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (OpenDlg.DoModal() == IDOK) {
		File.Open(OpenDlg.GetFileName(), CFile::modeRead);
		// 덧셈연산을 수행할 새로운 영상을 얻기 위해 
		// 열기 대화상자를 이용해 영상을 입력
		if (File.GetLength() == (unsigned)m_width * m_height) {
			temp = new unsigned char[m_size];
			// 입력 값 저장을 위한 배열 선언

			File.Read(temp, m_size); // 선택된 파일을 읽어 배열에 저장
			File.Close();

			// 프레임 간에 픽셀 대 픽셀로 덧셈연산 실행
			for (i = 0; i < m_size; i++) {
				if (m_InputImage[i] + temp[i] > 255)
					m_OutputImage[i] = 255;
				else
					m_OutputImage[i] = m_InputImage[i] + temp[i];
			}
		}
		else {
			AfxMessageBox(L"Image size not matched");
			//영상의 크기가 다를 때는 처리하지 않음
			return;
		}
	}

}


void CImageProcessDoc::OnFrameSubtraction()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFile File;
	CFileDialog OpenDlg(TRUE);

	int i;
	unsigned char *temp;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (OpenDlg.DoModal() == IDOK) {
		File.Open(OpenDlg.GetFileName(), CFile::modeRead);
		if (File.GetLength() == (unsigned)m_width * m_height) {
			temp = new unsigned char[m_size];

			File.Read(temp, m_size);
			File.Close();

			// 프레임 간에 픽셀 대 픽셀로 뺄셈연산 실행
			for (i = 0; i < m_size; i++) {
				if (m_InputImage[i] - temp[i] < 0)
					m_OutputImage[i] = 0;
				else
					m_OutputImage[i] = m_InputImage[i] - temp[i];
			}
		}
		else {
			AfxMessageBox(L"Image size not matched");
			return;
		}
	}

}


void CImageProcessDoc::OnFrameMultiply()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFile File;
	CFileDialog OpenDlg(TRUE);

	int i;
	unsigned char *temp;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (OpenDlg.DoModal() == IDOK) {
		File.Open(OpenDlg.GetFileName(), CFile::modeRead);
		if (File.GetLength() == (unsigned)m_width * m_height) {
			temp = new unsigned char[m_size];
			File.Read(temp, m_size);
			File.Close();

			// 프레임 간에 픽셀 대 픽셀로 곱셈연산 실행
			for (i = 0; i < m_size; i++) {
				if (m_InputImage[i] * temp[i] > 255)
					m_OutputImage[i] = 255;
				else
					m_OutputImage[i] = m_InputImage[i] * temp[i];
			}
		}
		else {
			AfxMessageBox(L"Image size not matched");
			return;
		}
	}

}


void CImageProcessDoc::OnFrameDivision()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFile File;
	CFileDialog OpenDlg(TRUE);

	int i;
	unsigned char *temp;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	if (OpenDlg.DoModal() == IDOK) {
		File.Open(OpenDlg.GetFileName(), CFile::modeRead);

		if (File.GetLength() == (unsigned)m_width * m_height) {
			temp = new unsigned char[m_size];

			File.Read(temp, m_size);
			File.Close();
			// 프레임 간에 픽셀 대 픽셀로 덧셈연산 실행
			for (i = 0; i < m_size; i++) {
				if (m_InputImage[i] == 0)
					// 나뉘는 값이‘0’이면 출력은 영상에서의 최소값
					m_OutputImage[i] = 0;
				else if (temp[i] == 0)
					// 나누는 값이‘0’이면 출력은 영상에서의 최대값
					m_OutputImage[i] = 255;
				else
					m_OutputImage[i]
					= (unsigned char)(m_InputImage[i] / temp[i]);
			}
		}
		else {
			AfxMessageBox(L"Image size not matched");
			return;
		}
	}

}


void CImageProcessDoc::OnBinaryErosion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, n, m;
	double Mask[3][3] = { {255.,255.,255.},
						 {255.,255.,255.},
						 {255.,255.,255.} };
	// 침식연산을 위한 마스크
	double **tempInput, S = 0.0;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;
	m_OutputImage = new unsigned char[m_Re_size];

	tempInput = Image2DMem(m_height + 2, m_width + 2);

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			tempInput[i + 1][j + 1]
				= (double)m_InputImage[i * m_width + j];
		}
	}
	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			for (n = 0; n < 3; n++) {
				for (m = 0; m < 3; m++) {
					if (Mask[n][m] == tempInput[i + n][j + m]) {
						// 마스크와 같은 값이 있는지 조사
						S += 1.0;
					}
				}
			}
			if (S == 9.0)
				m_OutputImage[i * m_Re_width + j] = (unsigned char)255.0;
			// 값이 모두 일치하면 출력 값은 255
			else
				m_OutputImage[i * m_Re_width + j] = (unsigned char)0.0;
			// 모두 일치하지 않으면 출력 값은 0
			S = 0.0; // reset
		}
	}
	delete[] tempInput;

}


void CImageProcessDoc::OnBinaryDilation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, n, m;
	double Mask[3][3] = { {0., 0., 0.},{0., 0., 0.},{0., 0., 0.} };
	// 팽창 처리를 위한 마스크
	double **tempInput, S = 0.0;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	tempInput = Image2DMem(m_height + 2, m_width + 2);

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			tempInput[i + 1][j + 1] = (double)m_InputImage[i * m_width + j];
		}
	}

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			for (n = 0; n < 3; n++) {
				for (m = 0; m < 3; m++) {
					if (Mask[n][m] == tempInput[i + n][j + m]) {
						// 마스크와 같은 값이 있는지 조사
						S += 1.0;
					}
				}
			}
			if (S == 9.0)
				m_OutputImage[i * m_Re_width + j] = (unsigned char)0.0;
			// 모두 일치하면 출력 값은 0
			else
				m_OutputImage[i * m_Re_width + j] = (unsigned char)255.0;
			// 모두 일치하지 않으면 출력 값은 255
			S = 0.0;
		}
	}
	delete[] tempInput;
}


void CImageProcessDoc::OnGrayErosion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, n, m, h;
	double Mask[9], MIN = 10000.0; // MIN = 최소값
	double **tempInput, S = 0.0;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	tempInput = Image2DMem(m_height + 2, m_width + 2);

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			tempInput[i + 1][j + 1]
				= (double)m_InputImage[i * m_width + j];
		}
	}


	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			MIN = 10000.0; // reset
			for (n = 0; n < 3; n++) {
				for (m = 0; m < 3; m++) {
					Mask[n * 3 + m] = tempInput[i + n][j + m];
					// 3*3 크기의 입력 값을 마스크 배열에 저장
				}
			}
			for (h = 0; h < 9; h++) {
				if (Mask[h] < MIN) // 마스크에서 최소값을 구한다.
					MIN = Mask[h];
			}
			m_OutputImage[i * m_Re_width + j]
				= (unsigned char)MIN; // 최소값 출력
		}
	}
}


void CImageProcessDoc::OnGrayDilation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, n, m, h;
	double Mask[9], MAX = 0.0;
	double **tempInput, S = 0.0;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	tempInput = Image2DMem(m_height + 2, m_width + 2);

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			tempInput[i + 1][j + 1]
				= (double)m_InputImage[i * m_width + j];
		}
	}

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			MAX = 0.0;
			for (n = 0; n < 3; n++) {
				for (m = 0; m < 3; m++) {
					Mask[n * 3 + m] = tempInput[i + n][j + m];
				}
			}
			for (h = 0; h < 9; h++) { // 마스크에서 최대값 구함
				if (Mask[h] > MAX)
					MAX = Mask[h];
			}
			m_OutputImage[i * m_Re_width + j]
				= (unsigned char)MAX; // 최대값 출력
		}
	}

}


void CImageProcessDoc::OnLowPassFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j;
	double LPF[3][3] = { {1. / 9., 1. / 9., 1. / 9.},
						 {1. / 9., 1. / 9., 1. / 9.},
						 {1. / 9., 1. / 9., 1. / 9.} };
	//double LPF[3][3] = {{1./12.,1./12.,1./12.}, {1. / 12., 4. / 12., 1. / 12.}, { 1. / 12.,1. / 12.,1. / 12. } };
	//double LPF[3][3] = {{1./18.,1./18.,1./18.}, {1. / 18., 10. / 18., 1. / 18.}, { 1. / 18.,1. / 18.,1. / 18. }};
	// 저주파 필터 마스크
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	m_tempImage = OnMaskProcess(m_InputImage, LPF);
	// 입력 영상과 마스크를 이용한 회선 처리
	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			if (m_tempImage[i][j] > 255)
				// 회선 처리 결과 값이 0~255 사이의 값이 아닐 때 
				// 0보다 작으면 0을,
				// 255보다 크면 255를 출력
				m_OutputImage[i*   m_Re_width + j] = 255;
			else if (m_tempImage[i][j] < 0)
				m_OutputImage[i*   m_Re_width + j] = 0;
			else
				m_OutputImage[i*   m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}
}


void CImageProcessDoc::OnHighPassFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j;
	double HPF[3][3] = { {-1. / 9., -1. / 9., -1. / 9.}, {-1. / 9., 8 / 9., -1. / 9.}, {-1. / 9., -1. / 9., -1. / 9.} };
	//double HPF[3][3] = {{-1., -1., -1.}, {-1., 9., -1.}, { -1., -1., -1. } };
	// 고주파 필터 마스크
	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	m_tempImage = OnMaskProcess(m_InputImage, HPF);

	for (i = 0; i < m_Re_height; i++) {
		for (j = 0; j < m_Re_width; j++) {
			// 회선 처리 결과 값이 0~255 사이의 값이 아닐 때 
			// 0보다 작으면 0을,
			// 255보다 크면 255를 출력
			if (m_tempImage[i][j] > 255)
				m_OutputImage[i*   m_Re_width + j] = 255;
			else if (m_tempImage[i][j] < 0)
				m_OutputImage[i*   m_Re_width + j] = 0;
			else
				m_OutputImage[i*   m_Re_width + j]
				= (unsigned char)m_tempImage[i][j];
		}
	}
}


void CImageProcessDoc::OnMeanFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, n, m;

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	double **tempInputImage, **tempOutputImage, S = 0.0;

	tempInputImage = Image2DMem(m_height + 2, m_width + 2);
	tempOutputImage = Image2DMem(m_height, m_width);

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			tempInputImage[i + 1][j + 1]
				= (double)m_InputImage[i * m_width + j];
		}
	}

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			for (n = 0; n < 3; n++) {
				for (m = 0; m < 3; m++) {
					S += tempInputImage[i + n][j + m];
					// 입력 영상에서 3*3 크기의 배열 값을 누적
				}
			}
			m_OutputImage[i*   m_Re_width + j]
				= (unsigned char)(S / 9.); // 평균값 출력
			S = 0.0; // reset
		}
	}

}


void CImageProcessDoc::OnMedianFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, n, m, index = 0;
	double **tempInputImage, Mask[9];

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	tempInputImage = Image2DMem(m_height + 2, m_width + 2);

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			tempInputImage[i + 1][j + 1]
				= (double)m_InputImage[i * m_width + j];
		}
	}

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			for (n = 0; n < 3; n++) {
				for (m = 0; m < 3; m++) {
					Mask[n * 3 + m] = tempInputImage[i + n][j + m];
					// 3*3 크기 배열 값을 마스크 배열에 할당
				}
			}
			OnBubbleSort(Mask, 9); // 마스크 값을 크기순으로 정렬
			m_OutputImage[index] = (unsigned char)Mask[4];
			// 중간 값 출력
			index++; // 출력 배열의 좌표
		}
	}

}


void CImageProcessDoc::OnMinFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, n, m, index = 0;
	double **tempInputImage, Mask[9];

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	tempInputImage = Image2DMem(m_height + 2, m_width + 2);

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			tempInputImage[i + 1][j + 1]
				= (double)m_InputImage[i * m_width + j];
		}
	}

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			for (n = 0; n < 3; n++) {
				for (m = 0; m < 3; m++) {
					Mask[n * 3 + m] = tempInputImage[i + n][j + m];
					// 3*3 크기 배열 값을 마스크 배열에 할당
				}
			}
			OnBubbleSort(Mask, 9); // 마스크 값을 크기순으로 정렬
			m_OutputImage[index] = (unsigned char)Mask[0];
			// 최소값 정렬
			index++; // 출력 배열의 좌표
		}
	}
}


void CImageProcessDoc::OnMaxFilter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, n, m, index = 0;
	double **tempInputImage, Mask[9];

	m_Re_height = m_height;
	m_Re_width = m_width;
	m_Re_size = m_Re_height * m_Re_width;

	m_OutputImage = new unsigned char[m_Re_size];

	tempInputImage = Image2DMem(m_height + 2, m_width + 2);

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			tempInputImage[i + 1][j + 1]
				= (double)m_InputImage[i * m_width + j];
		}
	}

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			for (n = 0; n < 3; n++) {
				for (m = 0; m < 3; m++) {
					Mask[n * 3 + m] = tempInputImage[i + n][j + m];
					// 3*3 크기 배열 값을 마스크 배열에 할당
				}
			}
			OnBubbleSort(Mask, 9); // 마스크 배열 값을 크기순으로 정렬
			m_OutputImage[index] = (unsigned char)Mask[8];
			// 최대값 출력
			index++; // 출력 배열의 좌표
		}
	}
}


void CImageProcessDoc::OnFft2d()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, row, col, Log2N, Num;
	Complex *Data;

	unsigned char **temp;
	double Value, Absol;

	Num = m_width;
	Log2N = 0;

	while (Num >= 2) // 영상의 너비 계산
	{
		Num >>= 1;
		Log2N++;
	}

	m_tempImage = Image2DMem(m_height, m_width); // 기억 장소 할당
	Data = new Complex[m_width];

	m_FFT = new Complex *[m_height];
	// 주파수 영역 변환 영상을 저장하기 위한 배열
	temp = new unsigned char *[m_height];

	for (i = 0; i < m_height; i++) {
		m_FFT[i] = new Complex[m_width];
		temp[i] = new unsigned char[m_width];
	}
	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			Data[j].Re = (double)m_InputImage[i*m_width + j];
			// 입력의 한 행을 복사, 실수 성분 값은 영상의 값
			Data[j].Im = 0.0; // 복소 성분 값은 0
		}
		OnFft1d(Data, m_width, Log2N); // 1차원 FFT
		for (j = 0; j < m_width; j++) { // 결과 저장
			m_FFT[i][j].Re = Data[j].Re;
			m_FFT[i][j].Im = Data[j].Im;
		}
	}

	Num = m_height;
	Log2N = 0;

	while (Num >= 2) // 영상의 높이 계산
	{
		Num >>= 1;
		Log2N++;
	}

	Data = new Complex[m_height];
	for (i = 0; i < m_width; i++) {
		for (j = 0; j < m_height; j++) {
			Data[j].Re = m_FFT[j][i].Re; // 영상의 한 열을 복사
			Data[j].Im = m_FFT[j][i].Im;
		}

		OnFft1d(Data, m_height, Log2N); // 1차원 FFT

		for (j = 0; j < m_height; j++) { // 결과 저장
			m_FFT[j][i].Re = Data[j].Re;
			m_FFT[j][i].Im = Data[j].Im;
		}
	}
	// FFT 실행 결과를 영상으로 출력하기 위한 연산
	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			Value = sqrt((m_FFT[i][j].Re * m_FFT[i][j].Re) +
				(m_FFT[i][j].Im * m_FFT[i][j].Im));
			Absol = 20 * log(Value);

			if (Absol > 255.0)
				Absol = 255.0;
			if (Absol < 0.0)
				Absol = 0.0;
			m_tempImage[i][j] = Absol;
		}
	}
	// 셔플링 과정 : 영상을 4등분하고 분할된 영상을 상하 대칭 및 좌우 대칭
	for (i = 0; i < m_height; i += m_height / 2) {
		for (int j = 0; j < m_width; j += m_width / 2) {
			for (row = 0; row < m_height / 2; row++) {
				for (col = 0; col < m_width / 2; col++) {
					temp[(m_height / 2 - 1) - row + i][(m_width / 2 - 1) - col + j]
						= (unsigned char)m_tempImage[i + row][j + col];
				}
			}
		}
	}
	
	m_OutputImage = new unsigned char[m_size];//////////////////

	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			m_OutputImage[i*m_width + j] = temp[i][j];
		}
	}

	delete[] Data, **temp;
}

void CImageProcessDoc::OnFft1d(Complex * X, int N, int Log2N)
{
	// TODO: 여기에 구현 코드 추가.
	   // 1차원 fft를 위한 함수
	OnShuffle(X, N, Log2N); // 함수 호출
	OnButterfly(X, N, Log2N, 1); // 함수 호출
}


void CImageProcessDoc::OnShuffle(Complex * X, int N, int Log2N)
{
	// 입력 데이터의 순서를 바구기 위한 함수
	int i;
	Complex *temp;

	temp = new Complex[N];

	for (i = 0; i < N; i++) {
		temp[i].Re = X[OnReverseBitOrder(i, Log2N)].Re;
		temp[i].Im = X[OnReverseBitOrder(i, Log2N)].Im;
	}

	for (i = 0; i < N; i++) {
		X[i].Re = temp[i].Re;
		X[i].Im = temp[i].Im;
	}
	delete[] temp;

}


void CImageProcessDoc::OnButterfly(Complex * X, int N, int Log2N, int mode)
{
	// TODO: 여기에 구현 코드 추가.
	   // 나비(Butterfly) 구조를 위한 함수
	int i, j, k, m;

	int start;
	double Value;
	double PI = 3.14159265358979;

	Complex *Y, temp;

	Y = new Complex[N / 2];

	for (i = 0; i < Log2N; i++) {
		Value = pow(2, i + 1);

		if (mode == 1) {
			for (j = 0; j < (int)(Value / 2); j++) {
				Y[j].Re = cos(j*2.0*PI / Value);
				Y[j].Im = -sin(j*2.0*PI / Value);
			}
		}
		if (mode == 2) {
			for (j = 0; j < (int)(Value / 2); j++) {
				Y[j].Re = cos(j*2.0*PI / Value);
				Y[j].Im = sin(j*2.0*PI / Value);
			}
		}

		start = 0;

		for (k = 0; k < N / (int)Value; k++) {
			for (j = start; j < start + (int)(Value / 2); j++) {
				m = j + (int)(Value / 2);
				temp.Re = Y[j - start].Re * X[m].Re
					- Y[j - start].Im * X[m].Im;
				temp.Im = Y[j - start].Im * X[m].Re
					+ Y[j - start].Re * X[m].Im;

				X[m].Re = X[j].Re - temp.Re;
				X[m].Im = X[j].Im - temp.Im;

				X[j].Re = X[j].Re + temp.Re;
				X[j].Im = X[j].Im + temp.Im;
			}
			start = start + (int)Value;
		}
	}
	if (mode == 2) {
		for (i = 0; i < N; i++) {
			X[i].Re = X[i].Re / N;
			X[i].Im = X[i].Im / N;
		}
	}

	delete[] Y;
}


int CImageProcessDoc::OnReverseBitOrder(int index, int Log2N)
{
	// TODO: 여기에 구현 코드 추가.
	int i, X, Y;

	Y = 0;

	for (i = 0; i < Log2N; i++) {
		X = (index & (1 << i)) >> i;
		Y = (Y << 1) | X;
	}

	return Y;
}



void CImageProcessDoc::OnIfft2d()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, Num, Log2N;
	Complex *Data;

	Num = m_width;
	Log2N = 0;
	while (Num >= 2) // 주파수 변환된 영상의 너비 계산
	{
		Num >>= 1;
		Log2N++;
	}

	Data = new Complex[m_height];
	m_IFFT = new Complex *[m_height]; // 역변환된 영상을 위한 배열

	for (i = 0; i < m_height; i++) {
		m_IFFT[i] = new Complex[m_width];
	}
	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) { // 한 행을 복사
			Data[j].Re = m_FFT[i][j].Re;
			Data[j].Im = m_FFT[i][j].Im;
		}

		OnIfft1d(Data, m_width, Log2N); // 1차원 IFFT
		for (j = 0; j < m_width; j++) {
			m_IFFT[i][j].Re = Data[j].Re; // 결과 저장
			m_IFFT[i][j].Im = Data[j].Im;
		}
	}

	Num = m_height;
	Log2N = 0;
	while (Num >= 2) // 주파수 변환된 영상의 높이 계산
	{
		Num >>= 1;
		Log2N++;
	}

	Data = new Complex[m_height];
	for (i = 0; i < m_width; i++) {
		for (j = 0; j < m_height; j++) {
			Data[j].Re = m_IFFT[j][i].Re; // 한 열을 복사
			Data[j].Im = m_IFFT[j][i].Im;
		}

		OnIfft1d(Data, m_height, Log2N); // 1차원 IFFT

		for (j = 0; j < m_height; j++) {
			m_IFFT[j][i].Re = Data[j].Re; // 결과 저장
			m_IFFT[j][i].Im = Data[j].Im;
		}
	}

	for (i = 0; i < m_width; i++) {
		for (j = 0; j < m_height; j++) {
			m_OutputImage[i*m_width + j]
				= (unsigned char)m_IFFT[i][j].Re;
			// 결과 출력
		}
	}
	delete[] Data;
}


void CImageProcessDoc::OnIfft1d(Complex * X, int N, int Log2N)
{
	// TODO: 여기에 구현 코드 추가.
	OnShuffle(X, N, Log2N);
	OnButterfly(X, N, Log2N, 2);
}



void CImageProcessDoc::OnLpfFrequency()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, x, y;
	double temp, D, N;
	D = 32.0;
	N = 4.0;

	OnFft2d(); // 주파수 변환

	// 주파수 변환된 값에서 고주파 성분 제거
	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			x = i;
			y = j;
			if (x > m_height / 2)
				x = x - m_height;
			if (y > m_width / 2)
				y = y - m_width;

			temp = 1.0 / (1.0 + pow(sqrt((double)
				(x * x + y * y)) / D, 2 * N));

			m_FFT[i][j].Re = m_FFT[i][j].Re * temp;
			m_FFT[i][j].Im = m_FFT[i][j].Im * temp;
		}
	}

	OnIfft2d(); // 주파수 역변환
}


void CImageProcessDoc::OnHpfFrequency()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int i, j, x, y;
	double temp, D, N;
	D = 128.0;
	N = 4.0;

	OnFft2d(); // 주파수 변환
	// 주파수 변환된 값에서 저주파 성분 제거
	for (i = 0; i < m_height; i++) {
		for (j = 0; j < m_width; j++) {
			x = i;
			y = j;

			if (x > m_height / 2)
				x = x - m_height;
			if (y > m_width / 2)
				y = y - m_width;

			temp = 1.0 / (1.0 + pow(D / sqrt((double)
				(x * x + y * y)), 2 * N));

			m_FFT[i][j].Re = m_FFT[i][j].Re * temp;
			m_FFT[i][j].Im = m_FFT[i][j].Im * temp;
		}
	}
	OnIfft2d(); // 주파수 역변환

}
