#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
cv::Mat src;
void imhist(cv::Mat image, int histogram[])
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

void histDisplay(int histogram[], const char* name)
{
    int hist[256];
    for (int i = 0; i < 256; i++)
    {
        hist[i] = histogram[i];
    }
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound((double)hist_w / 256);

    cv::Mat histImage(hist_h, hist_w, CV_8UC1, cv::Scalar(255, 255, 255));

    int max = hist[0];
    for (int i = 1; i < 256; i++)
    {
        if (max < hist[i])
        {
            max = hist[i];
        }
    }
    for (int i = 0; i < 256; i++)
    {
        hist[i] = ((double)hist[i] / max) * histImage.rows;
    }

    for (int i = 0; i < 256; i++)
    {
        line(histImage, cv::Point(bin_w * i, hist_h), cv::Point(bin_w * i, hist_h - hist[i]), cv::Scalar(0, 0, 0), 1, 8, 0);
    }
    cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
    imshow(name, histImage);
}
int main()
{
    src = cv::imread("src.png");
    cv::Mat src_gray;
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
    int histogram[256];
    imhist(src_gray, histogram);
    histDisplay(histogram, "hist");
    cv::imshow("source", src);
    cv::waitKey(0);
}