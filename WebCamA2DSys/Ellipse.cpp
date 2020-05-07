#include "Ellipse.h"

CEllipse::CEllipse()
{
	GetCircle = false;
}

CEllipse::~CEllipse()
{
}

//ハフ変換による円検出/円が検出されたかどうかを返却する
bool CEllipse::CircleDetection(Mat src, Mat& torus_mask, Point& c, uint& r, Mat& bk_mask, Mat& drawCir) {

	//マスク画像生成用
	torus_mask = Mat::zeros(src.size(), CV_8UC1);
	bk_mask = Mat::zeros(src.size(), CV_8UC1);
	draw = src.clone();
	tmp_img = src.clone();
	cvtColor(tmp_img, gray, CV_BGR2GRAY);
	//ハフ変換
	vector<Vec3f> circles;
	HoughCircles(gray, circles, CV_HOUGH_GRADIENT,
		2, 200, 100, 150, 150, 180);
	//パラメータの仕様
	//dp = 1,mindist = 200,param(cany),param2 = 円の中心の閾値, 最小距離, 最大距離

	for (size_t i = 0; i < circles.size(); i++) {
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		tmp_rad = cvRound(circles[i][2]);
		//マスク生成
		circle(torus_mask, center, tmp_rad + TH1, Scalar(255, 255, 255),
			((tmp_rad / 2) + TH2), 4);

		circle(bk_mask, center, tmp_rad + BK_TH1, Scalar(255, 255, 255), -1, CV_AA);
		//描画用
		circle(draw, center, tmp_rad, Scalar(255, 0, 255), 1);
		circle(draw, center, 3, Scalar(255, 0, 255), -1, 8, 0);
		x = center.x;
		y = center.y;
		radius = cvRound(circles[i][2]);
		c = center;
		r = radius;
		cnt++;
		//cout << "radius:" << radius << endl;
		break;
	}

	//円が検出されない場合
	if (cnt == 0) {
		cout << "円が検出されません\n圧力計とカメラ位置を調整してください\n" << endl;
	}
	else{
		drawCir = draw.clone();
		GetCircle = true;
	}

	return GetCircle;
}

//目盛り領域の楕円近似
void CEllipse::FittingEllipse(Mat bi, RotatedRect& el, float &r, Mat &draw_el,Mat& elMask,Point c) {
	vector<vector<Point> > contours;
	Mat bimage;
	bimage = bi.clone();
	//クロージング処理
	int dilate_size = 2;
	Mat element = getStructuringElement(cv::MORPH_ELLIPSE,
		Size(2 * dilate_size + 1, 2 * dilate_size + 1),
		Point(dilate_size, dilate_size));
	dilate(bimage, bimage, element);
	erode(bimage, bimage, element);
	Mat cimage = Mat::zeros(bimage.size(), CV_8UC3);

	//目盛り補正
	float bmax = 0;
	float bmin = 10000;
	float sum = 0;
	int cnt = 0;
	float dmin = 10000;
	//中央値の計算に用いる配列
	int buf[100] = { -1 };

	//輪郭検出
	findContours(bimage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	for (size_t i = 0; i < contours.size(); i++) {
		size_t count = contours[i].size();
		if (count < 6)
			continue;
		Mat pointsf;
		Mat(contours[i]).convertTo(pointsf, CV_32F);
		RotatedRect box = fitEllipse(pointsf);

		//取得する輪郭の大きさの閾値
		//if (count < 100 || count > 1000) continue;
		if (count < 30 || count > 2000) continue;
		//輪郭検出
		drawContours(cimage, contours, (int)i, Scalar::all(255), 1, 8);
		//楕円算出
		ellipse(cimage, box, Scalar(0, 0, 255), 1, CV_AA);
		ellipse(cimage, box.center, box.size*0.5f, box.angle, 0, 360, Scalar(0, 255, 0), 1, CV_AA);
		circle(cimage, box.center, 5, Scalar(0, 255, 0), -1, 8, 0);
		//cout << box.center << endl;
		
		//円の中心と楕円の中心の距離
		float distance = sqrt((box.center.x - c.x) * (box.center.x - c.x) + (box.center.y -c. y) * (box.center.y - c.y));
		buf[cnt] = box.size.width;
		//補正対象の楕円決定
		if (dmin > distance) {
			bmax = box.size.width;
			dmin = distance;
			el = box;
		}
		//楕円の最小の幅取得
		else if (box.size.width < bmin) {
			bmin = box.size.width;
		}
		sum += box.size.width;
		cnt++;
		/*BB描画用
		Point2f vtx[4];
		box.points(vtx);
		for (int j = 0; j < 4; j++) {
			line(cimage, vtx[j], vtx[(j + 1) % 4], Scalar(0, 255, 0), 1, CV_AA);
		}*/
	}
	//楕円の幅の中央値計算のためソート
	float tmp, median = 0;
	for (int i = 0; i < 60 - 1; i++) {
		for (int j = i + 1; j < 60; j++) {
			if (buf[i] > 0) {
				if (buf[i] > buf[j]) {
					tmp = buf[i];
					buf[i] = buf[j];
					buf[j] = tmp;
				}
			}
		}
	}
	//中央値算出
	median = (buf[29] + buf[30]) / 2;
	//目盛り補正の自動計算
	float rate = bmin / median;
	//返却値
	r = rate;
	elMask = Mat::zeros(bi.size(), CV_8UC1);
	circle(elMask, el.center, el.size.width + 10, Scalar(255, 255, 255), -1, CV_AA);
	draw_el = cimage;

}
