#pragma once

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>
#include <iostream>

using namespace cv;
using namespace std;

class DegreeHistgram
{

private:
	Mat dlbl;
	Point Rpt;
	Point Lpt;
	//radianŒvZ—p
	Point lv, tv;
	//Zo‚µ‚½Šp“x‚ÆÅ‚à‹ß‚¢Šp“x‚Ì‰æ‘f‚ğ’Tõ
	Point sMinRadP;
	Point eMinRadP;
	Point maxValueP;
	int dtmp = 0;
	int mind = 10000;
	Point minDp = 0;
	//nI“_Ši”[—p
	int Rmax = 0;
	int Lmax = 0;

public:
	DegreeHistgram();
	~DegreeHistgram();
	float getDistance(float x, float y, float x2, float y2);
	int histgramMethodUzGray(Mat lbl, Point c, Mat draw, Point&,Point&, Point&,RotatedRect& ,Mat&,Mat&);
};

