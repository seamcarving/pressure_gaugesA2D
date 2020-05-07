#pragma once

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iomanip>
#include <iostream>
#include <time.h>
//自作ヘッダ
#include "Draw.h"
#include "Thinning.h"
#include "Labeling.h"
#include "DegreeHistgram.h"
#include "Ellipse.h"

//総カメラ数
#define CAM_MAX_NUM 5
//適応的二値化におけるフィルタサイズ
#define ADAPT_GAUSS_FILTER 51
//デバックの有無変更
#define DEBUG_ON true
#define DEBUG_OFF false

using namespace cv;
using namespace std;

class CControl{

private:
	float analog_value;
	float crct_rate;
	bool GetCircle;
	//他クラス参照用
	CDraw D;
	DegreeHistgram HST;
	CThinning TH;
	CEllipse  EL;
	CLabeling LB;
	//カメラ取得用
	VideoCapture cam;
	//処理速度測定用
	clock_t full_start;
	clock_t full_end;
	clock_t local_time_s;
	clock_t local_time_e;
	//マトリックス
	Mat frame;
	Mat draw_line;
	Mat	trs_mask;
	Mat th_line;
	Mat draw_hou_c;
	Mat before_label;
	Mat al_bin;
	Mat elmask;
	Mat uz_ell_mask;
	Mat th_uz_cmask;
	Mat after_label;
	Mat l_dilate;
	Mat rot;
	Mat plot_data;
	Mat work_inv;
	Mat enhance;
	Mat fit_ell;
	Mat hist;
	Mat enh_bin;
	Mat bk_mask;
	Mat bin_uz_th;
	Mat response;
	//画素値の格納
	Point markSt;
	Point markEd;
	Point hough_ctr;
	Point elps_ctrPt;
	Point linePt;
	RotatedRect elps_box;
	//初期化用
	uint radius;
	uint camNum;
	ostringstream oss;
public:
	CControl();
	~CControl();
	int Start(vector<VideoCapture>,float[CAM_MAX_NUM],bool);
	void Init(Mat, Mat&, Mat&);
	void Init();
};

