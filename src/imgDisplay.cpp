// Author: Kevin Heleodoro
// Date: January 8, 2024
// Purpose: Display and save an image using OpenCV.

#include <cstdio>
#include <cstring>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

/**
 * @brief Uses OpenCV to display and save an image.
 *
 * This function uses OpenCV to display and save an image. The image is
 * displayed in a window with the same name as the image file. The image is
 * then saved as "tested.jpg" when the user presses the 's' key. The program
 * terminates when the user presses 'q' or 's' key. If no image file is
 * specified, the program will use the default image "starry_night.jpg".
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @return 0 if successful, -1 if error.
 *
 * @note Currently the program will only read "starry_night.jpg" if it is
 * in the same directory as the executable.
 */
int main(int argc, char *argv[])
{
    cv::Mat src;
    char fileName[256];

    if (argc < 2)
    {
        std::string image_path = cv::samples::findFile("starry_night.jpg");
        src = cv::imread(image_path);
    }
    else
    {
        strcpy(fileName, argv[1]);
        src = cv::imread(fileName);
    }

    if (src.data == NULL)
    {
        printf("Unable to read image %s\n", fileName);
        exit(-1);
    }

    cv::namedWindow(fileName, cv::WINDOW_NORMAL);
    cv::imshow(fileName, src);

    char key = cv::waitKey(0);

    if (key == 'q')
    {
        cv::destroyAllWindows();
    }
    else if (key = 's')
    {
        cv::destroyAllWindows();
        cv::imwrite("tested.jpg", src);
    }

    printf("Terminating\n");

    return 0;
}