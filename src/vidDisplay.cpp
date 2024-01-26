// Author: Kevin Heleodoro
// Date: January 8, 2024
// Purpose: Display live video using OpenCV.

#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "faceDetect.h"
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
 * @brief Create a menu of commands.
 *
 * This function creates a menu of commands using OpenCV. The menu is displayed in a window named "Commands".
 * The menu is a list of commands with the currently selected command highlighted in green.
 *
 * @param commandMat The matrix to draw the menu on.
 * @param commands The list of commands to display.
 * @param selectedCommand The index of the currently selected command.
 */
void drawMenu(cv::Mat &commandMat, const std::vector<std::string> &commands, int selectedCommand)
{
    commandMat = cv::Mat::zeros(500, 300, CV_8UC3);
    for (int i = 0; i < commands.size(); ++i)
    {
        cv::Scalar textColor = (i == selectedCommand) ? cv::Scalar(0, 255, 0) : cv::Scalar(255, 255, 255);
        cv::putText(commandMat, commands[i], cv::Point(10, 30 * (i + 1)), cv::FONT_HERSHEY_SIMPLEX, 0.7, textColor, 2);
    }
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
    cv::Mat frame, commandMat;

    capdev = new cv::VideoCapture(0);
    if (!capdev->isOpened())
    {
        printf("Unable to open camera\n");
        return (-1);
    }

    cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH), (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
    int fps = capdev->get(cv::CAP_PROP_FPS);

    printf("Size: %d %d\n", refS.width, refS.height);
    printf("FPS: %d\n", fps);

    cv::namedWindow("Video");

    std::vector<std::string> commandText = {
        "Commands:",          "'q': quit",        "'s': screen shot", "'g': greyscale", "'h': alternate grayscale",
        "'p': sepia tone",    "'b': blur",        "'x': sobel x",     "'y': sobel y",   "'m': gradient magnitude",
        "'l': blur quantize", "'f': face detect", "'e': emboss",      "'n': negative",  "'+ or -': brightness"};
    int selectedCommand = -1;

    // Text properties
    int baseline = 0;
    int thickness = 2;
    int lineType = 8;
    double fontScale = 1.0;

    // Filter booleans
    bool gray = false;
    bool altGray = false;
    bool sepia = false;
    bool blur = false;
    bool sobelX = false;
    bool sobelY = false;
    bool gradientMagnitude = false;
    bool blurQuantized = false;
    bool faceDetect = false;
    bool emboss = false;
    bool negative = false;
    double brightness = 1.0;

    for (;;)
    {
        *capdev >> frame;
        if (frame.empty())
        {
            printf("frame is empty\n");
            break;
        }

        // Negative
        if (negative)
        {
            cv::Mat negativeFrame;
            int negativeColor = negativeFilter(frame, negativeFrame);
            if (negativeColor == 0)
            {
                frame = negativeFrame;
            }
        }

        // Emboss
        if (emboss)
        {
            cv::Mat sobelXFrame;
            cv::Mat sobelYFrame;
            int sobelXColor = sobelX3x3(frame, sobelXFrame);
            int sobelYColor = sobelY3x3(frame, sobelYFrame);
            if (sobelXColor == 0 && sobelYColor == 0)
            {
                cv::Mat embossFrame;
                int embossColor = embossEffect(sobelXFrame, sobelYFrame, embossFrame);
                if (embossColor == 0)
                {
                    frame = embossFrame;
                }
            }
        }

        // Detect faces
        if (faceDetect)
        {
            cv::Mat greyFrame;
            cv::cvtColor(frame, greyFrame, cv::COLOR_BGR2GRAY);
            std::vector<cv::Rect> faces;
            detectFaces(greyFrame, faces);
            drawBoxes(frame, faces);
        }

        // Blur quantize
        if (blurQuantized)
        {
            cv::Mat blurQuantizeFrame;
            int levels = 10;
            int blurQuantizeColor = blurQuantize(frame, blurQuantizeFrame, levels);
            if (blurQuantizeColor == 0)
            {
                frame = blurQuantizeFrame;
            }
        }

        // Gradient magnitude
        if (gradientMagnitude)
        {
            cv::Mat sobelXFrame;
            cv::Mat sobelYFrame;
            int sobelXColor = sobelX3x3(frame, sobelXFrame);
            int sobelYColor = sobelY3x3(frame, sobelYFrame);
            if (sobelXColor == 0 && sobelYColor == 0)
            {
                cv::Mat gradientMagnitudeFrame;
                int gradientMagnitudeColor = magnitude(sobelXFrame, sobelYFrame, gradientMagnitudeFrame);
                if (gradientMagnitudeColor == 0)
                {
                    frame = gradientMagnitudeFrame;
                }
            }
        }

        // Sobel X
        if (sobelX)
        {
            cv::Mat sobelXFrame;
            int sobelXColor = sobelX3x3(frame, sobelXFrame);
            if (sobelXColor == 0)
            {
                // frame = sobelXFrame;
                cv::convertScaleAbs(sobelXFrame, frame, 1, 0);
            }
        }

        // Sobel Y
        if (sobelY)
        {
            cv::Mat sobelYFrame;
            int sobelYColor = sobelY3x3(frame, sobelYFrame);
            if (sobelYColor == 0)
            {
                // frame = sobelYFrame;
                cv::convertScaleAbs(sobelYFrame, frame, 1, 0);
            }
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

        // Blur
        if (blur)
        {
            cv::Mat blurFrame;
            int blurColor = blur5x5_4(frame, blurFrame);
            if (blurColor == 0)
            {
                frame = blurFrame;
            }
        }

        // Display brightness
        std::stringstream brightnessStream;
        brightnessStream << "Brightness: " << std::fixed << std::setprecision(2) << brightness;
        std::string brightnessText = brightnessStream.str();
        cv::Size brightnessTextSize =
            cv::getTextSize(brightnessText, cv::FONT_HERSHEY_SIMPLEX, fontScale, thickness, &baseline);
        int startY = frame.rows - brightnessTextSize.height - 10;
        int centerX = frame.cols / 2;
        cv::putText(frame, brightnessText, cv::Point(centerX, startY), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                    cv::Scalar(255, 255, 255), thickness, lineType);

        // Adjust brightness
        cv::Mat brightenedFrame;
        int brightnessAdjusted = adjustBrightness(frame, brightenedFrame, brightness);
        if (brightnessAdjusted == 0)
        {
            frame = brightenedFrame;
        }

        drawMenu(commandMat, commandText, selectedCommand);
        cv::imshow("Commands", commandMat);
        // Display frame
        cv::imshow("Video", frame);
        char key = cv::waitKey(10);

        // Quit program
        if (key == 'q')
        {
            selectedCommand = 1;
            break;
        }

        // Screen capture
        if (key == 's')
        {
            selectedCommand = 2;
            // Get current timestamp and save screen capture
            std::string currentDateTimeStamp = getCurrentDateTimeStamp();
            cv::imwrite(currentDateTimeStamp + "_screen_capture.jpg", frame);
            cv::waitKey(500); // Wait for .5 seconds

            // Display screen captured text
            std::string screenCapturedText = "Screen captured.";
            cv::Size screenCapturedTextSize =
                cv::getTextSize(screenCapturedText, cv::FONT_HERSHEY_SIMPLEX, fontScale, thickness, &baseline);
            int textX = (frame.cols - screenCapturedTextSize.width) / 2;
            int textY = (frame.rows + screenCapturedTextSize.height) / 2;
            cv::putText(frame, screenCapturedText, cv::Point(textX, textY), cv::FONT_HERSHEY_SIMPLEX, 1.0,
                        cv::Scalar(255, 255, 255), thickness, lineType);

            cv::imshow("Video", frame);
            cv::waitKey(500); // Wait for .5 seconds
        }

        // Toggle grayscale
        if (key == 'g')
        {
            selectedCommand = 3;
            gray = !gray;
            altGray = false;
            sepia = false;
            blur = false;
            sobelX = false;
            sobelY = false;
            gradientMagnitude = false;
            blurQuantized = false;
            emboss = false;
            negative = false;
        }

        // Toggle alternate grayscale
        if (key == 'h')
        {
            selectedCommand = 4;
            altGray = !altGray;
            gray = false;
            sepia = false;
            blur = false;
            sobelX = false;
            sobelY = false;
            gradientMagnitude = false;
            blurQuantized = false;
            emboss = false;
            negative = false;
        }

        // Toggle sepia tone
        if (key == 'p')
        {
            selectedCommand = 5;
            sepia = !sepia;
            gray = false;
            altGray = false;
            blur = false;
            sobelX = false;
            sobelY = false;
            gradientMagnitude = false;
            blurQuantized = false;
            emboss = false;
            negative = false;
        }

        // Toggle blur
        if (key == 'b')
        {
            selectedCommand = 6;
            blur = !blur;
            gray = false;
            altGray = false;
            sepia = false;
            sobelX = false;
            sobelY = false;
            gradientMagnitude = false;
            blurQuantized = false;
            emboss = false;
            negative = false;
        }

        // Toggle sobel x
        if (key == 'x')
        {
            selectedCommand = 7;
            sobelX = !sobelX;
            gray = false;
            altGray = false;
            sepia = false;
            blur = false;
            sobelY = false;
            gradientMagnitude = false;
            blurQuantized = false;
            emboss = false;
            negative = false;
        }

        // Toggle sobel y
        if (key == 'y')
        {
            selectedCommand = 8;
            sobelY = !sobelY;
            gray = false;
            altGray = false;
            sepia = false;
            blur = false;
            sobelX = false;
            gradientMagnitude = false;
            blurQuantized = false;
            emboss = false;
            negative = false;
        }

        // Toggle gradient magnitude
        if (key == 'm')
        {
            selectedCommand = 9;
            gradientMagnitude = !gradientMagnitude;
            gray = false;
            altGray = false;
            sepia = false;
            blur = false;
            sobelX = false;
            sobelY = false;
            blurQuantized = false;
            emboss = false;
            negative = false;
        }

        // Toggle blur quantize
        if (key == 'l')
        {
            selectedCommand = 10;
            blurQuantized = !blurQuantized;
            gray = false;
            altGray = false;
            sepia = false;
            blur = false;
            sobelX = false;
            sobelY = false;
            gradientMagnitude = false;
            emboss = false;
            negative = false;
        }

        // Toggle face detection
        if (key == 'f')
        {
            selectedCommand = 11;
            faceDetect = !faceDetect;
            // gray = false;
            // altGray = false;
            // sepia = false;
            // blur = false;
            // sobelX = false;
            // sobelY = false;
            // gradientMagnitude = false;
            // blurQuantized = false;
        }

        // Toggle emboss
        if (key == 'e')
        {
            selectedCommand = 12;
            emboss = !emboss;
            gray = false;
            altGray = false;
            sepia = false;
            blur = false;
            sobelX = false;
            sobelY = false;
            gradientMagnitude = false;
            negative = false;
        }

        // Toggle negative
        if (key == 'n')
        {
            selectedCommand = 13;
            negative = !negative;
            gray = false;
            altGray = false;
            sepia = false;
            blur = false;
            sobelX = false;
            sobelY = false;
            gradientMagnitude = false;
            blurQuantized = false;
            emboss = false;
        }

        // Adjust brightness
        if (key == '+')
        {
            selectedCommand = 14;
            brightness += 0.1;
        }

        if (key == '-')
        {
            selectedCommand = 14;
            brightness -= 0.1;
        }
    }

    delete capdev;
    return (0);
}
