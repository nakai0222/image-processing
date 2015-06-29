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


	// $B!J(B1$B!KF02h%U%!%$%k$r3+$/$?$a$N=`Hw$r9T$&(B
	cv::VideoCapture cap(video_name);


	// $B!J(B2$B!KF02h%U%!%$%k$,@5$7$/3+$1$F$$$k$+$r%A%'%C%/$9$k!J@5$7$/3+$1$F$$$J$1$l$P%(%i!<=*N;$9$k!K(B
	if(!cap.isOpened())
		return -1;

	// $B2hA|I=<(MQ$N%&%#%s%I%&$r@8@.$9$k(B
	cv::namedWindow("image", cv::WINDOW_AUTOSIZE);

	// $B2hA|%G!<%?$r3JG<$9$k$?$a$NJQ?t$r@k8@$9$k(B
	cv::Mat frame;


	double frame_time = 1000/60;
	int number = 0;

	for(;;)
	{
		// $B!J(B3$B!KF02h%U%!%$%k$+$i(B1$B%U%l!<%`J,$N2hA|%G!<%?$r<hF@$7$F!"JQ?t(Bframe$B$K3JG<$9$k(B
		cap >> frame;

		// $B2hA|%G!<%?<hF@$K<:GT$7$?$i%k!<%W$rH4$1$k(B
		if (frame.empty()){
		std::cout << "break : " << number  << std::endl;
		 break;
		}

		// $B<hF@$7$?2hA|%G!<%?$r%&%#%s%I%&I=<($9$k(B
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
