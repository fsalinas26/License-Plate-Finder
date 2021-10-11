
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
		Mat img, biFilter, filtered_image;
		copyTo(originalimg, img, noArray());
		cvtColor(img, img, COLOR_BGR2GRAY);

		bilateralFilter(img, biFilter, 13, 15, 15);
		medianBlur(biFilter, filtered_image, 3);

		vector<vector<Point>> edges, contours, possible_plates;
		vector<Vec4i> out_array;
		
		Mat edged(filtered_image.cols, filtered_image.rows, CV_8UC1);

		Canny(filtered_image, edged, 30, 200);

		findContours(edged, contours, out_array, RETR_TREE, CHAIN_APPROX_SIMPLE);
		
		sort(contours.begin(), contours.end(), [](const vector<Point>& a, const vector<Point>& b) { return a.size() > b.size(); });

		for (auto& contour : contours)
		{
			vector<Point> approx;
			double epsilon = 0.015 * arcLength(contour, true);
			approxPolyDP(contour, approx, epsilon, true);
			if (approx.size() == 4)
			{
				possible_plates.push_back(approx);
			}
		}
		
		sort(possible_plates.begin(), possible_plates.end(), [](const vector<Point>& a, const vector<Point>& b) { return boundingRect(a).area() > boundingRect(b).area(); });

		int largest_p_vec = 0, probable_plate_idx = 0;
		for (int i = 0; i < possible_plates.size(); i++)
		{
			Rect rect = boundingRect(possible_plates[i]);

			vector<vector<Point>> contours_p;
			vector<Vec4i> out_array_p;

			Mat canny_cropped = edged(rect);
			findContours(canny_cropped, contours_p, out_array_p, RETR_TREE, CHAIN_APPROX_SIMPLE);

			if (contours_p.size() > 0)
			{
				if (contours_p.size() > largest_p_vec)
				{
					largest_p_vec = contours_p.size();
					probable_plate_idx = i;
				}
			}
		}
		drawContours(originalimg, possible_plates, probable_plate_idx, Scalar(0, 0, 255), 3);

		imshow("final", originalimg);

		waitKey();

		i++;
		path = (("./plates/" + to_string(i) + ".jpg"));
	}
}

