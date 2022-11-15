#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

cv::Mat src, dst;
int x[4], y[4], histogram[256];
int maxhist(int histogram[256]);

void imhist(cv::Mat& image, int histogram[256])
{
    for (int i = 0; i < 256; i++)
    {
        histogram[i] = 0;
    }
    for (int x = 0; x < image.cols; x++)
    {
        for (int y = 0; y < image.rows; y++)
        {
            histogram[(int)image.at<uchar>(y, x)]++;
        }
    }
}

int maxhist(int histogram[256])
{
    int max = histogram[0];
    for (int i = 0; i < 256; i++)
    {
        if (max <= histogram[i])
        {
            max = histogram[i];
        }
    }
    return max;
}

void MappingFunction(cv::Mat& src, cv::Mat& dst)
{
    cv::Mat table(1, 256, CV_8U);
    uchar* p = table.ptr();
    int index = 0;
    for (int i = 0; i < 256; i++)
    {
        if (histogram[i] == maxhist(histogram))
        {
            index = i;
        }
    }
    x[0] = index - 7;
    x[1] = index + 35;
    x[2] = index + 55;
    x[3] = index + 85;
    y[0] = 0;
    y[1] = x[1] * 2;
    y[2] = x[2] * 2;
    y[3] = 255;
    for (int i = 0; i < x[0]; i++)
    {
        p[i] = (uchar)0;
    }
    float a0, b0, a1, b1, a2, b2;
    for (int i = x[0]; i < x[1]; i++)
    {
        a0 = ((float)(y[1] - y[0]) / (float)(x[1] - x[0]));
        b0 = y[0] - a0 * x[0];
        p[i] = (uchar)(a0 * i + b0);
    }
    std::cout << a0 << "\t" << b0 << std::endl;
    for (int i = x[1]; i < x[2]; i++)
    {
        a1 = ((float)(y[2] - y[1]) / (float)(x[2] - x[1]));
        b1 = y[1] - a1 * x[1];
        p[i] = (uchar)(a1 * i + b1);
    }
    std::cout << a1 << "\t" << b1 << std::endl;
    for (int i = x[2]; i < x[3]; i++)
    {
        a2 = ((float)(y[3] - y[2]) / (float)(x[3] - x[2]));
        b2 = y[2] - a2 * x[2];
        p[i] = (uchar)(a2 * i + b2);
    }
    std::cout << a2 << "\t" << b2 << std::endl;
    for (int i = x[3]; i < 256; i++)
    {
        p[i] = (uchar)255;
    }
    cv::LUT(src, table, dst);
}

void histDisplay(int histogram[256], const char* name, int mode)
{
    int hist[256];
    for (int i = 0; i < 256; i++)
    {
        hist[i] = histogram[i];
    }
    int hist_w = 256; int hist_h = 256;
    int bin_w = cvRound((double)hist_w / 256);

    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(255, 255, 255));

    for (int i = 0; i < 256; i++)
    {
        hist[i] = ((double)hist[i] / maxhist(histogram)) * histImage.rows;
    }

    for (int i = 0; i < 256; i++)
    {
        line(histImage, cv::Point(bin_w * i, hist_h), cv::Point(bin_w * i, hist_h - hist[i]), cv::Scalar(0, 0, 0), 1, 8, 0);
    }
    cv::namedWindow(name, cv::WINDOW_AUTOSIZE);

    if (mode != 0)
    {
        line(histImage, cv::Point(x[0], histImage.rows - y[0]), cv::Point(x[1], histImage.rows - y[1]), cv::Scalar(0, 0, 255), 1, 8, 0);
        line(histImage, cv::Point(x[1], histImage.rows - y[1]), cv::Point(x[2], histImage.rows - y[2]), cv::Scalar(255, 0, 0), 1, 8, 0);
        line(histImage, cv::Point(x[2], histImage.rows - y[2]), cv::Point(x[3], histImage.rows - y[3]), cv::Scalar(0, 255, 0), 1, 8, 0);
    }
    imshow(name, histImage);
    std::string text = (std::string)name + ".png";
    cv::imwrite(text, histImage);
}


int main()
{
    src = cv::imread("src.png");
    cv::Mat src_gray;
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
    imhist(src_gray, histogram);
    MappingFunction(src_gray, dst);
    histDisplay(histogram, "histogram_src", 1);
    cv::imshow("source", src);
    cv::imshow("destination", dst);
    imhist(dst, histogram);
    histDisplay(histogram, "histogram_dst", 0);
    cv::waitKey(0);
}