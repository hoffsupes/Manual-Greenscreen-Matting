# Manual-Greenscreen-Matting
Greenscreen Matting done on still images and greenscreen videos.
Results and code included within.
dassg@rpi.edu

Matting done according to the greenscreen matting equation:

alpha_matte = 1 - a1*( Green_layer - a2*Blue_layer );

and 

alpha_matte(foreground_region) = power(alpha_matte(foreground_region),gamma);

a1: tunable parameter
a2: tunable parameter
gamma: tunable parameter, represents power
Green_layer : Green layer of source image
Blue_layer : Blue layer of source image
 
If you're on linux, you can use some of the software I wrote (if you have OpenCV 3.0 Installed):

MATTE AND COMPOSITE GENERATOR

./op1 SOURCE_IMAGE TARGET_IMAGE [can be compiled using 1_track.cpp]

SOURCE_IMAGE: The original image for which you need the alpha matte (must be a green screen image)
TARGET_IMAGE: The Image on which you want to composite the foreground contained in the source image, you'll be shown the final composite as you change the parameter values

[https://i.imgur.com/0Ptmhz3.png] should give you an idea about the functioning of the program.

MANUAL COMPOSITE GENERATOR

./get_c MATTE_LOCATION TARGET_LOCATION SOURCE_LOCATION FINAL_COMPOSITE_LOCATION

Other files were used to matte video frames, they're there for reference, if you wish to use them, you must understand them yourself.

