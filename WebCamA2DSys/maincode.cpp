#include "Control.h"

void response(float *result);
vector<VideoCapture> getCamNum(vector<VideoCapture> list);

int main(void) {
	//アナログ値を返却
	float result[CAM_MAX_NUM];
	//カメラを獲得する変数
	vector<VideoCapture> list;
	//総カメラ数取得リストに保持
	list = getCamNum(list);

	//cout << "camnum" << list.size() << endl;

	while (1) {
		CControl *Algorithm = new CControl();
		Algorithm->Start//(list,result,DEBUG_ON);//画像表示
						(list,result, DEBUG_OFF);//画像非表示
		response(result);
		delete Algorithm;
	}
	return 0;
}

//アナログ値を描画
void response(float *result) {
	for (int i = 0; i < CAM_MAX_NUM; i++) {
		cout << "Analog[" << i << "]:" << result[i] << endl;
	}
	printf("\n");
}

//カメラ台数認識用
vector<VideoCapture> getCamNum(vector<VideoCapture> list) {
	//カメラ定義0-4はusbポート番号を示す
	cv::VideoCapture cam0(0);
	cv::VideoCapture cam1(1);
	cv::VideoCapture cam2(2);
	cv::VideoCapture cam3(3);
	cv::VideoCapture cam4(4);
	//カメラ確認
	if (cam0.isOpened())	list.push_back(cam0);
	if (cam1.isOpened())	list.push_back(cam1);
	if (cam2.isOpened())	list.push_back(cam2);
	if (cam3.isOpened())	list.push_back(cam3);
	if (cam4.isOpened())	list.push_back(cam4);

	return list;
}