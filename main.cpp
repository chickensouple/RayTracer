#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "RayTracer.hpp"

int main() {
	// RayTracer rayTracer(2, 2, 2, 2, 10);
	// Scene scene;
	// scene.spheres.push_back({Eigen::Vector3f(0, 0, 20), 10, {255, 0, 0}});
	// auto data = rayTracer.trace(scene);


	RayTracer rayTracer(64, 48, 640, 480, 10);

	Scene scene;
	scene.spheres.push_back({Eigen::Vector3f(0, 0, 20), 5, {255, 0, 0}});
	// scene.spheres.push_back({Eigen::Vector3f(20, 0, 20), 5, {0, 255, 0}});
	scene.lights.push_back({Eigen::Vector3f(3, 20, 0), 1});
	auto data = rayTracer.trace(scene);

	cv::Mat mat(480, 640, CV_8UC3);
	for (int i = 0; i < 480; i++) {
		for (int j = 0; j < 640; j++) {
			int idx = i * 480 + j;

			cv::Vec3b& color = mat.at<cv::Vec3b>(i, j);
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



