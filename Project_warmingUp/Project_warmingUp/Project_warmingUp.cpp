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
void Coordinates()
{
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
    x[3] = index + 90;
    y[0] = 0;
    y[1] = 100;
    y[2] = 190;
    y[3] = 255;
}

void MappingFunction(cv::Mat& src, cv::Mat& dst)
{
    cv::Mat table(1, 256, CV_8U);
    uchar* p = table.ptr();
    for (int i = 0; i < x[0]; i++)
    {
        p[i] = (uchar)0;
    }
    float a0 = ((float)(y[1] - y[0]) / (float)(x[1] - x[0]));
    float b0 = y[0] - a0 * x[0];
    std::cout << a0 << "\t" << b0 << std::endl;
    for (int i = x[0]; i < x[1]; i++)
    {
        p[i] = (uchar)(a0 * i + b0);
    }
    float a1 = ((float)(y[2] - y[1]) / (float)(x[2] - x[1]));
    float b1 = y[1] - a1 * x[1];
    std::cout << a1 << "\t" << b1 << std::endl;
    for (int i = x[1]; i < x[2]; i++)
    {

        p[i] = (uchar)(a1 * i + b1);
    }
    float a2 = ((float)(y[3] - y[2]) / (float)(x[3] - x[2]));
    float b2 = y[2] - a2 * x[2];
    std::cout << a2 << "\t" << b2 << std::endl;
    for (int i = x[2]; i < x[3]; i++)
    {

        p[i] = (uchar)(a2 * i + b2);
    }
    for (int i = x[3]; i < 256; i++)
    {
        p[i] = (uchar)255;
    }
    cv::LUT(src, table, dst);
}

void histDisplay(int histogram[256], const char* name, bool mode_drawline)
{
    int hist[256];
    for (int i = 0; i < 256; i++)
    {
        hist[i] = histogram[i];
    }
    int hist_w = 256; int hist_h = 256;
    int bin_w = cvRound((double)hist_w / 256);

    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(255, 255, 255));
    //normalize
    for (int i = 0; i < 256; i++)
    {
        hist[i] = ((double)hist[i] / maxhist(histogram)) * histImage.rows;
    }
    //draw the histogram
    for (int i = 0; i < 256; i++)
    {
        line(histImage, cv::Point(bin_w * i, hist_h), cv::Point(bin_w * i, hist_h - hist[i]), cv::Scalar(0, 0, 0), 1, 8, 0);
    }
    cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
    //draw linear line
    if (mode_drawline == true)
    {
        line(histImage, cv::Point(x[0], histImage.rows - y[0]), cv::Point(x[1], histImage.rows - y[1]), cv::Scalar(0, 0, 255), 1, 8, 0);
        line(histImage, cv::Point(x[1], histImage.rows - y[1]), cv::Point(x[2], histImage.rows - y[2]), cv::Scalar(255, 0, 0), 1, 8, 0);
        line(histImage, cv::Point(x[2], histImage.rows - y[2]), cv::Point(x[3], histImage.rows - y[3]), cv::Scalar(0, 255, 0), 1, 8, 0);
    }
    imshow(name, histImage);
    std::string text = (std::string)name + ".png";
    cv::imwrite(text, histImage);
}

void recognize_obj(cv::Mat& src, const char* name)
{
    cv::Mat dst(src.rows, src.cols, CV_8UC3);
    for (int i = 0; i < src.rows; i++)
        for (int j = 0; j < src.cols; j++)
        {
            int tmp = src.at<uchar>(i, j);
            if (tmp < x[0])
                dst.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);
            if (tmp >= x[0] && tmp < x[1])
                dst.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 255);
            if (tmp >= x[1] && tmp < x[2])
                dst.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 0, 0);
            if (tmp >= x[2] && tmp < x[3])
                dst.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 255, 0);
            if (tmp >= x[3])
                dst.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 255, 255);
        }
    cv::imshow(name, dst);
    std::string text = (std::string)name + ".png";
    cv::imwrite(text, dst);
}
void histDisplayRecognize(int histogram[], const char* name)
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
        if (i < x[0])
        {
            line(histImage, cv::Point(bin_w * i, hist_h), cv::Point(bin_w * i, hist_h - hist[i]), cv::Scalar(0, 0, 0), 1, 8, 0);
        }
        if (i >= x[0] && i < x[1])
        {
            line(histImage, cv::Point(bin_w * i, hist_h), cv::Point(bin_w * i, hist_h - hist[i]), cv::Scalar(0, 0, 255), 1, 8, 0);
        }
        if (i >= x[1] && i < x[2])
        {
            line(histImage, cv::Point(bin_w * i, hist_h), cv::Point(bin_w * i, hist_h - hist[i]), cv::Scalar(255, 0, 0), 1, 8, 0);
        }
        if (i >= x[2] && i < x[3])
        {
            line(histImage, cv::Point(bin_w * i, hist_h), cv::Point(bin_w * i, hist_h - hist[i]), cv::Scalar(0, 255, 0), 1, 8, 0);
        }
        if (i >= x[3])
        {
            line(histImage, cv::Point(bin_w * i, hist_h), cv::Point(bin_w * i, hist_h - hist[i]), cv::Scalar(0, 255, 255), 1, 8, 0);
        }
    }
    cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
    imshow(name, histImage);
    std::string text = (std::string)name + ".png";
    cv::imwrite(text, histImage);
}
int main()
{
    //Load source image
    src = cv::imread("src.png");
    cv::imshow("source", src);
    //Convert to grayscale
    cv::Mat src_gray;
    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

    imhist(src_gray, histogram);
    Coordinates();
    histDisplay(histogram, "histogram_src", true);
    MappingFunction(src_gray, dst);
    cv::imshow("destination", dst);
    cv::imwrite("destination.png", dst);
    recognize_obj(src_gray, "recognize_image");
    histDisplayRecognize(histogram, "histogram_recognize");

    imhist(dst, histogram);
    histDisplay(histogram, "histogram_dst", false);
    cv::waitKey(0);
}