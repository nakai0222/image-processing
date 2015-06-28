#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#define IMAGE_SIZE 2
#define XI_W 648
#define XI_H 488



int main(int argc, const char* argv[])
{


	// （1）動画ファイルを開くための準備を行う
	cv::VideoCapture cap("sinbashi.mp4");

	// （2）動画ファイルが正しく開けているかをチェックする（正しく開けていなければエラー終了する）
	if(!cap.isOpened())
		return -1;

	// 画像表示用のウィンドウを生成する
	cv::namedWindow("image", cv::WINDOW_AUTOSIZE);

	// 画像データを格納するための変数を宣言する
	cv::Mat frame;

	double frame_time = 1;
	int number = 0;

	for(;;)
	{
		// （3）動画ファイルから1フレーム分の画像データを取得して、変数frameに格納する
		cap >> frame;

		// 画像データ取得に失敗したらループを抜ける
		if (frame.empty()) break;

		// 取得した画像データをウィンドウ表示する
		cv::imshow("image", frame);
	
		std::stringstream ss;
		std::string image_name;
		ss <<  "./images/" << number << ".png";
		image_name = ss.str();
		cv::imwrite(image_name,frame);
		number++;

		if(cv::waitKey(frame_time) >= 0) break;


	}


	return 0;


}
