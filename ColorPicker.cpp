#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;


Mat imgHSV, mask;

int hmin = 0, smin = 0, vmin = 0;
int hmax = 179, smax = 255, vmax = 255;

VideoCapture cap(0);
Mat img;

void main() {
	namedWindow("Trackbars", (640, 200));
	createTrackbar("Hue min", "Trackbars", &hmin, 179);
	createTrackbar("Hue max", "Trackbars", &hmax, 179);
	createTrackbar("Sat min", "Trackbars", &smin, 255);
	createTrackbar("Sat max", "Trackbars", &smax, 255);
	createTrackbar("Val min", "Trackbars", &vmin, 255);
	createTrackbar("val max", "Trackbars", &vmax, 255);

	while (true) {
		cap.read(img);

		cvtColor(img, imgHSV, COLOR_BGR2HSV);

		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax); 
		inRange(imgHSV, lower, upper, mask);
		cout << hmin << "," << smin << "," << vmin << "," << hmax << "," << smax << "," << vmax << '\n';
		imshow("Image", img);
		imshow("Image mask", mask);
		waitKey(1);
	}
}