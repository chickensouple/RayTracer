#include <iostream>
#include <chrono>
#include <cmath>
#include <list>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "RayTracer.hpp"
#include "ColorCoordinates.hpp"

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


	scene.lights.push_back({Eigen::Vector3f(10, 20, 0), 0.5});
	scene.lights.push_back({Eigen::Vector3f(-30, 10, 0), 0.5});

	Camera camera(height, width, .48, .64, .1);
	camera.setCenter({0, 0, -10});
	camera.setViewVectors({0, 0, 1}, {0, 1, 0});

	RayTracer rayTracer(camera, scene);

	cv::Mat mat(height, width, CV_8UC3);
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
	cv::imshow("Display window", mat);

	Light& light = scene.lights[1];

	int dir = 1;



	std::list<float> fpsList;
	float fpsSum = 0;
	while (1) {
		for (int i = 0; i < 100; i++) {
			light.center[0]	+= dir;

			// float cameraParameterTheta = (M_PI * i) / 50;
			// float cameraX = 20 * sin(cameraParameterTheta);
			// float cameraZ = 20 * cos(cameraParameterTheta) + 10;

			float cameraX = 0;
			float cameraZ = -5;

			Eigen::Vector3f cameraCenter(cameraX, 0, cameraZ);
			camera.setCenter(cameraCenter);

			Eigen::Vector3f dir = Eigen::Vector3f(0, 0, 10) - cameraCenter;
			dir.normalize();
			camera.setViewVectors(dir, {0, 1, 0});


			std::chrono::time_point<std::chrono::system_clock> start, end;
			start = std::chrono::system_clock::now();
			auto data = rayTracer.trace();
			end = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsed_seconds = end-start;
			

			float fps = 1.0 / elapsed_seconds.count();
			fpsList.push_back(fps);
			fpsSum += fps;

			if (fpsList.size() > 100) {
				fpsSum -= fpsList.front();
				fpsList.pop_front();
			}


			std::cout << "trace fps: " << fps << "\taverage fps: " << fpsSum / fpsList.size() << '\n';


			populateMat(mat, data);
			cv::imshow("Display window", mat);
			
			cv::waitKey(25);
		}

		dir *= -1;
	}
}



