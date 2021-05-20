#pragma once

#include <memory.h>
#include <assert.h>
#include "IppConvert.h"
#include "RGBBYTE.h"


template<typename T>
class IppImage
{
protected:
	int width;  // 영상의 가로 크기(픽셀 단위)
	int height; // 영상의 세로 크기(픽셀 단위)
	T** pixels; // 픽셀 데이터

public:
	// 생성자와 소멸자
	IppImage();
	IppImage(int w, int h);
	IppImage(const IppImage<T>& img);
	~IppImage();

	// 이미지 생성과 소멸
	void CreateImage(int w, int h);
	void DestroyImage();

	// 픽셀 값 접근
	T* GetPixels() const {
		if (pixels) return pixels[0];
		else return NULL;
	}
	T** GetPixels2D() const { return pixels; }

	// 치환 연산자 재정의
	IppImage<T>& operator=(const IppImage<T>& img);
	template<typename U> IppImage<T>& operator=(const IppImage<U>& img);

	// 픽셀 값 설정
	template<typename U> void Convert(const IppImage<U>& img, bool use_limit = false);
	void    Convert(const IppImage<RGBBYTE> & img);

	// 영상 정보 반환
	int GetWidth()   const { return width; }
	int GetHeight()  const { return height; }
	int GetSize()    const { return width * height; }
	bool IsValid()   const { return (pixels != NULL); }
};

typedef IppImage<BYTE>     IppByteImage;
typedef IppImage<int>      IppIntImage;
typedef IppImage<float>    IppFloatImage;
typedef IppImage<double>   IppDoubleImage;
typedef IppImage<RGBBYTE>  IppRgbImage;


template<typename T>
IppImage<T>::IppImage()
	:width(0), height(0), pixels(NULL)
{
}

template<typename T>
IppImage<T>::IppImage(int w, int h)
	: width(w), height(h), pixels(NULL)
{
	pixels = new T*[sizeof(T*) * height];
	pixels[0] = new T[sizeof(T) * width * height];

	for (int i = 1; i < height; i++)
		pixels[i] = pixels[i - 1] + width;

	memset(pixels[0], 0, sizeof(T) * width * height); // 픽셀 값 초기화
}

template<class T>
IppImage<T>::IppImage(const IppImage<T>& img)
	: width(img.width), height(img.height), pixels(NULL)
{
	if (img.IsValid()) {
		pixels = new T*[sizeof(T*) * height];
		pixels[0] = new T[sizeof(T) * width * height];

		for (int i = 1; i < height; i++)
			pixels[i] = pixels[i - 1] + width;

		memcpy(pixels[0], img.pixels[0], sizeof(T) * width * height); // 픽셀 값 복사
	}
}

template<typename T>
IppImage<T>::~IppImage()
{
	if (pixels != NULL) {
		delete[] pixels[0];
		delete[] pixels;
	}
}


template<typename T>
void IppImage<T>::CreateImage(int w, int h)
{
	DestroyImage();

	width = w;
	height = h;

	pixels = new T*[sizeof(T*) * height];
	pixels[0] = new T[sizeof(T) * width * height];

	for (int i = 1; i < height; i++)
		pixels[i] = pixels[i - 1] + width;

	memset(pixels[0], 0, sizeof(T) * width * height); // 픽셀 값 초기화
}

template<typename T>
void IppImage<T>::DestroyImage()
{
	if (pixels != NULL) {
		delete[] pixels[0];
		delete[] pixels;
		pixels = NULL;
	}

	width = height = 0;
}


template<typename T>
IppImage<T>& IppImage<T>::operator=(const IppImage<T>& img)
{
	if (this == &img)
		return *this;

	CreateImage(img.width, img.height);
	memcpy(pixels[0], img.pixels[0], sizeof(T) * width * height);

	return *this;
}

template<typename T> template<typename U>
void IppImage<T>::Convert(const IppImage<U>& img, bool use_limit)
{
	CreateImage(img.GetWidth(), img.GetHeight());

	int size = GetSize();
	T* p1 = GetPixels();
	U* p2 = img.GetPixels();

	if (use_limit)
		for (int i = 0; i < size; i++)
			p1[i] = static_cast<T>(limit(p2[i]));

	else
		for (int i = 0; i < size; i++)
			p1[i] = static_cast<T>(p2[i]);
}

template<typename T>
inline T limit(const T& value)
{
	return ((value > 255) ? 255 : ((value < 0) ? 0 : value));
}


void IppDibToImage(IppDib& dib, IppByteImage& img)
{
	assert(dib.IsValid());
	assert(dib.GetBitCount() == 8);

	int w = dib.GetWidth();
	int h = dib.GetHeight();
	int ws = (w + 3) & ~3;
	BYTE* pDIBits = dib.GetDIBitsAddr();

	img.CreateImage(w, h);
	BYTE** pixels = img.GetPixels2D();

	for (int i = 0; i < h; i++)
	{
		memcpy(pixels[i], &pDIBits[(h - 1 - i) * ws], w);
	}
}

void IppDibToImage(IppDib& dib, IppRgbImage& img)
{
	assert(dib.IsValid());
	assert(dib.GetBitCount() == 24);

	int w = dib.GetWidth();
	int h = dib.GetHeight();
	int ws = (w * 3 + 3) & ~3;
	BYTE* pDIBits = dib.GetDIBitsAddr();

	img.CreateImage(w, h);
	RGBBYTE** pixels = img.GetPixels2D();

	for (int i = 0; i < h; i++)
	{
		memcpy(pixels[i], &pDIBits[(h - 1 - i) * ws], w * 3);
	}
}


void IppImageToDib(IppByteImage& img, IppDib& dib)
{
	assert(img.IsValid());

	int w = img.GetWidth();
	int h = img.GetHeight();
	int ws = (w + 3) & ~3;
	BYTE** pixels = img.GetPixels2D();

	dib.CreateGrayBitmap(w, h);
	BYTE* pDIBits = dib.GetDIBitsAddr();

	for (int i = 0; i < h; i++)
	{
		memcpy(&pDIBits[(h - 1 - i) * ws], pixels[i], w);
	}
}

void IppImageToDib(IppFloatImage& img, IppDib& dib)
{
	assert(img.IsValid());

	int w = img.GetWidth();
	int h = img.GetHeight();
	int ws = (w + 3) & ~3;
	float** pixels = img.GetPixels2D();

	dib.CreateGrayBitmap(w, h);
	BYTE* pDIBits = dib.GetDIBitsAddr();

	int i, j;
	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++)
		{
			pDIBits[(h - 1 - j) * ws + i] = static_cast<BYTE>(limit(pixels[j][i] + 0.5f));
		}
}

void IppImageToDib(IppRgbImage& img, IppDib& dib)
{
	assert(img.IsValid());

	int w = img.GetWidth();
	int h = img.GetHeight();
	int ws = (w * 3 + 3) & ~3;
	RGBBYTE** pixels = img.GetPixels2D();

	dib.CreateRgbBitmap(w, h);
	BYTE* pDIBits = dib.GetDIBitsAddr();

	for (int i = 0; i < h; i++)
	{
		memcpy(&pDIBits[(h - 1 - i) * ws], pixels[i], w * 3);
	}
}