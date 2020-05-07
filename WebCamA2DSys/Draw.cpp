#include "Draw.h"

CDraw::CDraw()
{
}

CDraw::~CDraw()
{
}

//出力結果の描画用
float CDraw::PlotData(Mat lab, Point c, Point s, Point e, uint rad, Point lp, Mat& dst, RotatedRect box, float rate) {
	dst = lab.clone();
	//角度計算用関数
	//アナログ値出力
	float analog_value = CalcAnalogValue(c, s, e, lp, rate);
	//アルゴリズム結果の描画
	cv::ellipse(dst, box, Scalar(237, 85, 27), 2, CV_AA);
	cv::circle(dst, box.center, 6, Scalar(237, 85, 27), -1, 8, 0);
	cv::line(dst, Point(e.x, e.y), Point(c.x, c.y), Scalar(0, 255, 0), 2, CV_AA);
	cv::line(dst, Point(s.x, s.y), Point(c.x, c.y), Scalar(0, 255, 0), 2, CV_AA);
	cv::line(dst, Point(lp),Point(c), Scalar(255, 0, 255), 3, CV_AA);
	cv::circle(dst, c, 1, Scalar(255, 0, 255), -1, 8, 0);
	const int face[] = { cv::FONT_HERSHEY_SIMPLEX, cv::FONT_HERSHEY_PLAIN, cv::FONT_HERSHEY_DUPLEX, cv::FONT_HERSHEY_COMPLEX,
		cv::FONT_HERSHEY_TRIPLEX, cv::FONT_HERSHEY_COMPLEX_SMALL, cv::FONT_HERSHEY_SCRIPT_SIMPLEX,
		cv::FONT_HERSHEY_SCRIPT_COMPLEX, cv::FONT_ITALIC };
	string str = to_string(analog_value);
	putText(dst, "Analog:" + str, cv::Point(40, 150), face[1], 2.5, cv::Scalar(255, 0, 255), 3, CV_AA);
	return analog_value;
}

//始点と芯線の角度算出用
float CDraw::CalcAnalogValue(Point c, Point s, Point e, Point li, float rate) {
	//内積外積の計算用変数
	float inner;
	float cross;
	float atan_rad;
	float atan_degree;
	//目盛りと目盛りの間隔に対する値
	float m2mValue = 1 * MTR_MAX_VALUE / MEMORI_NUM;

	Point v1, v2, v3;
	//ベクトル e s line
	v1.x = e.x - c.x;	v1.y = e.y - c.y;
	v2.x = s.x - c.x;	v2.y = s.y - c.y;
	v3.x = li.x - c.x;	v3.y = li.y - c.y;
	//startとendの角度からm2m算出
	inner = v1.x * v2.x + v1.y * v2.y;
	cross = v1.x * v2.y - v1.y * v2.x;
	atan_rad = atan2f(cross, inner);
	//360度に補正
	if (atan_rad < 0) {
		atan_rad = atan_rad + (2 * CV_PI);
		atan_degree = atan_rad * 180.0 / (CV_PI);
	}
	else {
		atan_degree = atan_rad * 180.0 / (CV_PI);
	}
	float m2mRad = (360 - atan_degree) / MEMORI_NUM;
	inner = v2.x * v3.x + v2.y * v3.y;
	cross = v2.x * v3.y - v2.y * v3.x;
	atan_rad = atan2f(cross, inner);
	//cout << "内積外積ラジアン:" << atan_rad << endl;
	if (atan_rad < 0) {
		atan_rad = atan_rad + (2 * CV_PI);
		atan_degree = atan_rad * 180.0 / (CV_PI);//0-360
	}
	else {
		atan_degree = atan_rad * 180.0 / (CV_PI);
	}

	//0から0.02目盛りの補正
	atan_degree += m2mRad * STABLE_RATE;
	//cout << "m2mrad" << m2mRad << endl;
	//cout << "内積外積角度:" << atan_degree << endl;

	//アナログ値算出結果
	float ResultValue = (atan_degree) * (m2mValue / m2mRad);
	//ベクトルの逆向き補正(0の場合)
	if(ResultValue >= MTR_MAX_VALUE){
		ResultValue = 0.0;
	}

	return ResultValue;
}
