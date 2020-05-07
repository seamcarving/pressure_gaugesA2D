#pragma once

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>
#include <iostream>

using namespace cv;
using namespace std;
//メータの目盛りの総数を定義
#define MEMORI_NUM 50
//メータの目盛りの最大値を定義
#define MTR_MAX_VALUE 1.0
//0から0.02のメモリの間隔を補正//通常の目盛りと比較した際の倍率
#define STABLE_RATE 0.3

class CDraw
{
public:
	CDraw();
	~CDraw();
	float PlotData(Mat lab, Point c, Point s, Point e, uint rad, Point lp, Mat& dst, RotatedRect box, float rate);
	float CalcAnalogValue(Point c, Point s, Point e, Point li, float rate);
};

