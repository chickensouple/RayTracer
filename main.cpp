#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "RayTracer.hpp"

using namespace std;

int main() {
	// RayTracer rayTracer(2, 2, 2, 2, 10);
	// Scene scene;
	// scene.spheres.push_back({Eigen::Vector3f(0, 0, 20), 10, {255, 0, 0}});
	// auto data = rayTracer.trace(scene);


	RayTracer rayTracer(64, 48, 640, 480, 10);
	// RayTracer rayTracer(48, 64, 480, 640, 10);

	Scene scene;
	scene.spheres.push_back({Eigen::Vector3f(0, 0, -10), 10, {255, 0, 0}});
	scene.spheres.push_back({Eigen::Vector3f(20, 0, -10), 10, {255, 255, 255}});
	// scene.spheres.push_back({Eigen::Vector3f(40, 0, 100), 10, {0, 255, 0}});
	// scene.spheres.push_back({Eigen::Vector3f(-40, 0, 100), 10, {0, 255, 0}});
	scene.lights.push_back({Eigen::Vector3f(10, 20, 10), 1});
	auto data = rayTracer.trace(scene);


	cv::Mat mat(480, 640, CV_8UC3);
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			int idx = i * mat.cols + j;

			cv::Vec3b& color = mat.at<cv::Vec3b>(mat.rows - i - 1, j);
			color[0] = data[idx][0];
			color[1] = data[idx][1];
			color[2] = data[idx][2];
		}
	}


	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
	cv::imshow("Display window", mat);
	cv::waitKey(0);

	return 0;
}



