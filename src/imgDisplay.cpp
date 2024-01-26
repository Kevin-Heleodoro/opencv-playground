// Author: Kevin Heleodoro
// Date: January 8, 2024
// Purpose: Display and save an image using OpenCV.

#include <cstdio>
#include <cstring>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "filter.h"

/**
 * @brief Display a menu of filters to apply.
 *
 * This function displays a menu of filters to apply. The user is prompted to
 * enter a number corresponding to the filter they wish to apply. The function
 * returns the user's choice.
 *
 * @return The user's choice.
 */
int displayMenu()
{
    int choice;
    std::cout << "\nSelect a filter to apply:\n";
    std::cout << "1. Greyscale\n";
    std::cout << "2. Sepia\n";
    std::cout << "3. Blur\n";
    std::cout << "4. Gradient Magnitude\n";
    std::cout << "5. Negative\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
    std::cin >> choice;
    return choice;
}

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
 * @param argv Array of command line arguments. This should be a relative path to an image file.
 * @return 0 if successful, -1 if error.
 *
 * @note Currently the program will only read "starry_night.jpg" if it is
 * in the same directory as the executable.
 */
int main(int argc, char *argv[])
{
    cv::Mat src, filteredImage, frame;
    // cv::Mat frame;
    char fileName[256];

    if (argc < 2)
    {
        std::string fileName = cv::samples::findFile("starry_night.jpg");
        src = cv::imread(fileName);
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

    src.copyTo(frame);

    while (true)
    {
        int filterChoice = displayMenu();

        if (filterChoice == 0)
        {
            break;
        }

        // cv::Mat filteredImage;
        switch (filterChoice)
        {
        // case 0:
        //     break;
        case 1:
            if (greyscale(frame, filteredImage) == 0)
            {
                // frame = filteredImage;
                filteredImage.copyTo(frame);
            }
            break;
        case 2:
            if (sepiaTone(frame, filteredImage) == 0)
            {
                // frame = filteredImage;
                filteredImage.copyTo(frame);
            }
            break;
        case 3:
            if (blur5x5_5(frame, filteredImage) == 0)
            {
                // frame = filteredImage;
                filteredImage.copyTo(frame);
            }
            break;
        case 4:
            if (magnitude(frame, filteredImage) == 0)
            {
                // frame = filteredImage;
                filteredImage.copyTo(frame);
            }
            break;
        case 5:
            if (negativeFilter(frame, filteredImage) == 0)
            {
                // frame = filteredImage;
                filteredImage.copyTo(frame);
            }
            break;
        default:
            std::cout << "Invalid choice.\n";
            break;
        }

        cv::namedWindow(fileName, cv::WINDOW_NORMAL);
        cv::putText(frame, "Press 'q' to quit or 's' to save.", cv::Point(100, 400), cv::FONT_HERSHEY_SIMPLEX, 1.0,
                    cv::Scalar(255, 255, 255), 2);
        cv::imshow(fileName, frame);

        while (true)
        {
            char key = cv::waitKey(0);

            if (key == 'q')
            {
                printf("Quitting\n");
                return 0;
            }
            else if (key == 's')
            {
                printf("Saving image\n");
                cv::imwrite("tested.jpg", frame);
                break;
            }
            else
            {
                break;
            }
        }
    }
    cv::destroyAllWindows();
    printf("Terminating\n");

    return 0;
}