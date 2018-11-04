#pragma once
#include <opencv2/opencv.hpp>
#include <cmath>

#define IMG(row)(col)  image.at<uchar>(row, col)
#define D_IMG(row)(col)  destination.at<uchar>(row, col)
#define S1_IMG(row)(col)  source1.at<uchar>(row, col)
#define S2_IMG(row)(col)  source2.at<uchar>(row, col)

#define DEPTH 256
#define MAX_DEPTH 255
#define MIN_DEPTH 0

#define RGB_CHANNELS 3
#define GRAY_CHANNEL 1

#define OVERFLOW_MANAGE true
#define OVERFLOW_IGNORE false

#define IGNORE '0'
#define MANAGE 0

#define SOBEL true
#define LAPLACIAN false

#define DILATION true
#define EROSION false

using namespace cv;

bool add(Mat& destination, Mat source1, Mat source2);
bool subtracte(Mat& destination, Mat source1, Mat source2);
void copy(Mat& destination, Mat source);
void transpose(int** square_matrix, int m_size);
bool or(Mat& destination, Mat source1, Mat source2);


Point rotate(double t, int y, int x);

uchar add(uchar element1, uchar element2);
uchar subtracte(uchar element1, uchar element2);

bool multiply(Mat& image, int ratio);
bool divide(Mat& image, int ratio);
bool modulo(Mat& image, int ratio);

void filter(Mat & image, double** mask, int m_size, bool overflow);
void filter_edge(Mat & image, int** mask, int m_size, bool type);
void filter_median(Mat & image, int m_size);	// for median filter

uchar filter_prevent_overflow(Mat source, double** mask, int x, int y, int m_size);
uchar filter_ignore_overflow(Mat source, double** mask, int x, int y, int m_size);
uchar filter_sobel(Mat source, int** mask, int x, int y, int m_size); // for sobel
int filter_laplacian(Mat source, int** mask, int x, int y, int m_size); // for sobel
uchar filter_median(Mat source, int x, int y, int m_size); // for median filter

void morphology(Mat& image, bool** mask, int m_size, bool type);
void dilation(Mat& destination, Mat source, bool** mask, int x, int y, int m_size);
void erosion(Mat& destination, Mat source, bool** mask, int x, int y, int m_size);
void fill_padding(Mat & image, int margin);

void swap(int& a, int& b);
int ceil(int n, int limit);
