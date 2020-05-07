#pragma once

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>
#include <iostream>

using namespace cv;
using namespace std;

class CThinning
{
public:
	CThinning();
	~CThinning();
	int Thinning(unsigned char * ucBinedImg, unsigned char * ucThinnedImage, long lWidth, long lHeight, long lIterativeLimit);
	void Thinning(Mat& src, Mat& dst, long IterativeLimit = -1);
	void PlayThinning(Mat src, Mat& thinned);
	void Dilate(Mat s, Mat& d, int size);

};

