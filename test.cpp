#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#define XI_W 648
#define XI_H 488


int ConvertVideoPicture(std::string video_name);

int main(int argc, const char* argv[])
{

	cv::vector<cv::Mat> images;
	/*
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


	number = 0;	
	
	for(;;){


		imshow("image",images[number]);
		cv::waitKey(1);

		

		number++;
		if(number == image_size) number = 0;
	}




	*/


	std::stringstream ss;
	std::string image_name;
	ss <<  "wilflowers.mp4";
	image_name = ss.str();
	
	ConvertVideoPicture(image_name);	

	std::cout << "complete convert!!" << std::endl;	

	return 0;


}


int ConvertVideoPicture(std::string video_name){


	// （1）動画ファイルを開くための準備を行う
	cv::VideoCapture cap(video_name);


	// （2）動画ファイルが正しく開けているかをチェックする（正しく開けていなければエラー終了する）
	if(!cap.isOpened())
		return -1;

	// 画像表示用のウィンドウを生成する
	cv::namedWindow("image", cv::WINDOW_AUTOSIZE);

	// 画像データを格納するための変数を宣言する
	cv::Mat frame;


	double frame_time = 1000/60;
	int number = 0;

	for(;;)
	{
		// （3）動画ファイルから1フレーム分の画像データを取得して、変数frameに格納する
		cap >> frame;

		// 画像データ取得に失敗したらループを抜ける
		if (frame.empty()){
		std::cout << "break : " << number  << std::endl;
		 break;
		}

		// 取得した画像データをウィンドウ表示する
		cv::imshow("image", frame);
		//cv::waitKey(frame_time); 
		std::stringstream ss;
		std::string image_name;
		ss << "./images2/" << number << ".bmp";
		image_name = ss.str();
		cv::imwrite(image_name,frame);
		
		number++;
		
		if(cv::waitKey(frame_time) >= 0) break;

	}
	return 0;

}
