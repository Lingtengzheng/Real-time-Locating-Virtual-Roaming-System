// myDLL.cpp : 定义 DLL 应用程序的导出函数。
//

#include <windows.h>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
using namespace cv;

int xv[3][20] = { 0 };
int yv[3][20] = { 0 };
int size[3][20] = { 0 };


int x[3];
int y[3];
int x2[3];
int y2[3];
int size2[3];

int x3[3];
int y3[3];
int size3[3];

int len[3] = { 0 };
int len2[3] = { 0 };



IplImage* gray = NULL;
IplImage* gray_smooth = NULL;
IplImage* gray_canny = NULL;
IplImage* img_color = NULL;
IplImage* img_rgb[3] = { 0, 0, 0 };
IplImage* gray_sobel = NULL;
IplImage* gray_sobel1 = NULL;
IplImage* gray_binary = NULL;
IplImage* gray_dif = NULL;

IplImage* first_gray = NULL;

extern "C" _declspec(dllexport) int begin()
{
	int i, j, k;
	unsigned char color1, color2, color3;

	//声明IplImage指针  
	IplImage* pFrame = NULL;
	IplImage* dst = NULL;

	//获取摄像头  
	CvCapture* pCapture = cvCreateCameraCapture(0);
	CvPoint pot;

	//创建窗口
	cvNamedWindow("video", 1);
	Sleep(700);
	pFrame = cvQueryFrame(pCapture);

	dst = cvCreateImage(cvGetSize(pFrame), IPL_DEPTH_8U, 3);
	/*gray_canny = cvCreateImage(cvGetSize(pFrame), IPL_DEPTH_8U, 1);
	gray = cvCreateImage(cvGetSize(pFrame), IPL_DEPTH_8U, 1);
	first_gray = cvCreateImage(cvGetSize(pFrame), IPL_DEPTH_8U, 1);
	gray_dif = cvCreateImage(cvGetSize(pFrame), IPL_DEPTH_8U, 1);

	gray_sobel1 = cvCreateImage(cvGetSize(pFrame), IPL_DEPTH_8U, 1);
	gray_smooth = cvCreateImage(cvGetSize(pFrame), IPL_DEPTH_8U, 1);
	gray_binary = cvCreateImage(cvGetSize(pFrame), IPL_DEPTH_8U, 1);
	gray_sobel = cvCreateImage(cvGetSize(pFrame), IPL_DEPTH_16S, 1);
	img_color = cvCreateImage(cvGetSize(pFrame), IPL_DEPTH_8U, 4);*/

	//memset(first_gray->imageData, 200, pFrame->height*pFrame->widthStep);
	//cvRectangle(first_gray, cvPoint(0, 0), cvPoint(pFrame->height, pFrame->width), cvScalar(200), -1, 8, 0);

	//cvCvtColor(pFrame, first_gray, CV_BGR2GRAY);


	/*for (i = 0; i < 3;i++)
	img_rgb[i] = cvCreateImage(cvGetSize(pFrame), IPL_DEPTH_8U, 1);


	gray_canny = cvCreateImage(cvGetSize(pFrame), IPL_DEPTH_8U, 1);*/




	//显示视   
	int * mask;

	mask = (int*)malloc(pFrame->width*pFrame->height * 4);

	memset(mask, 1, pFrame->width*pFrame->height * 4);





	int* addr;

	while (1)
	{
		pFrame = cvQueryFrame(pCapture);

		cvCvtColor(pFrame, dst, CV_BGR2HSV);
		/*cvCvtColor(pFrame, gray, CV_BGR2GRAY);


		cvAbsDiff(gray, first_gray, gray_dif);

		cvSmooth(gray, gray_smooth, CV_GAUSSIAN, 3, 0, 0, 0.0f);
		cvLaplace(gray_smooth, gray_canny,  3);

		//cvConvertScaleAbs(gray_canny, gray, 1.0f, 0.0f);

		cvSobel(gray_smooth, gray_sobel, 0, 1, 3);

		cvConvertScaleAbs(gray_sobel, gray_sobel1, 1.0f, 0.0f);

		cvSmooth(gray_canny, gray_smooth, CV_GAUSSIAN, 3, 0, 0, 0.0f);
		//cvCanny(gray_smooth, gray_sobel1, 0, 50, 3);
		cvThreshold(gray_dif, gray_binary, 80, 255, CV_THRESH_BINARY);
		//cvConvertScaleAbs(gray_sobel, gray_sobel1, 1.0f, 0.0f);*/
		if (!pFrame)break;

		int l = 0;

		for (l = 0; l < 3; l++)
		{
			x[l] = 0;
			y[l] = 0;

			x2[l] = 0;
			y2[l] = 0;
			size2[l] = 0;

			len[l] = 0;
			len2[l] = 10;
		}

		//cvRectangle(pFrame, cvPoint(0, 0), cvPoint(pFrame->width, pFrame->height), CV_RGB(0, 0, 0), -1);

		for (i = 0; i < pFrame->height; i++)
		{
			for (j = 0; j < pFrame->width; j++)
			{
				if (*(mask + i *pFrame->width + j))
				{
					*(mask + i *pFrame->width + j) = 0;
					color1 = (unsigned char)*(dst->imageData + i* dst->widthStep + j * 3);
					color2 = (unsigned char)*(dst->imageData + i* dst->widthStep + j * 3 + 1);
					color3 = (unsigned char)*(dst->imageData + i* dst->widthStep + j * 3 + 2);

					pot.x = j;
					pot.y = i;
					//if (i==0 && j==0)
					//printf("%d-%d-%d\n", color1, color2, color3);
					if ((color1 < 4 || color1>170) && color2 > 150 && color3 > 50)
					{
						cvCircle(pFrame, pot, 3, CV_RGB(255, 0, 0), 1, 8, 3);

						x[0] += j;
						y[0] += i;
						len[0]++;

					}
					else if ((color1 < 80 && color1>39) && color2 > 67 && color2 < 220 && color3 > 40)
					{
					cvCircle(pFrame, pot, 3, CV_RGB(0, 255, 0), 1, 8, 3);

					x[1] += j;
					y[1] += i;
					len[1]++;

					}
					/*else if ((color1 > 110 && color1<130) && color2 > 140 && color2 < 240 && color3 > 30)
					{
					cvCircle(pFrame, pot, 3, CV_RGB(0, 0, 255), 1, 8, 3);

					x[2] += j;
					y[2] += i;
					len[2]++;

					}*/

					/*
					39 - 50
					97 - 141
					114 - 256

					110 - 120
					140 - 200
					70 - 200
					*/

				}

			}
		}

		for (l = 0; l < 3; l++)
		{
			if (len[l] < 20)
			{
				memset(mask, 1, pFrame->width*pFrame->height * 4);

			}
			if (len[l] > 0)
			{
				x[l] = x[l] / len[l];
				y[l] = y[l] / len[l];
			}

			for (i = 0; i < len2[l] - 1; i++)
			{
				xv[l][i] = xv[l][i + 1];
				yv[l][i] = yv[l][i + 1];
				size[l][i] = size[l][i + 1];

				x2[l] += xv[l][i];
				y2[l] += yv[l][i];
				size2[l] += size[l][i];

			}

			if (abs(xv[l][i] - x[l]) < 5 && abs(yv[l][i] - y[l]) < 5)
			{

			}
			else
			{
				xv[l][i] = x[l];
				yv[l][i] = y[l];
				size[l][i] = len[l];
			}



			x2[l] += xv[l][i];
			y2[l] += yv[l][i];

			x2[l] /= len2[l];
			y2[l] /= len2[l];
			size2[l] /= len2[l];

			x3[l] = x2[l];
			y3[l] = y2[l];
			size3[l] = size2[l];



			//printf("%d-%d-%d\n", x, y, len);

			pot.x = pFrame->width - x2[l];
			pot.y = y2[l];

			/*if (l == 0)
			cvCircle(pFrame, pot, (int)(0.005 * (float)size2[l])<2 ? 2 : (int)(0.005 * (float)size2[l]), CV_RGB(255, 0, 0), -1, 8, 3);
			else if (l == 1)
			cvCircle(pFrame, pot, (int)(0.005 * (float)size2[l])<2 ? 2 : (int)(0.005 * (float)size2[l]), CV_RGB(0, 255, 0), -1, 8, 3);
			else if (l == 2)
			cvCircle(pFrame, pot, (int)(0.005 * (float)size2[l])<2 ? 2 : (int)(0.005 * (float)size2[l]), CV_RGB(0, 0, 255), -1, 8, 3);
			*/
			i = 200;

			if (l == 0)
				cvCircle(pFrame, pot, 20, CV_RGB(255, 0, 0), -1, 8, 3);
			else if (l == 1)
				cvCircle(pFrame, pot, 20, CV_RGB(0, 255, 0), -1, 8, 3);
			else if (l == 2)
				cvCircle(pFrame, pot, 20, CV_RGB(0, 0, 255), -1, 8, 3);

			for (j = 0; j <i; j++)
			{
				for (k = 0; k < i; k++)
				{
					//DrawPoint((float)((int)CVcolors[i].center_x - follow_r / 2 + k)*scale + 660, (float)((int)CVcolors[i].center_y - follow_r / 2 + j)*scale + 20, 2.0f);
					if (y[l] - i / 2 + j > -1 && y[l] - i / 2 + j <pFrame->height && x[l] - i / 2 + k>-1 && x[l] - i / 2 + k < pFrame->width)
					{
						addr = mask + (y[l] - i / 2 + j)*pFrame->width + (x[l] - i / 2 + k);
						*(addr) = 1;
					}
				}

			}
		}



		/*for (k = 0; k < i; k++)
		{
		memset(mask + (y - i / 2)*pFrame->width  + (x - i / 2), 1, i );

		}*/

		//cvShowImage("video_gray", gray);
		/*cvShowImage("video_gray_2", gray_binary);
		cvShowImage("video_gray_smooth", gray_dif);
		cvShowImage("video_gray_sobel", gray_sobel1);
		cvShowImage("video_gray_canny", gray_canny);*/


		cvShowImage("video", pFrame);
		char c = cvWaitKey(1);
		if (c == 27)break;
	}
	cvReleaseCapture(&pCapture);
	cvDestroyWindow("video");
	/*cvDestroyWindow("video_gray_smooth");
	cvDestroyWindow("video_gray_canny");
	cvDestroyWindow("video_gray_sobel");*/
	free(mask);
	return 0;
}

extern "C" _declspec(dllexport) int getX(int i)
{
	return x3[i];
}

extern "C" _declspec(dllexport) int getY(int i)
{
	return y3[i];
}

extern "C"_declspec(dllexport) int getSize(int i)
{
	return size3[i];

}