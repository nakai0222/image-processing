#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>



class Video{

public:
int size;
std::string video_name;
cv::vector<cv::Mat> video_data;

int CreateVideo(std::string video_name);
cv::vector<cv::Mat> PutVideo(std::string video_name);

};

int Video::CreateVideo(std::string video_name){


	Video::video_name = video_name;

	// 最初に見つかったカメラを開く
	cv::VideoCapture cap(0);
	if (!cap.isOpened()) {
	std::cout << "カメラの初期化に失敗しました" << std::endl;
	return -1;
	}


	
	// 画像表示用のウィンドウを生成する
	cv::namedWindow(video_name, cv::WINDOW_AUTOSIZE);

	// 画像データを格納するための変数を宣言する
	cv::Mat frame;

	double frame_time = 1;
		


	for(;;){


	// （3）動画ファイルから1フレーム分の画像データを取得して、変数frameに格納する
		cap >> frame;

		// 画像データ取得に失敗したらループを抜ける
		if (frame.empty()) break;

		// 取得した画像データをウィンドウ表示する
		cv::imshow("image", frame);

		cv::waitKey(1);

		Video::video_data.push_back(frame);	
		
		//if(number == image_size) number = 0;
	}

	Video::size = Video::video_data.size();	
	std::cout << Video::size << std::endl;

	return 0;
}



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
	  

	Video video;
	std::string name;
	int dammy = video.CreateVideo(name);

	return 0;
}


