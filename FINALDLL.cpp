#include "stdio.h"
# include "math.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <windows.h>



using namespace std;


HINSTANCE hdll1;
HINSTANCE hdll2;
typedef void(__stdcall *Dllfun1)();
Dllfun1 Begin;

typedef int(__stdcall *Dllfun2)(int i);
Dllfun2 getX;

typedef int(__stdcall *Dllfun3)(int i);
Dllfun3 getY;

typedef int(__stdcall *Dllfun4)(int i);
Dllfun4 getSize;

typedef void(__stdcall *Dllfun5)();
Dllfun1 Begin1;

typedef int(__stdcall *Dllfun6)(int i);
Dllfun2 getX1;

typedef int(__stdcall *Dllfun7)(int i);
Dllfun3 getY1;

typedef int(__stdcall *Dllfun8)(int i);
Dllfun4 getSize1;

typedef int(__stdcall *Dllfun9)(int i);
Dllfun9 getXL;

typedef int(__stdcall *Dllfun10)(int i);
Dllfun10 getYL;




# define stateNum 6
const int measureNum = 3;
CvKalman* kalman[3]; //= cvCreateKalman(stateNum, measureNum, 0);//state(x,y,detaX,detaY)   
CvMat* process_noise[3];// = cvCreateMat(stateNum, 1, CV_32FC1);
CvMat* measurement[3];// = cvCreateMat(measureNum, 1, CV_32FC1);//measurement(x,y)   
CvRNG rng[3];// = cvRNG(-1);
float A[stateNum][stateNum] = {//transition matrix   
	1, 0, 0, 1, 0, 0,
	0, 1, 0, 0, 1, 0,
	0, 0, 1, 0, 0, 1,
	0, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 1
};

/*extern int getX(int i);
extern int getY(int i);
extern int getX1(int i);
extern int getY1(int i);*/

typedef struct point
{
	float x;
	float y;
	float z;

}point;

extern "C" _declspec(dllexport) void kalman_cal();

float a, b, X1, Y1, X2, Y2, XL, YL, s;    /*a为摄像头中心高度，b为所要测试范围的长宽，(X1 Y1)为camera1识别的像素坐标，(X2 Y2)为camera2识别的像素坐标*/


vector<point> VectorOfPoint;


DWORD WINAPI cam1(LPVOID lpparentet)
{
	Begin();

	return 0;
}

DWORD WINAPI cam2(LPVOID lpparentet)
{
	Begin1();

	return 0;
}


extern "C" _declspec(dllexport) void run()
{



	hdll1 = LoadLibrary(L"DLL.dll");
	hdll2 = LoadLibrary(L"DLL1.dll");

	Begin = (Dllfun1)GetProcAddress(hdll1, "begin");
	getX = (Dllfun2)GetProcAddress(hdll1, "getX");
	getY = (Dllfun3)GetProcAddress(hdll1, "getY");
	getSize = (Dllfun4)GetProcAddress(hdll1, "getSize");
	getXL = (Dllfun9)GetProcAddress(hdll1, "getXL");
	getYL = (Dllfun10)GetProcAddress(hdll1, "getYL");
	CreateThread(NULL, 0, cam1, NULL, 0, NULL);

	Begin1 = (Dllfun5)GetProcAddress(hdll2, "begin1");
	getX1 = (Dllfun6)GetProcAddress(hdll2, "getX1");
	getY1 = (Dllfun7)GetProcAddress(hdll2, "getY1");
	getSize1 = (Dllfun8)GetProcAddress(hdll2, "getSize1");

	CreateThread(NULL, 0, cam2, NULL, 0, NULL);




	int i;
	point temp;


	for (i = 0; i < 3; i++)
	{
		kalman[i] = cvCreateKalman(stateNum, measureNum, 0);//state(x,y,detaX,detaY)   
		process_noise[i] = cvCreateMat(stateNum, 1, CV_32FC1);
		measurement[i] = cvCreateMat(measureNum, 1, CV_32FC1);//measurement(x,y)  
		memcpy(kalman[i]->transition_matrix->data.fl, A, sizeof(A));
		cvSetIdentity(kalman[i]->measurement_matrix, cvRealScalar(1.0f));
		cvSetIdentity(kalman[i]->process_noise_cov, cvRealScalar(1e-5));
		cvSetIdentity(kalman[i]->measurement_noise_cov, cvRealScalar(1e-1));
		cvSetIdentity(kalman[i]->error_cov_post, cvRealScalar(1));
		rng[i] = cvRNG(-1);
		cvRandArr(&rng[i], kalman[i]->state_post, CV_RAND_UNI, cvRealScalar(0), cvRealScalar(480));
		VectorOfPoint.push_back(temp);

	}

	while (1)
	{
		//scanf_s("%f  %f  %f  %f  %f  %f", &a, &b, &X1, &Y1, &X2, &Y2);

		a = 180.0;
		b = 200.0;

		X1 = (float)getX(0);
		Y1 = (float)getY(0);
		XL = (float)getX(1);
		YL = (float)getY(1);

		X2 = (float)getX1(0);
		Y2 = (float)getY1(0);

		//printf("%f %f\n", X1, Y1);


		s = ((b / 2) / 0.3404) + b / 2;
		/*c1(0, a, 0);camera1 position
		c2(-s, a, s);camera2 position
		A((3 * (X1 - 240) / 40)+s, (3 * (180 + Y1) / 40) + a, 70.5+s);A point is on the camera1's screen
		B(70.5 + s, (3 * (180 + Y1) / 40) + a, -(3 * (X1 - 240) / 40) - s);*/
		float AX, AY, AZ, BX, BY, BZ, C1X, C1Y, C1Z, C2X, C2Y, C2Z;
		C1X = 0;
		C1Y = a;
		C1Z = 0;
		C2X = -s;
		C2Y = a;
		C2Z = s;
		AX = (3 * (X1 - 320) / 40);
		AY = (3 * (240 - Y1) / 40);
		AZ = 59.31;//70.5;
		BX = 42.25;// 70.5;
		BY = (3 * (240 - Y1) / 40);
		BZ = -(3 * (X2 - 320) / 40);
		//printf("(%f %f %f) (%f %f %f) \n", AX, AY, AZ, BX, BY, BZ);
		float F1X, F1Y, F1Z, F2X, F2Y, F2Z, NX, NY, NZ, MX, MY, MZ, MX2, MY2, MZ2, PX, PY, PZ, G, DF2X, DF2Y, DF2Z, t, TX, TY, TZ;
		F1X = AX;// -C1X;
		F1Y = AY;// - C1Y;
		F1Z = AZ;// - C1Z;
		//printf("(%f %f %f) \n", F1X, F1Y, F1Z);
		F2X = BX;// - C2X;
		F2Y = BY;// - C2Y;
		F2Z = BZ;// - C2Z;
		//printf("(%f %f %f) \n", F2X, F2Y, F2Z);

		printf("\n");
		DF2X = F2X / (sqrt(F2X*F2X + F2Y*F2Y + F2Z*F2Z));
		DF2Y = F2Y / (sqrt(F2X*F2X + F2Y*F2Y + F2Z*F2Z));
		DF2Z = F2Z / (sqrt(F2X*F2X + F2Y*F2Y + F2Z*F2Z));
		NX = AZ*BY - AY*BZ;
		NY = AX*BZ - AZ*BX;
		NZ = AY*BX - AX*BY;
		MX = NZ*F1Y - NY*F1Z;
		MY = NX*F1Z - NZ*F1X;
		MZ = NY*F1X - NX*F1Y;
		//printf("(%f %f %f) \n", MX, MY, MZ);
		//printf("(%f %f %f) \n", MX, MY, MZ);


		MX2 = MX / (sqrt(MX*MX + MY*MY + MZ*MZ));
		MY2 = MY / (sqrt(MX*MX + MY*MY + MZ*MZ));
		MZ2 = MZ / (sqrt(MX*MX + MY*MY + MZ*MZ));
		//printf("(%f %f %f) \n", MX2, MY2, MZ2);

		float PD1X, PD1Y, PD1Z, PD2X, PD2Y, PD2Z, PD3X, PD3Y, PD3Z, G1, G2, G3;
		t = 1000;
		PD1X = C2X;
		PD1Y = C2Y;
		PD1Z = C2Z;
		PX = PD1X + t*DF2X;
		PY = PD1Y + t*DF2Y;
		PZ = PD1Z + t*DF2Z;
		PD2X = PX;
		PD2Y = PY;
		PD2Z = PZ;

		//if (AX > 0)
		{

			//printf("%f ,%f, %f\n", PD1X, PD1Y, PD1Z);
			//printf("%f ,%f, %f\n", F1X, F1Y, F1Z);

			//	printf("\n");
			//printf("%f ,%f, %f\n", DF2X, DF2Y, DF2Z);
			while (sqrt((PD1X - PD2X)*(PD1X - PD2X) + (PD1Y - PD2Y)*(PD1Y - PD2Y) + (PD1Z - PD2Z)*(PD1Z - PD2Z)) > 0.1)
			{

				PD3X = (PD1X + PD2X) / 2;
				PD3Y = (PD1Y + PD2Y) / 2;
				PD3Z = (PD1Z + PD2Z) / 2;

				//printf("%f ,%f, %f\n", PD1X, PD1Y, PD1Z);


				G3 = MX2*(PD3X - C1X) + MY2*(PD3Y - C1Y) + MZ2*(PD3Z - C1Z);
				G1 = MX2*(PD1X - C1X) + MY2*(PD1Y - C1Y) + MZ2*(PD1Z - C1Z);
				G2 = MX2*(PD2X - C1X) + MY2*(PD2Y - C1Y) + MZ2*(PD2Z - C1Z);

				if ((G3 < 0 && G1 < 0) || (G3>0 && G1>0))
				{
					PD1X = PD3X;
					PD1Y = PD3Y;
					PD1Z = PD3Z;
				}
				else
				{
					PD2X = PD3X;
					PD2Y = PD3Y;
					PD2Z = PD3Z;
				}

				/*if (G3 > 0 && G1 > 0 && G2<0)
				{
				PD1X = PD3X;
				PD1Y = PD3Y;
				PD1Z = PD3Z;

				}
				else if (G3 < 0 && G2 < 0 && G1>0)
				{
				PD2X = PD3X;
				PD2Y = PD3Y;
				PD2Z = PD3Z;
				}
				else if (G3 = 0)
				{
				break;
				}
				else
				{
				printf("错误");
				}*/

				TX = PD3X;
				TY = PD3Y;
				TZ = PD3Z;

			}
			VectorOfPoint[0].x = TX;
			VectorOfPoint[0].y = TY;
			VectorOfPoint[0].z = TZ;

			kalman_cal();


			Sleep(10);
		}
		/*if (AX<=0)
		{
		while (sqrt((PD1X - PD2X)*(PD1X - PD2X) + (PD1Y - PD2Y)*(PD1Y - PD2Y) + (PD1Z - PD2Z)*(PD1Z - PD2Z)) > 0.1)
		{

		PD3X = (PD1X + PD2X) / 2;
		PD3Y = (PD1Y + PD2Y) / 2;
		PD3Z = (PD1Z + PD2Z) / 2;
		G3 = MX*(PD3X - C1X) + MY*(PD3Y - C1Y) + MZ*(PD3Z - C1Z);
		G1 = MX*(PD1X - C1X) + MY*(PD1Y - C1Y) + MZ*(PD1Z - C1Z);
		G2 = MX*(PD2X - C1X) + MY*(PD2Y - C1Y) + MZ*(PD2Z - C1Z);
		if (G3 <= 0 && G1 <= 0 && G2>=0)
		{
		PD1X = PD3X;
		PD1Y = PD3Y;
		PD1Z = PD3Z;

		}
		if (G3 >= 0 && G2 >= 0 && G1<=0)
		{
		PD2X = PD3X;
		PD2Y = PD3Y;
		PD2Z = PD3Z;
		}
		else if (G3 = 0)
		{
		break;
		}
		else
		{
		printf("错误");
		}

		TX = PD3X;
		TY = PD3Y;
		TZ = PD3Z;

		}
		VectorOfPoint[0].x = TX;
		VectorOfPoint[0].y = TY;
		VectorOfPoint[0].z = TZ;

		kalman_cal();

		Sleep(100);
		}
		*/


	}


}



extern "C" _declspec(dllexport) void kalman_cal()
{

	//printf("%f ,%f, %f\n", VectorOfPoint[0].x, VectorOfPoint[0].y, VectorOfPoint[0].z);

	int i, j;
	for (i = 0; i < 3; i++)
	{
		if (VectorOfPoint[i].x < 1 && VectorOfPoint[i].y < 1)
			continue;


		const CvMat* prediction = cvKalmanPredict(kalman[i], 0);
		//CvPoint predict_pt = cvPoint((int)prediction->data.fl[0], (int)prediction->data.fl[1]);

		//3.update measurement   
		measurement[i]->data.fl[0] = (float)VectorOfPoint[i].x;
		measurement[i]->data.fl[1] = (float)VectorOfPoint[i].y;
		measurement[i]->data.fl[2] = (float)VectorOfPoint[i].z;
		//4.update   
		cvKalmanCorrect(kalman[i], measurement[i]);

		//if (i<7 && i>3).
		//fprintf(F_NUM_COLOR, "%5.3f,%5.3f   %5.3f,%5.3f\n", PointsOfColor[i].x, PointsOfColor[i].y, prediction->data.fl[0], prediction->data.fl[1]);
		//printf("%d-%d    %d-%d\n", (int)PointsOfColor[i].x, (int)PointsOfColor[i].y, (int)prediction->data.fl[0], (int)prediction->data.fl[1]);


		VectorOfPoint[i].x = prediction->data.fl[0];
		VectorOfPoint[i].y = prediction->data.fl[1];
		VectorOfPoint[i].z = prediction->data.fl[2];
	}
	//printf("%f ,%f, %f\n", VectorOfPoint[0].x, VectorOfPoint[0].y, VectorOfPoint[0].z);


	//fprintf(F_NUM_COLOR, "\n");
}

extern "C" _declspec(dllexport) int getTX()
{
	return VectorOfPoint[0].x;
}

extern "C" _declspec(dllexport) int getTY()
{
	return VectorOfPoint[0].y;
}

extern "C"_declspec(dllexport) int getTZ()
{
	return VectorOfPoint[0].z;

}
extern "C" _declspec(dllexport) int getTXL()
{
	return XL;
}

extern "C" _declspec(dllexport) int getTYL()
{
	return YL;
}