#pragma once
#include "MatHandler.h"
using namespace std;

//////////////////////////////////////////////////////

#define HORIZONTAL true
#define VERTICAL false

#define PI 3.14159265

#define C_IMG(row)(col)  cur_image.at<uchar>(row, col)
#define P_IMG(row)(col)  pre_image.at<uchar>(row, col)

//////////////////////////////////////////////////////

bool open(string path);
void save();
void savs_as(string path);

//////////////////////////////////////////////////////

bool undo();

//////////////////////////////////////////////////////

bool rgb_to_gray();
bool inverse_vertical();
bool inverse_horizontal();
bool show_bit_plane(int pos);

//////////////////////////////////////////////////////

bool histogram_equalization();
bool stretch();
bool stretch(int min, int max);
bool negative();
bool thresholding(int t);
void thresholding();

//////////////////////////////////////////////////////

bool gaussian_blur(int m_size);
bool unsharp();
bool average_filter();
bool median_filter();

//////////////////////////////////////////////////////

bool sobel_masking();
bool laplacian_masking();

//////////////////////////////////////////////////////

bool mopology_opne();
bool mopology_close();

//////////////////////////////////////////////////////

bool resize(int width, int height);
bool expansion_interpolation(int width, int height);
bool reduce_interpolation(int width, int height);
bool rotation(double t);

//////////////////////////////////////////////////////

void show();
bool preprocesse();
int get_width();
int get_height();

//////////////////////////////////////////////////////

int power_of_x(int x, int n);	// show_bit_plane
void min_max(int& min, int& max);	// stretch
void sobel_masking(Mat& image, int** mask, int m_size, bool directiond);