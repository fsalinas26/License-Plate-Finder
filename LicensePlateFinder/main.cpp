
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <filesystem>

using namespace cv;
using namespace std;

int main()
{
	int i = 1;
	std::string path = "./plates/1.jpg";
	while (filesystem::exists(path))
	{
		Mat originalimg = imread(path);
		Mat img, biFilter, processImg;
		copyTo(originalimg, img, noArray());
		cvtColor(img, img, COLOR_BGR2GRAY);

		bilateralFilter(img, biFilter, 13, 15, 15);
		medianBlur(biFilter, processImg, 3);

		vector<vector<Point>> edges, contours, possiblePlates;
		vector<Vec4i> out_array;

		Mat edged(processImg.cols, processImg.rows, CV_8UC1);

		Canny(processImg, edged, 30, 200);

		findContours(edged, contours, out_array, RETR_TREE, CHAIN_APPROX_SIMPLE);

		sort(contours.begin(), contours.end(), [](const vector<Point>& a, const vector<Point>& b) { return a.size() > b.size(); });

		for (auto& contour : contours)
		{
			double peri = arcLength(contour, true);
			vector<Point> approx;
			approxPolyDP(contour, approx, 0.018 * peri, true);
			double aspectRatio = boundingRect(contour).size().aspectRatio();
			if (approx.size() == 4)
			{
				possiblePlates.push_back(approx);
			}
		}

		sort(possiblePlates.begin(), possiblePlates.end(), [](const vector<Point>& a, const vector<Point>& b) { return boundingRect(a).area() > boundingRect(b).area(); });

		int largest_p_vec = 0, probable_plate_idx = 0;
		for (int i = 0; i < possiblePlates.size(); i++)
		{
			Mat temp;
			copyTo(img, temp, noArray());
			Rect rect = boundingRect(possiblePlates[i]);
			Mat plateCropped = temp(rect);

			vector<vector<Point>> contours_p;
			vector<Vec4i> out_array_p;

			Mat p_cnt(plateCropped.cols, plateCropped.rows, CV_8UC1);
			Canny(plateCropped, p_cnt, 30, 200);
			findContours(p_cnt, contours_p, out_array_p, RETR_TREE, CHAIN_APPROX_SIMPLE);

			if (contours_p.size() > 0)
			{
				if (contours_p.size() > largest_p_vec)
				{
					largest_p_vec = contours_p.size();
					probable_plate_idx = i;
				}
			}
		}
		drawContours(originalimg, possiblePlates, probable_plate_idx, Scalar(0, 0, 255), 3);


		imshow("processed", edged);
		imshow("original", originalimg);

		waitKey();

		i++;
		path = (("./plates/" + to_string(i) + ".jpg"));
	}
}

