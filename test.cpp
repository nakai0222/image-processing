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


	// $B!J(B1$B!KF02h%U%!%$%k$r3+$/$?$a$N=`Hw$r9T$&(B
	cv::VideoCapture cap("sinbashi.mp4");

	// $B!J(B2$B!KF02h%U%!%$%k$,@5$7$/3+$1$F$$$k$+$r%A%'%C%/$9$k!J@5$7$/3+$1$F$$$J$1$l$P%(%i!<=*N;$9$k!K(B
	if(!cap.isOpened())
		return -1;

	// $B2hA|I=<(MQ$N%&%#%s%I%&$r@8@.$9$k(B
	cv::namedWindow("image", cv::WINDOW_AUTOSIZE);

	// $B2hA|%G!<%?$r3JG<$9$k$?$a$NJQ?t$r@k8@$9$k(B
	cv::Mat frame;

	double frame_time = 1;
	int number = 0;

	for(;;)
	{
		// $B!J(B3$B!KF02h%U%!%$%k$+$i(B1$B%U%l!<%`J,$N2hA|%G!<%?$r<hF@$7$F!"JQ?t(Bframe$B$K3JG<$9$k(B
		cap >> frame;

		// $B2hA|%G!<%?<hF@$K<:GT$7$?$i%k!<%W$rH4$1$k(B
		if (frame.empty()) break;

		// $B<hF@$7$?2hA|%G!<%?$r%&%#%s%I%&I=<($9$k(B
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
