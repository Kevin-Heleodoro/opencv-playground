// Author: Kevin Heleodoro
// Date: January 9, 2024
// Purpose: Display live video using OpenCV.

#include "filter.h"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

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

            // 0.189, 0.168, 0.131  Blue coefficients
            // 0.769, 0.686, 0.534, Green coefficients
            // 0.393, 0.349, 0.272, Red coefficients

            // Apply sepia coefficients
            uchar newRed = std::min(255.0, 0.393 * red + 0.769 * green + 0.189 * blue);
            uchar newGreen = std::min(255.0, 0.349 * red + 0.686 * green + 0.168 * blue);
            uchar newBlue = std::min(255.0, 0.272 * red + 0.534 * green + 0.131 * blue);

            dst.at<cv::Vec3b>(y, x) = cv::Vec3b(newBlue, newGreen, newRed);
        }
    }

    return 0;
}

/**
 * @brief Blur a color image using a 5x5 Gaussian kernel.
 *
 * This function blurs a color image using a 5x5 Gaussian kernel. It does so by
 * applying the kernel to each pixel within a nested loop.
 *
 * @param src The source image.
 * @param dst The destination image.
 * @return 0 if successful, -1 if error.
 */
int blur5x5_1(cv::Mat &src, cv::Mat &dst)
{
    if (src.empty())
    {
        printf("Frame is empty\n");
        return -1;
    }

    dst = src.clone();

    int kernel[5][5] = {// Gaussian kernel 5x5
                        {1, 2, 4, 2, 1},
                        {2, 4, 8, 4, 2},
                        {4, 8, 16, 8, 4},
                        {2, 4, 8, 4, 2},
                        {1, 2, 4, 2, 1}};
    int kernelSum = 100; // sum of all kernel values

    for (int y = 2; y < src.rows - 2; y++) // iterate through image rows
    {
        for (int x = 2; x < src.cols - 2; x++) // iterate through image columns
        {
            int blue = 0, green = 0, red = 0; // initialize color channel values

            for (int ky = -2; ky <= 2; ky++) // iterate through kernel rows
            {
                for (int kx = -2; kx <= 2; kx++) // iterate through kernel columns
                {
                    cv::Vec3b pixel = src.at<cv::Vec3b>(y + ky, x + kx); // get pixel at kernel index
                    int weight = kernel[ky + 2][kx + 2];                 // get kernel weight

                    // Apply kernel to pixel
                    blue += pixel[0] * weight;
                    green += pixel[1] * weight;
                    red += pixel[2] * weight;
                }
            }

            // Set pixel to average of kernel
            dst.at<cv::Vec3b>(y, x) = cv::Vec3b(blue / kernelSum, green / kernelSum, red / kernelSum);
        }
    }

    return 0;
}

/**
 * @brief Blur a color image using a 1x5 Gaussian kernel.
 *
 * This function blurs a color image using a 1x5 Gaussian kernel. It does so by
 * applying separable 1x5 filters to each pixel in two passes (horizontal and veritcal).
 *
 * @param src The source image.
 * @param dst The destination image.
 * @return 0 if successful, -1 if error.
 */
int blur5x5_2(cv::Mat &src, cv::Mat &dst)
{
    if (src.empty())
    {
        printf("Frame is empty\n");
        return -1;
    }

    dst = src.clone();

    // 1x5 kernel
    int kernel[5] = {1, 2, 4, 2, 1};
    int kernelSum = 10;

    cv::Mat temp = cv::Mat::zeros(src.size(), src.type()); // Temporary image used for horizontal pass

    // Horizontal pass
    for (int y = 0; y < src.rows; ++y) // iterate through image rows
    {
        for (int x = 2; x < src.cols - 2; ++x) // iterate through image columns
        {
            int blue = 0, green = 0, red = 0; // initialize color channel values

            for (int k = -2; k <= 2; ++k) // iterate through kernel
            {
                // printf("y: %d, x: %d, k: %d\n", y, x, k);
                cv::Vec3b pixel = src.ptr<cv::Vec3b>(y)[x + k]; // get pixel at kernel index
                int weight = kernel[k + 2];                     // get kernel weight

                // Apply kernel to pixel
                blue += pixel[0] * weight;
                green += pixel[1] * weight;
                red += pixel[2] * weight;
            }

            // Set pixel to average of kernel
            temp.ptr<cv::Vec3b>(y)[x] = cv::Vec3b(blue / kernelSum, green / kernelSum, red / kernelSum);
        }
    }

    // Vertical pass
    for (int y = 2; y < src.rows - 2; ++y) // iterate through image rows
    {
        for (int x = 0; x < src.cols; ++x) // iterate through image columns
        {
            int blue = 0, green = 0, red = 0; // initialize color channel values

            for (int k = -2; k <= 2; ++k) // iterate through kernel
            {
                cv::Vec3b pixel = temp.ptr<cv::Vec3b>(y + k)[x]; // get pixel at kernel index
                int weight = kernel[k + 2];                      // get kernel weight

                // Apply kernel to pixel
                blue += pixel[0] * weight;
                green += pixel[1] * weight;
                red += pixel[2] * weight;
            }

            // Set pixel to average of kernel
            dst.ptr<cv::Vec3b>(y)[x] = cv::Vec3b(blue / kernelSum, green / kernelSum, red / kernelSum);
        }
    }

    return 0;
}

/**
 * @brief Blur a color image using a 5x5 Gaussian kernel.
 *
 * This function blurs a color image using a 5x5 Gaussian kernel. It does so by
 * applying the kernel to each pixel within a nested loop. This version of the function
 * uses the .at method to access the pixels. It adds the values of each pixel in the kernel
 * without looping through it.
 *
 * @param src The source image.
 * @param dst The destination image.
 * @return 0 if successful, -1 if error.
 */
int blur5x5_3(cv::Mat &src, cv::Mat &dst)
{
    if (src.empty())
    {
        printf("Frame is empty\n");
        return -1;
    }

    // int kernel[5][5] = {// Gaussian kernel 5x5
    //                     {1, 2, 4, 2, 1},
    //                     {2, 4, 8, 4, 2},
    //                     {4, 8, 16, 8, 4},
    //                     {2, 4, 8, 4, 2},
    //                     {1, 2, 4, 2, 1}};
    // int kernelSum = 100; // sum of all kernel values

    dst = src.clone();

    for (int y = 2; y < src.rows - 2; y++)
    {
        // cv::Vec3b *ptr = src.ptr<cv::Vec3b>(y);
        for (int x = 2; x < src.cols - 2; x++)
        {
            for (int k = 0; k < src.channels(); k++)
            {
                int sum = src.at<cv::Vec3b>(y - 2, x - 2)[k] + 2 * src.at<cv::Vec3b>(y - 2, x - 1)[k] +
                          4 * src.at<cv::Vec3b>(y - 2, x)[k] + 2 * src.at<cv::Vec3b>(y - 2, x + 1)[k] +
                          src.at<cv::Vec3b>(y - 2, x + 2)[k] + 2 * src.at<cv::Vec3b>(y - 1, x - 2)[k] +
                          4 * src.at<cv::Vec3b>(y - 1, x - 1)[k] + 8 * src.at<cv::Vec3b>(y - 1, x)[k] +
                          4 * src.at<cv::Vec3b>(y - 1, x + 1)[k] + 2 * src.at<cv::Vec3b>(y - 1, x + 2)[k] +
                          4 * src.at<cv::Vec3b>(y, x - 2)[k] + 8 * src.at<cv::Vec3b>(y, x - 1)[k] +
                          16 * src.at<cv::Vec3b>(y, x)[k] + 8 * src.at<cv::Vec3b>(y, x + 1)[k] +
                          4 * src.at<cv::Vec3b>(y, x + 2)[k] + 2 * src.at<cv::Vec3b>(y + 1, x - 2)[k] +
                          4 * src.at<cv::Vec3b>(y + 1, x - 1)[k] + 8 * src.at<cv::Vec3b>(y + 1, x)[k] +
                          4 * src.at<cv::Vec3b>(y + 1, x + 1)[k] + 2 * src.at<cv::Vec3b>(y + 1, x + 2)[k] +
                          src.at<cv::Vec3b>(y + 2, x - 2)[k] + 2 * src.at<cv::Vec3b>(y + 2, x - 1)[k] +
                          4 * src.at<cv::Vec3b>(y + 2, x)[k] + 2 * src.at<cv::Vec3b>(y + 2, x + 1)[k];

                sum /= 100;

                dst.at<cv::Vec3b>(y, x)[k] = sum;
            }
        }
    }

    return 0;
}

/**
 * @brief Blur a color image using a 5x5 Gaussian kernel.
 *
 * This function blurs a color image using a 5x5 Gaussian kernel. It does so by
 * applying the kernel to each pixel within a nested loop. This version of the function uses the .ptr method to access
 * pixels. It also does not loop through the kernel, but instead calculates the sum of each row and column of the kernel
 * separately.
 *
 * @param src The source image.
 * @param dst The destination image.
 * @return 0 if successful, -1 if error.
 **/
int blur5x5_4(cv::Mat &src, cv::Mat &dst)
{
    src.copyTo(dst);

    // int kernel[5][5] = {// Gaussian kernel 5x5
    //                     {1, 2, 4, 2, 1},
    //                     {2, 4, 8, 4, 2},
    //                     {4, 8, 16, 8, 4},
    //                     {2, 4, 8, 4, 2},
    //                     {1, 2, 4, 2, 1}};
    // int kernelSum = 100; // sum of all kernel values

    for (int y = 2; y < src.rows - 2; y++)
    {
        // This .ptr method is faster than .at method because it stores the row as reference.
        cv::Vec3b *ptrTwoUp = src.ptr<cv::Vec3b>(y - 2);
        cv::Vec3b *ptrOneUp = src.ptr<cv::Vec3b>(y - 1);
        cv::Vec3b *ptr = src.ptr<cv::Vec3b>(y);
        cv::Vec3b *ptrOneDown = src.ptr<cv::Vec3b>(y + 1);
        cv::Vec3b *ptrTwoDown = src.ptr<cv::Vec3b>(y + 2);

        cv::Vec3b *ptrDst = dst.ptr<cv::Vec3b>(y);

        for (int x = 2; x < src.cols - 2; x++)
        {

            for (int k = 0; k < src.channels(); k++)
            {
                // ptrmd[x][k] accesses the pixel at (x, y) and color channel k
                int sumOne = ptrTwoUp[x - 2][k] + 2 * ptrTwoUp[x - 1][k] + 4 * ptrTwoUp[x][k] + 2 * ptrTwoUp[x + 1][k] +
                             ptrTwoUp[x + 2][k];
                int sumTwo = 2 * ptrOneUp[x - 2][k] + 4 * ptrOneUp[x - 1][k] + 8 * ptrOneUp[x][k] +
                             4 * ptrOneUp[x + 1][k] + 2 * ptrOneUp[x + 2][k];
                int sumThree =
                    4 * ptr[x - 2][k] + 8 * ptr[x - 1][k] + 16 * ptr[x][k] + 8 * ptr[x + 1][k] + 4 * ptr[x + 2][k];
                int sumFour = 2 * ptrOneDown[x - 2][k] + 4 * ptrOneDown[x - 1][k] + 8 * ptrOneDown[x][k] +
                              4 * ptrOneDown[x + 1][k] + 2 * ptrOneDown[x + 2][k];
                int sumFive = ptrTwoDown[x - 2][k] + 2 * ptrTwoDown[x - 1][k] + 4 * ptrTwoDown[x][k] +
                              2 * ptrTwoDown[x + 1][k] + ptrTwoDown[x + 2][k];

                int sum = sumOne + sumTwo + sumThree + sumFour + sumFive;

                sum /= 100;

                ptrDst[x][k] = sum; // assign result
            }
        }
    }

    return 0;
}

int blur5x5_5(cv::Mat &src, cv::Mat &dst)
{
    src.copyTo(dst);

    int kernel[5] = {1, 2, 4, 2, 1};
    int kernelSum = 10;
    cv::Mat temp = cv::Mat::zeros(src.size(), src.type());

    // Horizontal pass
    for (int y = 0; y < src.rows; y++)
    {
        cv::Vec3b *ptr = src.ptr<cv::Vec3b>(y);
        for (int x = 2; x < src.cols - 2; x++)
        {
            for (int k = 0; k < src.channels(); k++)
            {
                int sum = ptr[x - 2][k] + 2 * ptr[x - 1][k] + 4 * ptr[x][k] + 2 * ptr[x + 1][k] + ptr[x + 2][k];

                sum /= 10;

                dst.ptr<cv::Vec3b>(y)[x][k] = sum;
            }
        }
    }

    // Vertical pass
    for (int y = 2; y < src.rows - 2; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {
            cv::Vec3b *ptr = dst.ptr<cv::Vec3b>(y);
            for (int k = 0; k < src.channels(); k++)
            {
                int sum = ptr[x - 2][k] + 2 * ptr[x - 1][k] + 4 * ptr[x][k] + 2 * ptr[x + 1][k] + ptr[x + 2][k];

                sum /= 10;

                dst.ptr<cv::Vec3b>(y)[x][k] = sum;
            }
        }
    }

    return 0;
}

/**
 * @brief Blur a color image using a 3x3 Gaussian kernel.
 *
 * This function blurs a color image using a 3x3 Gaussian kernel.
 *
 * @param src The source image.
 * @param dst The destination image.
 * @return 0 if successful, -1 if error.
 */
int gauss3x3at(cv::Mat &src, cv::Mat &dst) // pass images by reference
{
    // allocate space for destination image
    src.copyTo(dst); // Let's us ignore the outer boundaries

    // alternative
    // dst.create(src.size(), src.type()); // makes duplicate image, uninitialized data

    // only calculating values for pixels where filter fits in the image.
    for (int y = 1; y < src.rows - 1; y++) // rows
    {
        for (int x = 1; x < src.cols - 1; x++) // cols
        {
            for (int k = 0; k < src.channels(); k++) // color channels
            {
                int sum = src.at<cv::Vec3b>(y - 1, x - 1)[k] + 2 * src.at<cv::Vec3b>(y - 1, x)[k] +
                          src.at<cv::Vec3b>(y - 1, x + 1)[k] + 2 * src.at<cv::Vec3b>(y, x - 1)[k] +
                          4 * src.at<cv::Vec3b>(y, x)[k] + 2 * src.at<cv::Vec3b>(y, x + 1)[k] +
                          src.at<cv::Vec3b>(y + 1, x - 1)[k] + 2 * src.at<cv::Vec3b>(y + 1, x)[k] +
                          src.at<cv::Vec3b>(y + 1, x + 1)[k];

                // normalize the value back to a range of [0 ,255]
                sum /= 16;

                dst.at<cv::Vec3b>(y, x)[k] = sum;
            }
        }
    }

    return 0;
}