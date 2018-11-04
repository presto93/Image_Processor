#include "MatHandler.h"


bool add(Mat & destination, Mat source1, Mat source2)
{
	if (destination.channels() * source1.channels() * source2.channels() != GRAY_CHANNEL) return false;
	int rows, cols;
	rows = source1.rows > source2.rows ? source1.rows : source2.rows;
	cols = source1.cols > source2.cols ? source1.cols : source2.cols;

	for (int col = 0; col < cols; col++)
		for (int row = 0; row < rows; row++)
			destination.at<uchar>(row, col) = add(source1.at<uchar>(row, col), source2.at<uchar>(row, col));
	return true;
}

bool subtracte(Mat & destination, Mat source1, Mat source2)
{
	if (destination.channels() * source1.channels() * source2.channels() != GRAY_CHANNEL) return false;
	int rows, cols;
	rows = source1.rows > source2.rows ? source1.rows : source2.rows;
	cols = source1.cols > source2.cols ? source1.cols : source2.cols;

	for (int col = 0; col < cols; col++)
		for (int row = 0; row < rows; row++)
			destination.at<uchar>(row, col) = subtracte(source1.at<uchar>(row, col), source2.at<uchar>(row, col));
	
	return true;
}

void copy(Mat & destination, Mat source)
{
	if(source.channels() == GRAY_CHANNEL)
		for (int col = 0; col < source.cols; col++)
			for (int row = 0; row < source.rows; row++)
				destination.at<uchar>(row, col) = source.at<uchar>(row, col);
		
	else for(int i = 0; i < source.channels(); i++)
		for (int col = 0; col < source.cols; col++)
			for (int row = 0; row < source.rows; row++)
				destination.at<Vec3b>(row, col)[i] = source.at<Vec3b>(row, col)[i];
}

void transpose(int ** square_matrix, int m_size)
{
	for (int row = 0; row < m_size; row++)
		for (int col = 0; col < row; col++)
			swap(square_matrix[row][col], square_matrix[col][row]);
}

bool or (Mat & destination, Mat source1, Mat source2)
{
	if (destination.channels() * source1.channels() * source2.channels() != GRAY_CHANNEL) return false;
	if (destination.rows != source1.rows || destination.cols != source1.cols || destination.rows != source2.rows || destination.cols != source2.cols) return false;

	for (int col = 0; col < destination.cols; col++)
		for (int row = 0; row < destination.rows; row++)
			destination.at<uchar>(row, col) = source1.at<uchar>(row, col) > source2.at<uchar>(row, col) ? source1.at<uchar>(row, col) : source2.at<uchar>(row, col);

	return true;
}

Point rotate(double t, int y, int x)
{
	return Point(x * cos(t) - y * sin(t), x * sin(t) + y * cos(t));
}

uchar add(uchar element1, uchar element2)
{
	uint temp = element1 + element2;
	if (temp > MAX_DEPTH) return MAX_DEPTH;
	return temp;
}

uchar subtracte(uchar element1, uchar element2)
{
	if (element1 < element2) return 0;
	return element1 - element2;
}

bool multiply(Mat & image, int ratio)
{
	if (image.channels() != GRAY_CHANNEL) return false;
	for (int col = 0; col < image.cols; col++)
		for (int row = 0; row < image.rows; row++)
			image.at<uchar>(row, col) *= ratio;
	return true;
}

bool divide(Mat & image, int ratio)
{
	if (image.channels() != GRAY_CHANNEL) return false;
	for (int col = 0; col < image.cols; col++)
		for (int row = 0; row < image.rows; row++)
			image.at<uchar>(row, col) /= ratio;
	return true;
}

bool modulo(Mat & image, int ratio)
{
	if (image.channels() != GRAY_CHANNEL) return false;
	for (int col = 0; col < image.cols; col++)
		for (int row = 0; row < image.rows; row++)
			image.at<uchar>(row, col) %= ratio;
	return true;
}

void filter(Mat & image, double** mask, int m_size, bool overflow)
{
	Mat temp(image.rows + m_size - 1, image.cols + m_size -1, CV_8UC1);
	const int margin = m_size / 2;

	// 패딩이미지(temp) 생성
	for (int col = 0; col < image.cols; col++)
		for (int row = 0; row < image.rows; row++)
			temp.at<uchar>(row + margin, col + margin) = image.at<uchar>(row, col);
	fill_padding(temp, margin);
	// 패딩이미지(temp) 생성 완료
	// 필터 적용
	switch (overflow)
	{
	case OVERFLOW_IGNORE:
		for (int row = 0; row < image.rows; row++)
			for (int col = 0; col < image.cols; col++)
				image.at<uchar>(row, col) = filter_prevent_overflow(temp, mask, col, row, m_size);
		break;
	case OVERFLOW_MANAGE:
		for (int row = 0; row < image.rows; row++)
			for (int col = 0; col < image.cols; col++)
				image.at<uchar>(row, col) = filter_ignore_overflow(temp, mask, col, row, m_size);
		break;
	}
	// 필터적용 완료

	temp.release();
}

void filter_edge(Mat & image, int ** mask, int m_size, bool type)
{

	Mat temp(image.rows + m_size - 1, image.cols + m_size - 1, CV_8UC1);
	const int margin = m_size / 2;

	// 패딩이미지(temp) 생성
	for (int col = 0; col < image.cols; col++)
		for (int row = 0; row < image.rows; row++)
			temp.at<uchar>(row + margin, col + margin) = image.at<uchar>(row, col);
	fill_padding(temp, margin);
	// 패딩이미지(temp) 생성 완료
	// 필터 적용
	freopen("out.txt", "w", stdout);
	switch(type)
	{
	case SOBEL :
		for (int row = 0; row < image.rows; row++)
			for (int col = 0; col < image.cols; col++)
				image.at<uchar>(row, col) = filter_sobel(temp, mask, col, row, m_size);
		break;
	case LAPLACIAN :
		for (int row = 0; row < image.rows; row++)
			for (int col = 0; col < image.cols; col++)
			{
				image.at<uchar>(row, col) = filter_laplacian(temp, mask, col, row, m_size);
				//printf("%d\n", filter_laplacian(temp, mask, col, row, m_size));
			}
		break;
	}

	// 필터적용 완료

	temp.release();
}

void filter_median(Mat & image, int m_size)
{
	Mat temp(image.rows + m_size - 1, image.cols + m_size - 1, CV_8UC1);
	const int margin = m_size / 2;

	// 패딩이미지(temp) 생성
	for (int col = 0; col < image.cols; col++)
		for (int row = 0; row < image.rows; row++)
			temp.at<uchar>(row + margin, col + margin) = image.at<uchar>(row, col);
	fill_padding(temp, margin);

	// 패딩이미지(temp) 생성 완료
	// 필터 적용
	for (int row = 0; row < image.rows; row++)
		for (int col = 0; col < image.cols; col++)
			image.at<uchar>(row, col) = filter_median(temp, col, row, m_size);
	// 필터적용 완료

	temp.release();
}

uchar filter_ignore_overflow(Mat source, double ** mask, int x, int y, int m_size)
{
	double temp = 0;
	for (int row = 0; row < m_size; row++)
		for (int col = 0; col < m_size; col++)
			temp += (double)source.at<uchar>(y + row, x + col) * mask[row][col];

	return (uchar)temp;
}

uchar filter_sobel(Mat source, int ** mask, int x, int y, int m_size)
{
	int temp = 0;
	for (int row = 0; row < m_size; row++)
		for (int col = 0; col < m_size; col++)
			temp += (double)source.at<uchar>(y + row, x + col) * mask[row][col];
	
	return abs(temp) >> 2;
}

int filter_laplacian(Mat source, int ** mask, int x, int y, int m_size)
{
	int k = 0;
	for (int row = 0; row < m_size; row++)
		for (int col = 0; col < m_size; col++)
			k += (double)source.at<uchar>(y + row, x + col) * mask[row][col];
	return abs(k) << 1;
	return ceil(k, 128) + (ceil(k, 128) / 128) * (ceil(k, 256) - 127) / 2 + (ceil(k, 256) / 256) * (ceil(k, 512) - 255) / 8 + (k / 512) * (k - 512) / 16;
//	return k & 0b00111111 + ((k >> 1) & 0b01000000) | (k & 0b01000000) + ((k >> 3) & 0b10000000) | ((k >> 2) & 0b10000000) | ((k >> 1) & 0b10000000);
//	return ((k & 0xFF) / 2 + ((k & 0x1FF) / 256) * ((k % 512) - 255) / 4 + ((k & 0x2FF) / 512) * ((k % 1024) - 511) / 16 + (k / 1024) * (k - 1023) / 32);
	//return (ceil(k, 256) / 2 + (ceil(k, 256) / 256) * (ceil(k, 512) - 255) / 4 + (ceil(k, 512) / 512) * (ceil(k, 1024) - 511) / 16 + (k / 1024) * (k - 1024) / 32)*2;
}

uchar filter_prevent_overflow(Mat source, double** mask, int x, int y, int m_size)
{
	double temp = 0;
	for (int row = 0; row < m_size; row++)
		for (int col = 0; col < m_size; col++)
			temp += (double)source.at<uchar>(y + row, x + col) * mask[row][col];

	if (temp > MAX_DEPTH) return MAX_DEPTH;
	else if (temp < MIN_DEPTH) return MIN_DEPTH;
	else return (uchar)temp;
}

uchar filter_median(Mat source, int x, int y, int m_size)
{
	uchar pixels[10];
	int j, length = 9;
	pixels[0] = 0;
	for (int i = 0; i < length; i++)
	{
		uchar temp = source.at<uchar>(y + (i / m_size), x + (i % m_size));
		for (j = i; pixels[j] > temp; j--) pixels[j + 1] = pixels[j];
		pixels[j + 1] = temp;
	}
	return pixels[5];	// median_pos;
}

void morphology(Mat & image, bool ** mask, int m_size, bool type)
{
	Mat temp(image.rows, image.cols, CV_8UC1);
	copy(temp, image);


	for (int row = 0; row < image.rows; row++)
		for (int col = 0; col < image.cols; col++)
			image.at<uchar>(row, col) = 0;

	int rows = image.rows - 1;
	int cols = image.cols - 1;
	switch (type)
	{
	case DILATION:
		for (int row = 1; row < rows; row++)
			for (int col = 1; col < cols; col++)
				if(temp.at<uchar>(row, col))
					dilation(image, temp, mask, col, row, m_size);
		break;
	case EROSION:
		for (int row = 1; row < rows; row++)
			for (int col = 1; col < cols; col++)
				if(temp.at<uchar>(row, col)) erosion(image, temp, mask, col, row, m_size);
		break;
	}
	// 필터적용 완료

	temp.release();
}

void dilation(Mat& destination, Mat source, bool ** mask, int x, int y, int m_size)
{
	for (int row = 0; row < m_size; row++)
		for (int col = 0; col < m_size; col++)
			destination.at<uchar>(y + row - 1, x + col - 1) |= source.at<uchar>(y + row - 1, x + col - 1) | mask[row][col];

}

void erosion(Mat& destination, Mat source, bool ** mask, int x, int y, int m_size)
{
	bool temp = 1;
	for (int row = 0; row < m_size; row++)
		for (int col = 0; col < m_size; col++)
			if (mask[row][col]) temp &= source.at<uchar>(y + row - 1, x + col - 1)*temp;
	if (temp) destination.at<uchar>(y - 1, x - 1) = 1;
}

void fill_padding(Mat & image, int margin)
{
	int width = image.cols - 1, height = image.rows - 1;
	for (int row = 0; row < margin; row++)
		for (int col = 0; col < image.cols; col++)
		{
			image.at<uchar>(row, col) = image.at<uchar>(margin, col);
			image.at<uchar>(height - row, col) = image.at<uchar>(height - margin - row, col);
		}
	for (int col = 0; col < margin; col++)
	{
		for (int row = 0; row < image.rows; row++)
		{
			image.at<uchar>(row, col) = image.at<uchar>(row, margin);
			image.at<uchar>(row, width - col) = image.at<uchar>(row, width - margin - col);
		}
	}
}

void swap(int & a, int & b)
{
	int temp = a;
	a = b;
	b = temp;
}

int ceil(int n, int limit) { return n > limit ? limit : n; }
