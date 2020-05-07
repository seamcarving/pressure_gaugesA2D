#include "DegreeHistgram.h"

DegreeHistgram::DegreeHistgram()
{
}

DegreeHistgram::~DegreeHistgram()
{
}
//二点間の距離測定
float DegreeHistgram::getDistance(float x, float y, float x2, float y2) {
	float distance = sqrt((x2 - x) * (x2 - x) + (y2 - y) * (y2 - y));
	return distance;
}

//角度ヒストグラムによる目盛り取得と始終点探索
int DegreeHistgram::histgramMethodUzGray(Mat lbl, Point c, Mat draw, Point& s, Point& e, Point& l, RotatedRect& el, Mat& res, Mat& dline) {
	int cols = lbl.cols;
	int rows = lbl.rows;
	//円検出の半径を制限
	if (c.x >= cols) {
		cout << "正しく円が検出できていない(大きい）" << endl;
		return 0;
	}
	if (c.y >= rows) {
		cout << "正しく円が検出できていない(大きい)" << endl;
		return 0;
	}

	dlbl = lbl.clone();
	//y最大値による始終
	//トリミング
	Mat Llabel(lbl, Rect(0, c.y, c.x, lbl.rows - c.y));
	Mat Rlabel(lbl, Rect(c.x, c.y, lbl.cols - c.x, lbl.rows - c.y));
	//描画Debug
	//imshow("R", Rlabel);
	//imshow("L", Llabel);
	//Rlabel = ~Rlabel;
	//Llabel = ~Llabel;
	//imwrite("R.jpg", Rlabel);
	//imwrite("L.jpg", Llabel);

	//終点y最大値
	for (int row = 0; row < Rlabel.rows; row++) {
		for (int col = 0; col < Rlabel.cols; col++) {
			if (Rlabel.at<unsigned char>(row, col) == 255) {
				if(row > Rmax){
					Rmax = row;
					Rpt.x = col;
					Rpt.y = row;
				}
			}
		}
	}
	//トリミングの補正
	Rpt.x += c.x;
	Rpt.y += c.y;
	//cout << "rmin" << Rpt << endl;
	//始点y最大値
	for (int row = 0; row < Llabel.rows; row++) {
		for (int col = 0; col < Llabel.cols; col++) {
			if (Llabel.at<unsigned char>(row, col) == 255) {
				if (row > Lmax) {
					Lmax = row;
					Lpt.x = col;
					Lpt.y = row;
				}
			}
		}
	}
	//トリミングの補正
	Lpt.y += c.y;

	//ヒストグラムによる芯線座標算出
	Mat calc_hist = Mat::zeros(360, 360, lbl.type());
	Mat draw_hist = Mat::zeros(360, 360, lbl.type());
	//角度格納用配列
	int buf[360] = {0};
	//ベクトルを固定
	lv.x = 0 - c.x;
	lv.y = 0 - c.y;
	//角度計算用変数
	float sita;
	float radian;
	float inner;
	float cross;
	int q;

	//ベクトルの角度算出
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			if (lbl.at<unsigned char>(row, col) == 255) {
				tv.x = col - c.x;
				tv.y = row - c.y;
				//外積内積
				inner = lv.x * tv.x + lv.y * tv.y;
				cross = lv.x * tv.y - lv.y * tv.x;
				sita = atan2f(cross, inner);
				//360度に補正
				if (sita < 0) {
					sita = sita + (2 * CV_PI);
					radian = sita * 180.0 / (CV_PI);
				}
				else {
					radian = sita * 180.0 / (CV_PI);
				}
				//頻度加算値
				buf[(int)radian] += 3;
			}
		}
	}

	//計算用ヒストグラムに代入
	int maxrad = 0;
	int tmpbuf = 0;
	for (int i = 0; i < 360; i++) {
		if (buf[i] >= calc_hist.rows) {
			buf[i] = calc_hist.rows -1;
		}
		if (buf[i] > tmpbuf) {
			tmpbuf = buf[i];
			maxrad = i;
		}
		calc_hist.at<unsigned char>(buf[i], i) = 255;
		//描画用ヒストグラムに描画
		if (buf[i] > 0) {
			cv::line(draw_hist, Point(i, buf[i]), Point(i, 0), Scalar(255, 255, 255), 1, 4);
		}
	}

	//算出した角度と最も近い角度の画素を探索
	int maxp = 0;
	int sminrad = 1000;
	int eminrad = 1000;
	int maxminrad = 1000;
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			if (lbl.at<unsigned char>(row, col) == 255) {
				tv.x = col - c.x;
				tv.y = row - c.y;
				float inner = lv.x * tv.x + lv.y * tv.y;
				float cross = lv.x * tv.y - lv.y * tv.x;
				sita = atan2f(cross, inner);
				if (sita < 0) {
					sita = sita + (2 * CV_PI);
					radian = sita * 180.0 / (CV_PI);
				}
				else {
					radian = sita * 180.0 / (CV_PI);
				}
				if (abs(radian - maxrad) < maxminrad) {
					maxminrad = abs(radian - maxrad);
					maxValueP.x = col;
					maxValueP.y = row;
				}
			}
		}
	}
	//最下端の始終点座標を格納
	if (Rpt.x < Lpt.x) {
		Rpt.x += 1;
		Lpt.x -= 1;
		s = Rpt;
		e = Lpt;
	}
	else {
		Rpt.x += 1;
		Lpt.x -= 1;
		e = Rpt;
		s = Lpt;
	}

	//返却値の代入
	dline = dlbl;
	l = maxValueP;
	res = draw_hist;

	cout << "e" << endl;
	cout << e << endl;

	cout << "s" << endl;
	cout <<  s << endl;

	return 0;
}