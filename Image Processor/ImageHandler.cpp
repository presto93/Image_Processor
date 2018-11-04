#include "ImageHandler.h"

Mat cur_image, pre_image;
string filename;
bool undoable;

enum { BLUE, GREEN, RED };
bool open(string path)
{
	undoable = false;
	cur_image = imread(path, CV_LOAD_IMAGE_UNCHANGED);
	pre_image = imread(path, CV_LOAD_IMAGE_UNCHANGED);
	if (cur_image.empty()) return false;
	filename = path;
	//show();
	printf("succeed open\n");
	return true;
}

void save()
{
	imwrite(filename, cur_image);
}

void savs_as(string path)
{
	imwrite(path, cur_image);
}

//////////////////////////////////////////////////////

bool undo()
{
	if (!undoable) return false;

	if (pre_image.channels() == RGB_CHANNELS || pre_image.cols != cur_image.cols || pre_image.rows != cur_image.rows)
	{
		cur_image.release();
		cur_image.create(pre_image.size(), CV_8UC1);
	}
	copy(cur_image, pre_image);
	undoable = false;
	show();
	return true;
}

//////////////////////////////////////////////////////

bool rgb_to_gray()
{
	if (cur_image.channels() != RGB_CHANNELS) return false;
	copy(pre_image, cur_image);
	cur_image.release();
	cur_image.create(pre_image.size(), CV_8UC1);
	for (int col = 0; col < cur_image.cols; col++)
		for (int row = 0; row < cur_image.rows; row++)
			cur_image.at<uchar>(row, col) = (pre_image.at<Vec3b>(row, col)[RED] + pre_image.at<Vec3b>(row, col)[GREEN] + pre_image.at<Vec3b>(row, col)[BLUE]) / 3;
	show();
	return true;
}

bool inverse_vertical()
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;

	int height = cur_image.rows - 1;
	for (int col = 0; col < cur_image.cols; col++)
		for (int row = 0; row < cur_image.rows; row++)
			cur_image.at<uchar>(row, col) = pre_image.at<uchar>(height - row, col);
	show();
	return true;
}

bool inverse_horizontal()
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;

	int width = cur_image.cols - 1;
	for (int col = 0; col < cur_image.cols; col++)
		for (int row = 0; row < cur_image.rows; row++)
			cur_image.at<uchar>(row, col) = pre_image.at<uchar>(row, width - col);
	show();
	return true;
}

bool show_bit_plane(int pos)
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;
	if (pos > 7 || pos < 0) return false;

	for (int col = 0; col < cur_image.cols; col++)
		for (int row = 0; row < cur_image.rows; row++)
			cur_image.at<uchar>(row, col) = (pre_image.at<uchar>(row, col) >> pos) & 1;

	thresholding();
	show();
	return true;
}

//////////////////////////////////////////////////////

bool histogram_equalization()
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;

	int intensity[DEPTH];

	for (int i = 0; i < DEPTH; i++) intensity[i] = 0;

	for (int col = 0; col < cur_image.cols; col++)
		for (int row = 0; row < cur_image.rows; row++) intensity[cur_image.at<uchar>(row, col)]++;

	for (int i = 1; i < DEPTH; i++) intensity[i] += intensity[i - 1];

	float average_of_intensitis = intensity[MAX_DEPTH] / (MAX_DEPTH);

	for (int i = 0; i < DEPTH; i++) intensity[i] /= average_of_intensitis;

	for (int col = 0; col < cur_image.cols; col++)
		for (int row = 0; row < cur_image.rows; row++)
			cur_image.at<uchar>(row, col) = intensity[cur_image.at<uchar>(row, col)];
	show();
	return true;
}

bool stretch()
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;
	int min, max;
	min_max(min, max);
	stretch(min, max);
	return true;
}

bool stretch(int min, int max)
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;
	int intensity[DEPTH], range = max - min;

	for (int i = MIN_DEPTH; i < min; i++) intensity[i] = 0;
	for (int i = min; i < max; i++) intensity[i] = (i - min) * (MAX_DEPTH) / range;
	for (int i = max; i < DEPTH; i++) intensity[i] = MAX_DEPTH;

	for (int col = 0; col < cur_image.cols; col++)
		for (int row = 0; row < cur_image.rows; row++) cur_image.at<uchar>(row, col) = intensity[cur_image.at<uchar>(row, col)];
	show();
	return true;
}

bool negative()
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;
	for (int col = 0; col < cur_image.cols; col++)
		for (int row = 0; row < cur_image.rows; row++)
			cur_image.at<uchar>(row, col) = (MAX_DEPTH) - cur_image.at<uchar>(row, col);
	show();
	return true;
}

bool thresholding(int t)
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;
	if (t > MAX_DEPTH || t < MIN_DEPTH) return false;

	for (int col = 0; col < cur_image.cols; col++)
		for (int row = 0; row < cur_image.rows; row++)
			cur_image.at<uchar>(row, col) = cur_image.at<uchar>(row, col) > t ? MAX_DEPTH : MIN_DEPTH;

	show();
	return true;
}

void thresholding()
{
	for (int col = 0; col < cur_image.cols; col++)
		for (int row = 0; row < cur_image.rows; row++)
			cur_image.at<uchar>(row, col) = cur_image.at<uchar>(row, col) > 0 ? MAX_DEPTH : MIN_DEPTH;
}

//////////////////////////////////////////////////////

bool gaussian_blur(int m_size)
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;
	double** mask = new double*[m_size];
	const int cx = m_size / 2, cy = m_size / 2;
	double sum = 0;
	for (int y = 0; y < m_size; y++)
	{
		mask[y] = new double[m_size];
		for (int x = 0; x < m_size; x++)
		{
			mask[y][x] = 1.0 / power_of_x(2, abs(cx - x) + abs(cy - y) + 2);
			sum += mask[y][x];
		}
	}
	for (int y = 0; y < m_size; y++) for (int x = 0; x < m_size; x++) mask[y][x] /= sum;
	filter(cur_image, mask, m_size, OVERFLOW_MANAGE);

	show();
	return true;
}

bool unsharp()
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;
	const int m_size = 3;
	double** mask = new double*[m_size];
	const double alpha = 0.5;
	const int cx = m_size / 2, cy = m_size / 2;
	double look_up_table[] = { alpha + 5, alpha - 1, alpha * -1 };
	for (int y = 0; y < m_size; y++)
	{
		mask[y] = new double[m_size];
		for (int x = 0; x < m_size; x++) mask[y][x] = look_up_table[abs(cx - x) + abs(cy - y)] / (alpha + 1);
	}
	filter(cur_image, mask, m_size, OVERFLOW_MANAGE);

	show();
	return true;
}

bool average_filter()
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;

	const int m_size = 3;
	double** mask = new double*[m_size];
	for (int y = 0; y < m_size; y++)
	{
		mask[y] = new double[m_size];
		for (int x = 0; x < m_size; x++) mask[y][x] = 1.0 / (m_size * m_size);
	}
	filter(cur_image, mask, m_size, OVERFLOW_IGNORE);
	show();
	return true;
}

bool median_filter()
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;

	const int m_size = 3;
	filter_median(cur_image, m_size);
	show();
	return true;
}

bool sobel_masking()
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;
	const int m_size = 3;
	int** mask = new int*[m_size];
	for (int y = 0; y < m_size; y++)
	{
		mask[y] = new int[m_size];
		for (int x = 0; x < m_size; x++) mask[y][x] = power_of_x(2, y % 2) * (x - 1);
	}
	Mat x_image, y_image;
	sobel_masking(y_image, mask, m_size, VERTICAL);
	sobel_masking(x_image, mask, m_size, HORIZONTAL);
	or (cur_image, x_image, y_image);
	show();
	return true;
}

bool laplacian_masking()
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;
	const int m_size = 3;
	int** mask = new int*[m_size];
	const int cx = m_size / 2, cy = m_size / 2;
	double look_up_table[] = { -4, 1, 0 };
	for (int y = 0; y < m_size; y++)
	{
		mask[y] = new int[m_size];
		for (int x = 0; x < m_size; x++)
		{
			mask[y][x] = look_up_table[abs(cx - x) + abs(cy - y)];
			printf("%d\t", mask[y][x]);
		}
		printf("\n");
	}

	filter_edge(cur_image, mask, m_size, LAPLACIAN);
	show();
	return true;
}

//////////////////////////////////////////////////////

bool mopology_opne()
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;
	const int m_size = 3;
	bool** mask = new bool*[m_size];
	for (int i = 0; i < m_size; i++)
	{
		mask[i] = new bool[m_size];
		for (int j = 0; j < m_size; j++) mask[i][j] = 1;
	}
	morphology(cur_image, mask, m_size, EROSION);
	morphology(cur_image, mask, m_size, DILATION);
	thresholding();
	show();
	return true;
}

bool mopology_close()
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;
	const int m_size = 3;
	bool** mask = new bool*[m_size];
	for (int i = 0; i < m_size; i++)
	{
		mask[i] = new bool[m_size];
		for (int j = 0; j < m_size; j++) mask[i][j] = 1;
	}
	morphology(cur_image, mask, m_size, DILATION);
	morphology(cur_image, mask, m_size, EROSION);
	thresholding();
	show();
	return true;
}

bool resize(int width, int height)
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;

	cur_image.release();
	cur_image.create(height, width, CV_8UC1);

	double r_y = (double) pre_image.rows / height;
	double r_x = (double) pre_image.cols / width;
	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++)
			cur_image.at<uchar>(row, col) = pre_image.at<uchar>(row * r_y, col * r_x);
	show();
	return true;
}

bool expansion_interpolation(int width, int height)
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;

	double r_y = (double)height / pre_image.rows;
	double r_x = (double)width / pre_image.cols;

	cur_image.release();
	cur_image.create(height, width, CV_8UC1);
	for (int row = 0; row < height; row++)
	{
		int y1 = row / r_y;
		int y2 = MIN(pre_image.rows - 1, y1 + 1);
		double dy1 = 1 - (row - y1 * r_y) / r_y;
		double dy2 = 1 - dy1;
		for (int col = 0; col < width; col++)
		{
			int x1 = col / r_x;
			int x2 = MIN(pre_image.cols - 1, x1 + 1);
			double dx1 = 1 - (col - x1 * r_x) / r_x;
			double dx2 = 1 - dx1;

			uchar east = pre_image.at<uchar>(y1, x1) * dy1 + pre_image.at<uchar>(y2, x1) * dy2;
			uchar west = pre_image.at<uchar>(y1, x2) * dy1 + pre_image.at<uchar>(y2, x2) * dy2;
			uchar north = pre_image.at<uchar>(y1, x1) * dx1 + pre_image.at<uchar>(y1, x2) * dx2;
			uchar south = pre_image.at<uchar>(y2, x1) * dx1 + pre_image.at<uchar>(y2, x2) * dx2;
	
			cur_image.at<uchar>(row, col) = (east * dx1 + west * dx2 + north * dy1 + south * dy2) / 2;
		}
	}
	show();
	return true;
}
bool reduce_interpolation(int width, int height)
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;

	double r_y = (double) pre_image.rows / height;
	double r_x = (double) pre_image.cols / width;

	cur_image.release();
	cur_image.create(height, width, CV_8UC1);
	for (int row = 0; row < height; row++)
	{
		int y1 = row * r_y;
		int y2 = MIN(pre_image.rows - 1, y1 + r_y);
		double c_y = (y1 + y2) / 2;
		for (int col = 0; col < width; col++)
		{
			int x1 = col * r_x;
			int x2 = MIN(pre_image.cols - 1, x1 + r_x);
			double c_x = (x1 + x2) / 2;
			int temp = 0;
			for (int r = y1; r < y2; r++)
			{
				for (int c = x1; c < x2; c++)
				{
					temp += pre_image.at<uchar>(r, c) * (1 - 2 * abs(c - c_x) / r_x) + pre_image.at<uchar>(r, c) * (1 - 2 * abs(r - c_y) / r_y);
				}
			}
			cur_image.at<uchar>(row, col) = temp / (r_x*r_y);
		}
	}
	show();
	return true;
}
bool rotation(double t)
{
	if (cur_image.channels() == RGB_CHANNELS) return false;
	if (!preprocesse()) return false;
	t *= PI / 180;
	int x = cur_image.cols / 2;
	int y = cur_image.rows / 2;
	Point a = rotate(t, y, x);
	Point b = rotate(t, y * -1, x);
	int height = MAX(abs(a.y), abs(b.y)) * 2;
	int width = MAX(abs(a.x), abs(b.x)) * 2;
	int pre_h = pre_image.rows - 1;
	int pre_w = pre_image.cols - 1;
	cur_image.release();
	cur_image.create(height, width, CV_8UC1);
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			Point p = rotate(t, row - height / 2, col - width /2);
			p.x += pre_w / 2;
			p.y += pre_h / 2;
			if (p.x > pre_w || p.x < 0 || p.y > pre_h || p.y < 0) cur_image.at<uchar>(row, col) = 0;
			else cur_image.at<uchar>(row, col) = pre_image.at<uchar>(p.y, p.x);
		}
	}
	show();
	return true;
}

//////////////////////////////////////////////////////

void show()
{
	namedWindow(filename);
	imshow(filename, cur_image);
	waitKey(0);
	destroyWindow(filename);
}

bool preprocesse()
{
	if (pre_image.channels() == RGB_CHANNELS || pre_image.cols != cur_image.cols || pre_image.rows != cur_image.rows)
	{
		pre_image.release();
		pre_image.create(cur_image.size(), CV_8UC1);
	}
	else if (cur_image.channels() != GRAY_CHANNEL) return false;

	undoable = true;
	copy(pre_image, cur_image);
	return true;
}

int get_width()
{
	return cur_image.cols;
}

int get_height()
{
	return cur_image.rows;
}

int power_of_x(int x, int n)
{
	int result = 1;
	for (int i = n; i > 0; i--) result *= x;
	return result;
}

void min_max(int& min, int& max)
{
	min = MAX_DEPTH; max = MIN_DEPTH;
	for (int col = 0; col < cur_image.cols; col++)
		for (int row = 0; row < cur_image.rows; row++)
		{
			uchar cur_value = cur_image.at<uchar>(row, col);
			if (cur_value > max) max = cur_value;
			else if (cur_value < min) min = cur_value;
		}
}

void sobel_masking(Mat& image, int** mask, int m_size, bool direction)
{
	image.create(cur_image.size(), CV_8UC1);
	copy(image, cur_image);
	if (direction) transpose(mask, m_size);
	filter_edge(image, mask, m_size, SOBEL);
}