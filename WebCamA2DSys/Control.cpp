#include "Control.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

CControl::CControl()
{
	Init();
}
CControl::~CControl()
{
}

void CControl::Init(void) {
	camNum = 5;
	crct_rate = 1.0;
	analog_value = 0;
}

void CControl::Init(Mat src, Mat& enh, Mat& inv) {
	//ダウンスケール
	resize(src, src, Size(), 0.5, 0.5, INTER_LANCZOS4);
	//詳細強化フィルタ
	detailEnhance(src, enh, 20, 0.15f);
	inv = enh.clone();
}

int CControl::Start(vector<VideoCapture> list, float result[CAM_MAX_NUM], bool d_mode) {
	//初期化
	for (int i = 0; i < CAM_MAX_NUM; i++) {
		result[i] = -1;
	}

	//処理開始リストが空になるまで
	//リストにはwebカメラを保持
	while (!list.empty()) {
		//cout << "listsize" << list.size() << endl;
		//リストの末尾取得
		cam = list.back();
		///debug
		//if (list.size() == 2) {
		//	frame = imread("./images\\input\\6 (2).jpg");
		//}else if (list.size() == 1) {
		//	frame = imread("./images\\input\\6 (3).jpg");
		//}else if (list.size() == 0) {
		//	frame = imread("./images\\input\\6 (4).jpg");
		//}
		///debug

		//listの末尾のカメラから画像取得
		cam >> frame;
		//初期化
		Init(frame, enhance, work_inv);
		//描画用
		response = frame.clone();
		resize(response, response, Size(), 0.5, 0.5, INTER_LANCZOS4);
		//確率的ハフによる円検出/円検出でtrue
		GetCircle = EL.CircleDetection(enhance, trs_mask, hough_ctr, radius, bk_mask, draw_hou_c);
		if (GetCircle) {
			//処理速度測定用変数
			//local_time_s = clock();
			//グレイスケール変換
			cvtColor(work_inv, enh_bin, CV_BGR2GRAY);
			//適応的二値化
			adaptiveThreshold(enh_bin, enh_bin, 255, ADAPTIVE_THRESH_GAUSSIAN_C,
				THRESH_BINARY_INV, ADAPT_GAUSS_FILTER, 2);
			//背景分離のマスク処理
			bitwise_and(bk_mask, enh_bin, bin_uz_th);
			//imwrite("./images\\bin.png", bin_uz_th);
			//ラベリングによる目盛り領域取得
			LB.Labeling(bin_uz_th, after_label);
			//グレイスケールと二値化
			al_bin = after_label.clone();
			cvtColor(al_bin, al_bin, CV_BGR2GRAY);
			threshold(al_bin, al_bin, 128, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
			//細線化
			TH.PlayThinning(al_bin, th_uz_cmask);
			before_label = th_uz_cmask.clone();
			//楕円近似
			EL.FittingEllipse(th_uz_cmask, elps_box, crct_rate, fit_ell, elmask, hough_ctr);
			//ハフの中心から楕円の中心に変更
			hough_ctr.x = elps_box.center.x;
			hough_ctr.y = elps_box.center.y;
			//マスク反転
			elmask = ~elmask;
			//マスク処理
			bitwise_and(elmask, th_uz_cmask, uz_ell_mask);
			//ヒストグラム計算
			HST.histgramMethodUzGray(uz_ell_mask, hough_ctr, enhance, markSt, markEd, linePt, elps_box, hist, draw_line);
			//アナログ値の算出結果
			analog_value = D.PlotData(response, hough_ctr, markSt, markEd, radius, linePt, plot_data, elps_box, crct_rate);
			//処理速度測定用変数
			//full_end = clock();
			//cout << "full_time: " << (float)(full_end - full_start) / CLOCKS_PER_SEC << "sec.\n\n";

			//デバッグモード
			if (d_mode) {
				oss << std::setfill('0') << std::setw(1) << list.size()/*i*/;
				//画像表示
				imshow("hough_circle", draw_hou_c);
				imshow("before_thining", bin_uz_th);
				imshow("adapt_binary", enh_bin);
				imshow("thinning(th_uz_cmask)", th_uz_cmask);
				imshow("before_label", before_label);
				imshow("after_label", after_label);
				imshow("ellipse", fit_ell);
				imshow("histgram", hist);
				imshow("plot", plot_data);
				//画像反転
				before_label = ~before_label;
				after_label = ~after_label;
				fit_ell = ~fit_ell;
				draw_line = ~draw_line;
				th_uz_cmask = ~th_uz_cmask;
				hist = ~hist;
				//絶対パスに画像保存
				//フォルダを作成すると保存される
				imwrite("./images\\cam" + oss.str() + "\\enhance.png", enhance);
				imwrite("./images\\cam" + oss.str() + "\\binary.png", enh_bin);
				imwrite("./images\\cam" + oss.str() + "\\hough_circle.png", draw_hou_c);
				imwrite("./images\\cam" + oss.str() + "\\bkmask.png", bk_mask);
				imwrite("./images\\cam" + oss.str() + "\\cmask.png", trs_mask);
				imwrite("./images\\cam" + oss.str() + "\\thinned.png", th_uz_cmask);
				imwrite("./images\\cam" + oss.str() + "\\before_label.png", before_label);
				imwrite("./images\\cam" + oss.str() + "\\after_label.png", after_label);
				imwrite("./images\\cam" + oss.str() + "\\Ellipse.png", fit_ell);
				imwrite("./images\\cam" + oss.str() + "\\histgram.png", hist);
				imwrite("./images\\cam" + oss.str() + "\\line.png", draw_line);
				imwrite("./images\\cam" + oss.str() + "\\Results_data" + oss.str() + " .png", plot_data);
				//画像表示用
				waitKey(0);
			}
		}
		//返却値に代入
		result[list.size() - 1] = analog_value;
		//リストの更新
		list.pop_back();
	}
	return 0;
}