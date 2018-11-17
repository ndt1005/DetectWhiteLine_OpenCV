#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;
vector<Vec3f> FindRMax();

namespace
{
	// windows and trackbars name
	const std::string windowName = "Hough Circle Detection Demo";
	const std::string upperThresholdTrackbarName = "Upper threshold";
	const std::string lowerThresholdTrackbarName = "Lower Threshold";
	const std::string usage = "Usage : tutorial_HoughCircle_Demo <path_to_input_image>\n";

	// initial and max values of the parameters of interests.
	const int upperThresholdInitialValue = 255;
	const int lowerThresholdInitialValue = 107;
	const int maxlowerThreshold = 200;
	const int maxupperThreshold = 255;
	// will hold the results of the detection
	//std::vector<Vec3f> circles;
	//static std::vector<Vec3f> rmax;

	
}

int main(int argc, char** argv)
{
	Mat src, src_gray, src_bin,src_reduceNoise;

	// Read the image
	String imageName("tennis-court.jpg"); // by default
	if (argc > 1)
	{
		imageName = argv[1];
	}
	src = imread(imageName, IMREAD_COLOR);

	if (src.empty())
	{
		std::cerr << "Invalid input image\n";
		std::cout << usage;
		return -1;
	}

	// Convert it to gray
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	imshow("grayscale2", src_gray);
	// Reduce the noise so we avoid false circle detection
	//GaussianBlur(src_gray, src_gray, Size(9, 9), 2, 2);
	//imshow("grayscale", src_gray);
	
	//declare and initialize both parameters that are subjects to change
	int upperThreshold = upperThresholdInitialValue;
	int lowerThreshold = lowerThresholdInitialValue;

	// create the main window, and attach the trackbars
	namedWindow(windowName, WINDOW_AUTOSIZE);
	//namedWindow("grayscale", WINDOW_AUTOSIZE);
	//namedWindow("grayscale2", WINDOW_AUTOSIZE);
	createTrackbar(upperThresholdTrackbarName, windowName, &upperThreshold, maxupperThreshold);
	createTrackbar(lowerThresholdTrackbarName, windowName, &lowerThreshold, maxlowerThreshold);

	// infinite loop to display
	// and refresh the content of the output image
	// until the user presses q or Q
	char key = 0;
	while (key != 'q' && key != 'Q')
	{
		// those parameters cannot be =0
		// so we must check here
		upperThreshold = std::max(upperThreshold, 1);
		lowerThreshold = std::max(lowerThreshold, 1);

		//runs the detection, and update the display
		//HoughDetection(src_gray, src, upperThreshold, lowerThreshold);
		inRange(src_gray, lowerThreshold, upperThreshold, src_bin);
		//imshow("binscale", src_bin);
		//reduce noise 
		Mat kernel = cv::getStructuringElement(MORPH_CROSS, Size(5, 5));
		erode(src_bin, src_reduceNoise, kernel);
		dilate(src_reduceNoise, src_reduceNoise, kernel);
		//morphologyEx(src_reduceNoise, src_reduceNoise, MORPH_ERODE, kernel);
		imshow("reduceNoise", src_reduceNoise);
		//find contours to get center
		vector<Vec4i> hierarchy;
		try
		{
			findContours(src_reduceNoise, src_bin, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		}
		catch (cv::Exception & e )
		{
			cerr << e.msg << endl;
		}
		imshow("findContours", src_bin);
		
		// get user key
		key = (char)waitKey(10);
	}

	return 0;
}