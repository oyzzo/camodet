#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/imgproc/imgproc.hpp" 
#include <math.h>
#include "settings.h"

#define TIME_REC_AFTER 10 /* Record seconds after stop motion */

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	Settings settings;

	if (settings.load_from_args(argc, argv) != 0) {
		exit(-1);
	}

	bool record = false;
	double t1 = 0;
	double t2 = 0;
	double t3 = 0;
	int motion = 0;
	VideoWriter writer;
	int counter = 0; /* Video name counter */
	char name[255];

	VideoCapture cap;
	bool started = false;
	
	//If no provided input source, try to open the 
	//first device's camera
	if (settings.input_source == "") { 
		cap.open(0);
	} else {
		cap.open(settings.input_source);
	}
	
	counter = settings.counter_start;

	//Prepare windows
	if (settings.debug > 0 && settings.debug < 5) {
		namedWindow("Debug", WINDOW_AUTOSIZE);
	}
	if (settings.show_input) {
		namedWindow("Input", WINDOW_AUTOSIZE);
	}


	double fps = cap.get(CAP_PROP_FPS);
	Size size(
		(int)cap.get(CAP_PROP_FRAME_WIDTH),
		(int)cap.get(CAP_PROP_FRAME_HEIGHT)
	);
	Mat frame, pyr1, gray, gaus, delta, old, binary, dilated;
	Mat element = getStructuringElement(MORPH_RECT,Size(7,7),Point(-1,-1));
	for (;;) {
		cap >> frame;

		if (frame.empty()) break; //ran out of film
		pyrDown(frame,pyr1);
		cvtColor(pyr1, gray, COLOR_BGR2GRAY);
		GaussianBlur(gray, gaus,Size(21,21), 5,5);
		if (!started) {
			gaus.copyTo(old);
			started = true;
		}	
		absdiff(old,gaus,delta);	
		gaus.copyTo(old);
		threshold(delta,binary,15,255,THRESH_BINARY);
		dilate(binary,dilated, element, Point(-1,-1), 2);
		vector<vector < Point > > contours0;
		findContours(dilated, contours0, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		ostringstream nameStream;

		for (size_t k = 0; k < contours0.size(); k++) {
			if (contourArea(contours0[k]) < settings.area) {
				continue;
			}
			
			motion++; /* Some contour big enough to be movement */
			t2 = 0;

			if (!record) {
				record = true;
				t1 = getTickCount();
				counter++;
				nameStream << settings.output_name << counter << ".avi";
				writer.open(nameStream.str().c_str(), CV_FOURCC('X','V','I','D'),15,size);
			}
	
		}
		if (motion == 0 && record && t2 == 0) {
			/* If no motion in this frame, stop recording */
			t2 = getTickCount();	
		}
		motion = 0; /* Reset motion detected for the new frame */

		//Show input source if desired
		if (settings.show_input) {
			namedWindow("Input", WINDOW_AUTOSIZE);
			imshow("Input", frame);
		}

		/* Now compute time to stop recording */
		if (record && t2 != 0) {
			t3 = getTickCount();
			double tf = (t3 - t2)/(double)getTickFrequency();
			printf("TIME %f\n", tf);
			if ( tf >= settings.seconds_after) {
				record = false;
				t1 = 0;
				t2 = 0;
				t3 = 0;	
				writer.release();
			}
		}
		if (record) {
			writer << frame;
		}

		//Debug window
		switch (settings.debug) {
		case 1:
			imshow("Debug", gaus); //Noise reduction
			break;
		case 2:
			imshow("Debug", delta); //Difference image
			break;
		case 3:
			imshow("Debug", binary); //Binary threshold image
			break;
		case 4:
			imshow("Debug", dilated ); //Dilated
			break;
		}
		if(waitKey(33) == 0) return 0;
	}	
	return 0;
}
