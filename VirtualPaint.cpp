#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>



cv::Mat img;
std::vector<std::vector<int>> newPoints;


//Run the ColorPicker.cpp for selecting the colors and their HSV values
std::vector<std::vector<int>> myColors{ {116,94,109,128,141,198},//Purple
	                          {64,74,59,85,182,127}, //Green
                             };


//BGR format color values
std::vector<cv::Scalar> myColorValues{ {255,0,255}, //Purple
	                          {0,255,0}, //Green
};


cv::Point getContours(cv::Mat imgDil) {
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	std::vector<std::vector<cv::Point>> conPoly(contours.size());
	std::vector<cv::Rect> boundRect(contours.size());
	std::string objectType;

	cv::Point myPoint(0, 0);

	for (int i = 0; i < contours.size(); i++) {
		int area = contourArea(contours[i]);
		if (area > 1000) {
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);//to show the vertex points

			boundRect[i] = boundingRect(conPoly[i]);//to show bounding rectangles
			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			myPoint.y = boundRect[i].y;

			drawContours(img, contours, i, cv::Scalar(255, 0, 255), 2);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(0, 255, 0), 5);
		}
	}
	return myPoint;
}

std::vector<std::vector<int>> findColor(cv::Mat img) {
	cv::Mat imgHSV;
	cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

	for (int i = 0; i < myColors.size(); i++) {
		cv::Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		cv::Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
		cv::Mat mask;
		inRange(imgHSV, lower, upper, mask);
		//imshow(to_string(i), mask);
		cv::Point myPoint = getContours(mask);
		if (myPoint.x != 0 && myPoint.y != 0) {
			newPoints.push_back({ myPoint.x, myPoint.y, i });
		}
	}

	return newPoints;
}

void drawOnCanvas(std::vector<std::vector<int>>& newPoints, std::vector<cv::Scalar>& myColorValues) {
	for (int i = 0; i < newPoints.size(); i++) {
		circle(img, cv::Point(newPoints[i][0], newPoints[i][1]), 10, myColorValues[newPoints[i][2]], cv::FILLED);
	}
}

int main() {
	cv::VideoCapture cap(0);
	while (true) {
		cap.read(img);
		flip(img, img, 1);
		newPoints = findColor(img);
		drawOnCanvas(newPoints, myColorValues);
		
		imshow("real time", img);
		cv::waitKey(1);
	}
	return 0;
}