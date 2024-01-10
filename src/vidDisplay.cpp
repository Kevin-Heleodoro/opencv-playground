// Author: Kevin Heleodoro
// Date: January 8, 2024
// Purpose: Display live video using OpenCV.

#include <cstdio>
#include <cstring>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "filter.h"

/**
 * @brief Get the current date and time as a formatted string.
 *
 * This function retrieves the current date and time from the system clock and formats it as a string
 * in the format "YYYY-MM-DD_HH-MM-SS". The formatted string is returned.
 *
 * @return std::string The current date and time as a formatted string.
 */
std::string getCurrentDateTimeStamp()
{
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&currentTime), "%Y-%m-%d_%H-%M-%S");

    return ss.str();
}

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
    cv::Mat frame;

    capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened())
    {
        printf("Unable to open camera\n");
        return (-1);
    }

    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH),
                  (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    int fps = capdev->get(cv::CAP_PROP_FPS);

    printf("Size: %d %d\n", refS.width, refS.height);
    printf("FPS: %d\n", fps);

    cv::namedWindow("Video");

    std::vector<std::string> commandText = {"Commands:", "'q': quit", "'s': screen shot", "'g': greyscale", "'h': alternate grayscale", "'p': Sepia tone"};

    // Text properties
    int baseline = 0;
    int thickness = 2;
    int lineType = 8;
    double fontScale = 1.0;

    bool gray = false;
    bool altGray = false;
    bool sepia = false;

    for (;;)
    {
        *capdev >> frame;
        if (frame.empty())
        {
            printf("frame is empty\n");
            break;
        }

        // Regular grayscale
        if (gray)
        {
            cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        }

        // Alternate grayscale
        if (altGray)
        {
            cv::Mat grayFrame;
            int grayColor = greyscale(frame, grayFrame);
            if (grayColor == 0)
            {
                frame = grayFrame;
            }
        }

        // Sepia tone
        if (sepia)
        {
            cv::Mat sepiaFrame;
            int sepiaColor = sepiaTone(frame, sepiaFrame);
            if (sepiaColor == 0)
            {
                frame = sepiaFrame;
            }
        }

        // Text properties for command list display
        int startY = frame.rows - (commandText.size() + 1) * 20;
        int textX = 10;

        // Display command text
        for (const std::string &line : commandText)
        {
            cv::Size lineSize = cv::getTextSize(line, cv::FONT_HERSHEY_SIMPLEX, fontScale, thickness, &baseline);
            cv::putText(frame, line, cv::Point(textX, startY),
                        cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), thickness, lineType);
            startY += lineSize.height + 5;
        }

        cv::imshow("Video", frame);
        char key = cv::waitKey(10);

        // Quit program
        if (key == 'q')
        {
            break;
        }

        // Screen capture
        if (key == 's')
        {
            // Display screen captured text
            std::string screenCapturedText = "Screen captured.";
            cv::Size screenCapturedTextSize = cv::getTextSize(screenCapturedText, cv::FONT_HERSHEY_SIMPLEX, fontScale, thickness, &baseline);
            int textX = (frame.cols - screenCapturedTextSize.width) / 2;
            int textY = (frame.rows + screenCapturedTextSize.height) / 2;
            cv::putText(frame, screenCapturedText, cv::Point(textX, textY),
                        cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), thickness, lineType);

            // Get current timestamp and save screen capture
            std::string currentDateTimeStamp = getCurrentDateTimeStamp();
            cv::imwrite(currentDateTimeStamp + "_screen_capture.jpg", frame);

            cv::imshow("Video", frame);
            cv::waitKey(500); // Wait for .5 seconds
        }

        // Toggle grayscale
        if (key == 'g')
        {
            gray = !gray;
            altGray = false;
            sepia = false;
        }

        // Toggle alternate grayscale
        if (key == 'h')
        {
            altGray = !altGray;
            gray = false;
            sepia = false;
        }

        // Toggle sepia tone
        if (key == 'p')
        {
            sepia = !sepia;
            gray = false;
            altGray = false;
        }
    }

    delete capdev;
    return (0);
}
