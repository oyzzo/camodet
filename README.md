# camodet
Lightweight Simple CAmera MOtion DEtection application. 

Tired of looking for an application that could do motion detection on my own surveillance cameras I decided to create it.
The existing solutions were too complicated to install and set up, too slow, abandoned or simply didn't work.

camodet is ultra lightweight, portable and fast, it can be run in a raspberry pi or similar.

## Compile
For Ubuntu based distros use the __prepare.sh__ script, it'll install all dependencies and will compile OpenCV3.
For other distros use the script as a guide for the steps.
The www.opencv.org documentation on how to compile it for your Operating System, can also be used.

1) mkdir build
2) cd build
3) cmake ..
4) make

done!
