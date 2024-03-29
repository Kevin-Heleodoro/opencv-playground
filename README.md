# Project One

## Run these programs

1. Navigate to the directory containing the `.exe` files and run them as such:

-   `./photo.exe starry_night.jpg`
    > Relative path to any image file.
-   `./vid.exe`
    > Ensure the `haarcascade_frontalface_alt2.xml` file is in the same directory.

## How to compile

> Make sure you have opencv installed on your system.

1. To compile a new executable enter the following in the terminal from within the `/src` dir:
   `make <insert name from makefile>` i.e. `make photo` for the photo viewer.

2. Then navigate to the `/bin` directory and run the name of the executable i.e. `./photo.exe`

<!-- ## Steps taken

1. Created `data`, `bin`, `src`, and `include` subdirectories.
2. Created `makefile` and installed Makefile Tools extension. Make sure the makefile is within the `src` directory.
3. Created `src/imgDisplay.cpp` using the [OpenCV tutorial](https://docs.opencv.org/4.5.1/db/deb/tutorial_display_image.html) for reading/writing images.
4. Added the `starry_night.jpg` sample image.
5. Run command `make photo` from within the `./src` directory.
6. `cd` into bin and run `./photo.exe`.
7. You can also run `./photo <path_to_image>` if you want to use your own image.

8. Used template from assignment to create `vidDisplay.cpp`.
9. Run command `make vid` from within `./src` directory.
10. `cd` into `bin/` and run `./vid.exe`. -->

## Errors I ran into

1. Makefile was not pointing to the correct location for opencv library.
2. The PKG_CONFIG_PATH was not set to the correct location.
3. When creating the blur filter using the `src.ptr` extra care had to be taken to ensure the loop was not trying to access an out-of-bounds pixel.
4. When breaking up the blur filter into 1x5 arrays I was accidentally accessing the y values instead of x which caused the dst image to be rotated.
5. The xml file needs to be relative from where the program is being ran (i.e. the bin dir).

## Credits

This project utilizes several external resources for its development:

-   OpenCV Gaussian, Median, and Bilateral Filter Documentation: [OpenCV Docs](https://docs.opencv.org/3.4/dc/dd3/tutorial_gausian_median_blur_bilateral_filter.html)
-   Applying GaussianBlur to Individual Pixels: [Stack Overflow](https://stackoverflow.com/questions/16132242/apply-gaussianblur-to-individual-pixels)
-   Stencil (Numerical Analysis) on Wikipedia: [Wikipedia](https://en.wikipedia.org/wiki/Stencil_%28numerical_analysis%29)
-   Microchip Technology Documentation: [Microchip Documentation](https://onlinedocs.microchip.com/pr/GUID-37AD5EEE-6FAB-48FC-89F6-CAA649534B2A-en-US-1/index.html)
-   Image embossing on Wikipedia: [Wikipedia](https://en.wikipedia.org/wiki/Image_embossing)
-   Designing Image Filters using OpenCV: [Medium](https://medium.com/dataseries/designing-image-filters-using-opencv-like-abode-photoshop-express-part-2-4479f99fb35)
