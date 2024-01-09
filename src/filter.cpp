#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "filter.h"

/**
 * @brief Convert a color image to greyscale.
 *
 * This function converts a color image to greyscale. It does so by subtracting
 * the red value of each pixel from 255 and setting the red, green, and blue
 * values of each pixel to the resulting value.
 *
 * @param src The source image.
 * @param dst The destination image.
 * @return 0 if successful, -1 if error.
 */
int greyscale(cv::Mat &src, cv::Mat &dst)
{
    if (src.empty())
    {
        printf("Frame is empty\n");
        return -1;
    }

    dst = src.clone();

    for (int y = 0; y < dst.rows; ++y)
    {
        for (int x = 0; x < dst.cols; ++x)
        {
            cv::Vec3b &pixel = dst.at<cv::Vec3b>(y, x);

            uchar invertedRed = 255 - pixel[2];
            pixel[0] = invertedRed;
            pixel[1] = invertedRed;
            pixel[2] = invertedRed;
        }
    }

    return 0;
}
