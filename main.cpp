#include <iostream>
#include <chrono>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "RayTracer.hpp"

using namespace std;
using namespace Alectryon;

void populateMat(cv::Mat& mat, const std::array<uint8_t, 3>* data) {
	for (int i = 0; i < mat.rows; i++) {
		for (int j = 0; j < mat.cols; j++) {
			int idx = i * mat.cols + j;

			cv::Vec3b& color = mat.at<cv::Vec3b>(mat.rows - i - 1, j);
			color[0] = data[idx][0];
			color[1] = data[idx][1];
			color[2] = data[idx][2];
		}
	}
}


int main() {

	int width = 640;
	int height = 480;

	Scene scene;
	scene.spheres.push_back({Eigen::Vector3f(0, 0, 10), 10, {255, 0, 0}});
	scene.spheres.push_back({Eigen::Vector3f(5, 0, 4), 5, {255, 255, 255}});
	scene.spheres.push_back({Eigen::Vector3f(-5, 0, 4), 5, {255, 255, 255}});
	scene.lights.push_back({Eigen::Vector3f(10, 20, -10), 0.5});
	scene.lights.push_back({Eigen::Vector3f(-30, 10, -10), 0.5});

	Camera camera(height, width, 48, 64, 10);
	camera.setCenter({0, 0, -10});
	camera.setViewVectors({0, 0, 1}, {0, 1, 0});

	RayTracer rayTracer(camera, scene);

	auto data = rayTracer.trace();

	cv::Mat mat(height, width, CV_8UC3);
	populateMat(mat, data);
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
	cv::imshow("Display window", mat);
	cv::waitKey(25);


	Light& light = scene.lights[1];

	while (1) {

		for (int i = 0; i < 100; i++) {
			light.center[0]	+= 1;
			data = rayTracer.trace();
			populateMat(mat, data);
			cv::imshow("Display window", mat);
			cv::waitKey(25);
		}

		for (int i = 0; i < 100; i++) {
			light.center[0]	-= 1;
			data = rayTracer.trace();
			populateMat(mat, data);
			cv::imshow("Display window", mat);
			cv::waitKey(25);
		}
	}



	// int width = 640;
	// int height = 480;

	// RayTracer rayTracer(64, 48, width, height, 10);

	// Scene scene;
	// scene.spheres.push_back({Eigen::Vector3f(0, 0, -10), 10, {255, 0, 0}});
	// scene.spheres.push_back({Eigen::Vector3f(5, 0, -4), 5, {255, 255, 255}});
	// scene.spheres.push_back({Eigen::Vector3f(-5, 0, -4), 5, {255, 255, 255}});
	// // scene.spheres.push_back({Eigen::Vector3f(40, 0, 100), 10, {0, 255, 0}});
	// // scene.spheres.push_back({Eigen::Vector3f(-40, 0, 100), 10, {0, 255, 0}});
	// scene.lights.push_back({Eigen::Vector3f(10, 20, 10), 0.5});
	// scene.lights.push_back({Eigen::Vector3f(-30, 10, 10), 0.5});

	// std::chrono::time_point<std::chrono::system_clock> start, end;
	// start = std::chrono::system_clock::now();

	// auto data = rayTracer.trace(scene);

	// end = std::chrono::system_clock::now();

	// std::chrono::duration<double> elapsed_seconds = end-start;
	// std::cout <<  "trace took " << elapsed_seconds.count() << " seconds\n";


	// cv::Mat mat(height, width, CV_8UC3);
	// populateMat(mat, data);

	// Light& light = scene.lights[1];

	// cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
	// cv::imshow("Display window", mat);
	// cv::waitKey(25);

	// while (1) {

	// 	for (int i = 0; i < 100; i++) {
	// 		light.center[0]	+= 1;
	// 		data = rayTracer.trace(scene);
	// 		populateMat(mat, data);
	// 		cv::imshow("Display window", mat);
	// 		cv::waitKey(25);
	// 	}

	// 	for (int i = 0; i < 100; i++) {
	// 		light.center[0]	-= 1;
	// 		data = rayTracer.trace(scene);
	// 		populateMat(mat, data);
	// 		cv::imshow("Display window", mat);
	// 		cv::waitKey(25);
	// 	}
	// }

	// cv::waitKey(0);

	// return 0;
}



