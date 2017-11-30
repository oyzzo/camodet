# camodet
__CAmera MOtion DETection application.__

Fast, simple and stable motion detection application that works with any local and remote cameras or video sources.

The existing solutions were too complicated to install and set up, too slow, abandoned or simply didn't work, so I decided
there was a need for a simple camera motion detection application as easy to use as any other command line application. 
__camodet__ is ultra lightweight, portable and fast, it can be run in a raspberry pi or similar.

## Features

* __Fast__! It can run on most machines! even old netbooks or raspberri pi!
* __No Installation__ or set up needed. Just run it!.
* __Flexible__. Customize just the parameter you need using the command line parameters.
* __Noise reduction__. No problem with noisy cameras!
* __Area threshold__. Reduce false positives and useless recordings of too small movements.
* __Time window__ to record after motion stopped.
* __Supports all the cameras__ or video sources thanks to OpenCV3, flash, MJPEG, flv, etc...
* Use Remote cameras or __network video streams__ as video source!
* __Video files__ can also be used as an input for motion detection, useful for fine tunning parameters on different cases.
* __Headless mode__, no window output for servers.
* __Visual debug__ mode shows how the video is processed in any step! It's great for finding the right values that work for your camera. 
* __Portable__. Compile it on any platform supported by OpenCV3.
* __Written in__ fast and stable __C++!__ Forget about software written in prototyping or web languages (nodejs, php, python etc..)
* __Add camera name__, if the camera does not support it, camodet labels the videos using a camera name label.
* __Add date and time__ to the video, very usefull to see in the recordings.
* __Any shape ROI mask__, use a black/white mask image to only detect motion in the desired areas, ignoring the rest.

## Compile

### Dependencies
For Ubuntu based distros use the __prepare.sh__ script, it'll install all dependencies and will compile OpenCV3.
```
./prepare.sh
``` 
For other distros use the script as a guide for the steps.
The www.opencv.org documentation on how to compile it for your Operating System, can also be used.

### Building
Download and extract the sources, then inside:
```
mkdir build
cd build
cmake ..
make
```

done!
