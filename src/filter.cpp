// Author: Kevin Heleodoro
// Date: January 9, 2024
// Purpose: Display live video using OpenCV.

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

/**
 * @brief Convert a color image to sepia tone.
 *
 * This function converts a color image to sepia tone. It does so by applying
 * sepia coefficients to each pixel.
 *
 * @param src The source image.
 * @param dst The destination image.
 * @return 0 if successful, -1 if error.
 */
int sepiaTone(cv::Mat &src, cv::Mat &dst)
{
    if (src.empty())
    {
        printf("Frame is empty\n");
        return -1;
    }

    dst = src.clone();

    for (int y = 0; y < dst.rows; y++)
    {
        for (int x = 0; x < dst.cols; x++)
        {
            cv::Vec3b pixel = src.at<cv::Vec3b>(y, x);

            uchar blue = pixel[0];
            uchar green = pixel[1];
            uchar red = pixel[2];

            // 0.189, 0.168, 0.131  // Blue coefficients
            // 0.769, 0.686, 0.534, // Green coefficients
            // 0.393, 0.349, 0.272, // Red coefficients

            // Apply sepia coefficients
            uchar newRed = std::min(255.0, 0.393 * red + 0.769 * green + 0.189 * blue);
            uchar newGreen = std::min(255.0, 0.349 * red + 0.686 * green + 0.168 * blue);
            uchar newBlue = std::min(255.0, 0.272 * red + 0.534 * green + 0.131 * blue);

            dst.at<cv::Vec3b>(y, x) = cv::Vec3b(newBlue, newGreen, newRed);
        }
    }

    return 0;
}