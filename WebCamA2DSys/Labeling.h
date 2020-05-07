#pragma once

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>
#include <iostream>

using namespace cv;
using namespace std;

class CLabeling
{

private:

public:
	CLabeling();
	~CLabeling();
	void Labeling(Mat bin, Mat& MaxLabel);
};
