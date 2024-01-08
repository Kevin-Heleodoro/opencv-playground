// Author: Kevin Heleodoro
// Date: January 8, 2024
// Purpose: Display live video using OpenCV.

#include <cstdio>
#include <cstring>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

/**
 * @brief Uses OpenCV to display live video.
 *
 * This function uses OpenCV to display live video. The video is displayed in a
 * window named "Video". The program terminates when the user presses the 'q' key.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @return 0 if successful, -1 if error.
 *
 * @note A warning may appear when running this program. This is due to the
 * OpenCV library using a deprecated function for continuous video capture.
 */
int main(int argc, char *argv[])
{
    cv::VideoCapture *capdev;

    capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened())
    {
        printf("Unable to open camera\n");
        return (-1);
    }

    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH),
                  (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Expected size: %d %d\n", refS.width, refS.height);
    cv::namedWindow("Video", 1);
    cv::Mat frame;

    for (;;)
    {
        *capdev >> frame;
        if (frame.empty())
        {
            printf("frame is empty\n");
            break;
        }

        cv::imshow("Video", frame);

        char key = cv::waitKey(10);

        if (key == 'q')
        {
            break;
        }
        else if (key == 's')
        {
            cv::imwrite("screen_capture.jpg", frame);
        }
    }

    delete capdev;
    return (0);
}