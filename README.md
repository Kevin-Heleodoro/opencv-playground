# Project One

## How to run

To compile a new executable enter the following in the terminal from within the `/src` dir:
`make <insert name from makefile>` i.e. `make photo` for the photo viewer.

Then navigate to the `/bin` directory and run the name of the executable i.e. `./photo`

## Steps taken

1. Created `data`, `bin`, `src`, and `include` subdirectories.
2. Created `makefile` and installed Makefile Tools extension. Make sure the makefile is within the `src` directory.
3. Created `src/imgDisplay.cpp` using the [OpenCV tutorial](https://docs.opencv.org/4.5.1/db/deb/tutorial_display_image.html) for reading/writing images.
4. Added the `starry_night.jpg` sample image.
5. Run command `make photo` from within the `./src` directory.
6. `cd` into bin and run `./photo`.
7. You can also run `./photo <path_to_image>` if you want to use your own image.

8. Used template from assignment to create `vidDisplay.cpp`.
9. Run comman `make vid` from within `./src` directory.
10. `cd` into `bin/` and run `./vid`.

## Errors I ran into

1. Makefile was not pointing to the correct location for opencv library.
2. The PKG_CONFIG_PATH was not set to the correct location.
