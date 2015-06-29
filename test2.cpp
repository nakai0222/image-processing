#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

// マウスコールバック関数に渡す用
struct MOUSE_CALLBACK_DATA {
	int trackObject;
	bool selectObject;
	cv::Point origin;
	cv::Rect selection;
};


// マウスコールバック関数
static void onMouse(int event, int x, int y, int flag, void *arg)
{
	MOUSE_CALLBACK_DATA *data = (MOUSE_CALLBACK_DATA*)arg;

	if (data->selectObject) {
		data->selection.x = MIN(x, data->origin.x);
		data->selection.y = MIN(y, data->origin.y);
		data->selection.width = std::abs(x - data->origin.x);
		data->selection.height = std::abs(y - data->origin.y);
	}

	switch (event) {
		case cv::EVENT_LBUTTONDOWN:
			data->origin = cv::Point(x, y);
			data->selection = cv::Rect(x, y, 0, 0);
			data->selectObject = true;
			break;
		case cv::EVENT_LBUTTONUP:
			data->selectObject = false;
			if (data->selection.width > 0 && data->selection.height > 0) data->trackObject = -1;
			break;
	}
}


// メイン関数
int main(int argc, const char *argv[])
{
	/*  
	// 最初に見つかったカメラを開く
	cv::VideoCapture cap(0);
	if (!cap.isOpened()) {
	std::cout << "カメラの初期化に失敗しました" << std::endl;
	return -1;
	}
	 */ 
	cv::vector<cv::Mat> images;

	int number = 0 ;
	for(;;){
		std::stringstream ss;
		std::string image_name;
		ss <<  "./images/"<<  number << ".png";
		image_name = ss.str();
		cv::Mat image = cv::imread(image_name.c_str(),1);
		if(image.rows == 0 && image.cols == 0)  break;	

		images.push_back(image);
		number++;	
	}
	
	int image_size = images.size();	

	// 閾値
	MOUSE_CALLBACK_DATA data = { 0 };
	int vmin = 10, vmax = 256, smin = 20;
	
	cv::namedWindow("Histogram");
	cv::namedWindow("CamShift Demo");
	cv::setMouseCallback("CamShift Demo", onMouse, &data);
	cv::createTrackbar("Vmin", "CamShift Demo", &vmin, 256);
	cv::createTrackbar("Vmax", "CamShift Demo", &vmax, 256);
	cv::createTrackbar("Smin", "CamShift Demo", &smin, 256);

	// ヒストグラム
	cv::Mat hist;
	cv::Mat histimg = cv::Mat::zeros(200, 320, CV_8UC3);
	cv::Rect trackWindow;
	int hsize = 16;
	float hranges[] = { 0, 180 };
	const float* phranges = hranges;

	int image_number = 0;



	while (1) {
		// キー入力
		char c = cv::waitKey(10);
		if (c == 0x1b) break;

		// 画像を取得
		/*
		cv::Mat image;
		cap >> image;
		if (image.empty()) break;
		*/

		cv::Mat image = images[image_number];
	
		// HSVに変換
		cv::Mat hsv;
		cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

		if (data.trackObject) {
			// 閾値処理
			cv::Mat mask;
			cv::Scalar lower(  0, smin, MIN(vmin, vmax));
			cv::Scalar upper(180,  256, MAX(vmin, vmax));
			cv::inRange(hsv, lower, upper, mask);


			// Hueだけ抽出
			int ch[] = { 0, 0 };
			cv::Mat hue(hsv.size(), hsv.depth());
			mixChannels(&hsv, 1, &hue, 1, ch, 1);

			// 矩形領域を選択した
			if (data.trackObject < 0) {
				// 追跡窓を設定
				trackWindow = data.selection;
				trackWindow &= cv::Rect(0, 0, image.cols, image.rows);

				// ヒストグラムを計算
				cv::Mat roi(hue, trackWindow), maskroi(mask, trackWindow);
				cv::calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
				cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);

				// 表示用にRGBに変換
				histimg = cv::Scalar::all(0);
				int binW = histimg.cols / hsize;
				cv::Mat buf(1, hsize, CV_8UC3);
				for (int i = 0; i < hsize; i++) buf.at<cv::Vec3b>(i) = cv::Vec3b(cv::saturate_cast<uchar>(i*180. / hsize), 255, 255);
				cv::cvtColor(buf, buf, cv::COLOR_HSV2BGR);

				// ヒストグラムを描画
				for (int i = 0; i < hsize; i++) {
					int val = cv::saturate_cast<int>(hist.at<float>(i)*histimg.rows / 255);
					cv::rectangle(histimg, cv::Point(i*binW, histimg.rows), cv::Point((i + 1)*binW, histimg.rows - val), cv::Scalar(buf.at<cv::Vec3b>(i)), -1, 8);
				}

				// トラッキングを開始
				data.trackObject = 1;
			}

			// バックプロジェクション
			cv::Mat backproj;
			cv::calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
			backproj &= mask;

			// CamShiftアルゴリズム
			cv::RotatedRect trackBox = cv::CamShift(backproj, trackWindow, cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10, 1));

			// 表示
			ellipse(image, trackBox, cv::Scalar(0, 0, 255), 3, 16); // cv::LINE_AA=16
		}

		// 選択領域を表示
		if (data.selectObject && data.selection.width > 0 && data.selection.height > 0) {
			cv::Rect roi = data.selection & cv::Rect(0, 0, image.cols, image.rows);
			cv::Mat tmp(image, roi);
			cv::bitwise_not(tmp, tmp);
		}
		
		// 表示
		cv::imshow("CamShift Demo", image);
		cv::imshow("Histogram", histimg);
			
		image_number++;
		if(image_number >= image_size) image_number = 0;


	}

	return 0;
}
