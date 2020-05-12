#include "CannyEdgeDetector.h"

int CannyEdgeDetector::Apply(const Mat& srcImage, Mat& dstImage)
{
	// Kiểm tra ảnh có tồn tại hay không
	if (srcImage.empty())
		return 0;

	// Lần lượt là kích thước nhân và giá trị sigma
	int kernel_size = 5;
	float sigma = 2;

	// Tính nhân gaussian 
	Mat g_kernel = gaussian_kernel(kernel_size, sigma);

	// Dùng gaussian để làm giảm bớt ảnh hưởng của nhiễu, áp dụng để làm mượt ảnh
	Mat img_smoothed = doconvolve(srcImage, g_kernel);

	// Áp dụng mặt nạ sobel để tìm biên độ và hưởng gradient của ảnh
	Mat G, angle; // độ lớn và gốc (magnitude and slope)
	sobel_filters(img_smoothed, G, angle);

	// Loại bỏ những điểm không cực đại
	dstImage = non_max_suppression(G, angle);

	// Lấy ngưỡng kép cho ảnh
	threshold(dstImage);

	// Ta sẽ xét lại các điểm xám nào là biên cạnh
	dstImage = hysteresis(dstImage);

	return 1;
}

Mat CannyEdgeDetector::doconvolve(const Mat& srcImage, const Mat& kernel)
{
	Mat image;
	srcImage.convertTo(image, CV_32FC1, 1.0 / 255.0);
	int rows = image.rows, cols = image.cols;
	int kernel_size = kernel.rows; // Ta sẽ kích thước nhân luôn là hình vuông
	Mat img_smoothed = Mat(rows, cols, CV_32FC1, Scalar(0));
	vector<vector<float>> tx(kernel_size, vector<float>(kernel_size)), ty(kernel_size, vector<float>(kernel_size));

	for (int i = 0; i < kernel_size; i++)
		for (int j = 0; j < kernel_size; j++)
		{
			tx[i][j] = i - (kernel_size / 2);
			ty[i][j] = j - (kernel_size / 2);
		}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			float convolution = 0;
			for (int u = 0; u < kernel_size; u++)
			{
				for (int v = 0; v < kernel_size; v++)
				{
					int i_new = i + tx[u][v];
					int j_new = j + ty[u][v];
					if ((i_new < 0) || (j_new < 0) || (i_new >= rows) || (j_new >= cols))
						continue;
					convolution += image.at<float>(i_new, j_new) * kernel.at<float>(u, v);
				}
			}
			img_smoothed.at<float>(i, j) = convolution;
		}
	}
	return img_smoothed;
}

Mat CannyEdgeDetector::gaussian_kernel(int kernel_size, float sigma)
{
	// Kernel_size = 2*k + 1 => k = kernel_size / 2
	int size = kernel_size / 2;
	Mat g = Mat(kernel_size, kernel_size, CV_32FC1, Scalar(0));
	vector<vector<float>> tx(kernel_size, vector<float>(kernel_size)), ty(kernel_size, vector<float>(kernel_size));

	for (int i = 0; i < kernel_size; i++)
		for (int j = 0; j < kernel_size; j++)
		{
			tx[i][j] = i - size;
			ty[i][j] = j - size;
		}

	// Tính bộ lọc Gaussian 
	for (int i = 0; i < kernel_size; i++)
	{
		for (int j = 0; j < kernel_size; j++)
		{
			float x = tx[i][j], y = ty[i][j];
			float normal = 1.0 / (2.0 * M_PI * sigma * sigma);
			g.at<float>(i, j) = exp(-((x * x + y * y) / (2.0 * sigma * sigma))) * normal;
		}
	}
	return g;
}

void CannyEdgeDetector::sobel_filters(const Mat& img, Mat& G, Mat& angle)
{
	// Khởi tạo mặt nạ sobel
	float Wx[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
	float Wy[3][3] = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			Wx[i][j] /= 4;
			Wy[i][j] /= 4;
		}
	Mat Kx = Mat(3, 3, CV_32FC1, Wx);
	Mat Ky = Mat(3, 3, CV_32FC1, Wy);

	// Tích chập mặt nạ sobel với ảnh
	Mat Ix = doconvolve(img, Kx);
	Mat Iy = doconvolve(img, Ky);

	// Lấy kích thước ảnh sau khi tích chập
	int cols = Ix.cols, rows = Ix.rows;

	// Khởi tạo ma trận độ lớn, hướng gradient của ảnh
	G = Mat(rows, cols, CV_32FC1, Scalar(0));
	angle = Mat(rows, cols, CV_32FC1, Scalar(0));

	float Max = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			// Tính độ lớn gradient
			G.at<float>(i, j) = sqrt(Ix.at<float>(i, j) * Ix.at<float>(i, j) + Iy.at<float>(i, j) * Iy.at<float>(i, j));

			// Tính hướng gradient
			angle.at<float>(i, j) = atan(Iy.at<float>(i, j) / Ix.at<float>(i, j));

			// Tìm giá trị điểm ảnh lớn nhất của ảnh
			if (Max < G.at<float>(i, j))
				Max = G.at<float>(i, j);
		}
	}
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			// Tránh trường hợp ảnh quá tối, các giá trị điểm ảnh quá nhỏ
			// Ta sẽ tỉ lệ lại các giá trị điểm ảnh so với giá trị điểm ảnh lớn nhất nhưng vẫn giữ lại được bản chất của ảnh
			G.at<float>(i, j) = G.at<float>(i, j) * 255 / Max;

			// Chuyển góc gradient thành số
			angle.at<float>(i, j) = angle.at<float>(i, j) * 180 / M_PI;
			if (angle.at<float>(i, j) < 0)
				angle.at<float>(i, j) += 180;
		}
}

Mat CannyEdgeDetector::non_max_suppression(const Mat& img, const Mat& angle)
{
	// Lấy kích thước của ảnh
	int rows = img.rows, cols = img.cols;
	Mat dstImage = Mat(rows, cols, CV_8UC1, Scalar(0));

	for (int i = 1; i < rows - 1; i++)
		for (int j = 1; j < cols - 1; j++)
		{
			int q = 255, r = 255;
			float theta = angle.at<float>(i, j);

			// Dựa vào hướng gradient tại vị trí (i, j) với 8 điểm xung quanh nó
			// => Ta chia thành góc như sau:
			// 0 <= x < 22.5, 157.5 <= x <= 180
			// 22.5 <= x < 67.5
			// 67.5 <= x < 112.5
			// 112.5 <= x < 157.5
			// angle = 0
			if (0 <= theta < 22.5 || 157.5 <= theta <= 180)
			{
				q = img.at<float>(i, j + 1);
				r = img.at<float>(i, j - 1);
			}
			//angle = 45
			if (22.5 <= theta < 67.5)
			{
				q = img.at<float>(i + 1, j - 1);
				r = img.at<float>(i - 1, j + 1);
			}
			// angle = 90
			if (67.5 <= theta < 112.5)
			{
				q = img.at<float>(i + 1, j);
				r = img.at<float>(i - 1, j);
			}
			// angle = 135
			if (112.5 <= theta < 157.5)
			{
				q = img.at<float>(i - 1, j - 1);
				r = img.at<float>(i + 1, j + 1);
			}

			// Nếu tại vị trí điểm ảnh đó nó lớn hơn 2 thằng theo hướng gradient của nó thì nó là điểm cực đại
			// => Ta sẽ giữ lại, còn không thì ta sẽ bỏ
			// => Giúp ta loại bỏ các biên giả, thu được đường biên mảnh
			if (img.at<float>(i, j) >= q && img.at<float>(i, j) >= r)
				dstImage.at<uchar>(i, j) = img.at<float>(i, j);
			else
				dstImage.at<uchar>(i, j) = 0;
		}

	return dstImage;
}

void CannyEdgeDetector::threshold(Mat& img)
{
	// Lấy kích thước của ảnh
	int rows = img.rows, cols = img.cols;

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			// Giá trị điểm ảnh nhỏ hơn ngưỡng thấp
			// => Không phải điểm biên cạnh (đen)
			if (img.at<uchar>(i, j) < _lowThreshold)
				img.at<uchar>(i, j) = 0;

			// Giá trị điểm ảnh lớn hơn ngưỡng lớn
			// => Là điểm biên cạnh (trắng)
			if (img.at<uchar>(i, j) > _highThreshold)
				img.at<uchar>(i, j) = 255;

			// Giá trị điểm ảnh nằm giữa ngưỡng kép
			// => Điểm cần xét (xám)
			if (img.at<uchar>(i, j) >= _lowThreshold && img.at<uchar>(i, j) <= _highThreshold)
				img.at<uchar>(i, j) = 75;
		}
}

Mat CannyEdgeDetector::hysteresis(const Mat& srcImage)
{
	// Khởi tạo
	Mat img = srcImage.clone();
	int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	vector<pair<int, int>> que;

	// Lấy kích thước của ảnh
	int rows = img.rows, cols = img.cols;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			if (img.at<uchar>(i, j) == 255)
				que.push_back({ i, j });
	// Truyển vào tọa độ những điểm là biên cạnh vào hàng đợi

	// Sử dụng thuật duyệt đồ thị theo chiều rộng (BFS) để tìm xác định lại các điểm xám nào là biên cạnh
	int l = 0, r = que.size() - 1;
	while (l <= r)
	{
		pair<int, int> u = que[l];
		for (int i = 0; i < 8; i++)
		{
			int x = u.first + dx[i];
			int y = u.second + dy[i];
			if (x >= 0 && y >= 0 && x < rows && y < cols)
			{
				if (img.at<uchar>(x, y) == 75)
				{
					img.at<uchar>(x, y) = 255;
					que.push_back({ x, y });
					r++;
				}
			}
		}
		l++;
	}

	// Loại bỏ các điểm xám không phải là biên cạnh
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			if (img.at<uchar>(i, j) != 255)
				img.at<uchar>(i, j) = 0;

	// Extension - Loại bỏ các điểm biên nhiễu
	int threshold = 40;
	vector<vector<bool>> checking(rows, vector<bool>(cols, false));
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			if (checking[i][j] == false)
			{
				checking[i][j] = true;
				int l = 0, r = 0, Count = 0;
				vector< pair<int, int> > que;
				que.push_back({ i, j });
				while (l <= r)
				{
					pair<int, int> u = que[l];
					for (int k = 0; k < 8; k++)
					{
						int x = dx[k] + u.first;
						int y = dy[k] + u.second;
						if (x >= 0 && y >= 0 && x < rows && y < cols)
							if (img.at<uchar>(x, y) == 255 && checking[x][y] == false)
							{
								que.push_back({ x, y });
								checking[x][y] = true;
								r++;
								Count++;
							}
					}
					l++;
				}
				if (Count < threshold)
				{
					for (int k = 0; k < que.size(); k++)
						img.at<uchar>(que[k].first, que[k].second) = 0;
				}
			}

	return img;
}