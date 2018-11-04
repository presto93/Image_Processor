#include <opencv2/opencv.hpp>

using namespace cv;

void main()
{
	Mat image = imread("heeyong.jpg", CV_LOAD_IMAGE_UNCHANGED);

	Mat bImage(image);

	Mat cImage = image.clone();

	Point s1(0, 0);
	Point e1(100, 100);
	line(bImage, s1, e1, Scalar(255, 0, 0), 2);


	Point s2(100, 100);
	Point e2(200, 200);
	line(cImage, s2, e2, Scalar(0, 0, 255), 2);

	imshow("image", image);
	imshow("b image", bImage);
	imshow("c image", cImage);
}