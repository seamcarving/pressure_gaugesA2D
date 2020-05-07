#pragma once
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>
#include <iostream>

//マスクの閾値
//変更する必要はありません
#define TH1 10//値を小さくすると針の領域を多く取得する
#define TH2 30//大きくするとトーラスの幅が増える
#define BK_TH1 30//大きくすると大きく取得

using namespace cv;
using namespace std;
class CEllipse
{
private:
	bool GetCircle;
	uint x, y = 0;
	uint cnt = 0;
	uint radius;
	uint tmp_rad;
	Mat draw;
	Mat gray;
	Mat tmp_img;
public:
	CEllipse();
	~CEllipse();
	bool CircleDetection(Mat src, Mat& mask, Point& c, uint& r, Mat& bkgd_mask, Mat& drawCir);
	void FittingEllipse(Mat bi, RotatedRect& el, float &r, Mat& draw_el,Mat&,Point);
};

