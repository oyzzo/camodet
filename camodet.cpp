#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/imgproc/imgproc.hpp" 
#include <math.h>
#include <ctime>
#include <chrono>

#include "settings.h"

using namespace cv;
using namespace std;

/** @brief Helper logging function
 * This function logs events to stdout
 * @param event The string describing the event to log
 * @param quiet The quiet flag status to disable logging
 */
static void log_event(string event)
{
	auto now = chrono::system_clock::now();
	time_t time_now = chrono::system_clock::to_time_t(now);
	cout << ctime(&time_now) << " - " << event << endl;
}

/** @brief Sabe ROI mask template to image file
 * This function generates an image template for creating
 * The ROI mask.
 * @param cap VideoCapture where extract a frame for the image
 */
static void generate_mask_template(Mat frame)
{
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

    try {
        imwrite("mask.png", frame, compression_params);
    }
    catch (runtime_error& ex) {
        cerr << "Exception converting image to PNG format: " << ex.what() << endl;
        return;
    }

    log_event("Saved template for ROI Mask image.");
}

int main(int argc, char** argv) {
	Settings settings;

	if (settings.load_from_args(argc, argv) != 0) {
		exit(-1);
	}

	bool record = false;
	double t2 = 0;
	double t3 = 0;
	int motion = 0;
	VideoWriter writer;
	int counter = 0; /* Video name counter */

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


	Size size(
		(int)cap.get(CAP_PROP_FRAME_WIDTH),
		(int)cap.get(CAP_PROP_FRAME_HEIGHT)
	);
	Mat frame, pyr1, gray, gaus, delta, old, binary, dilated, mask, bin_mask;
	Mat element = getStructuringElement(MORPH_RECT,Size(7,7),Point(-1,-1));

	//Generate ROI mask template if needed
	if (settings.mask_template) {
		cap >> frame;
		generate_mask_template(frame);
	}
	
	//Load the ROI mask image and binaryze
	bool do_mask = false;
	if (settings.mask_file != "") {
		do_mask = true;
		mask = imread(settings.mask_file,IMREAD_GRAYSCALE);	
		pyrDown(mask,mask);
		threshold(mask,bin_mask,15,255,THRESH_BINARY);
	}

	unsigned int n = settings.noise;
	for (;;) {
		cap >> frame;

		if (frame.empty()) break; //ran out of film
		pyrDown(frame,pyr1);
		cvtColor(pyr1, gray, COLOR_BGR2GRAY);
		GaussianBlur(gray, gaus,Size(n,n), 5,5);
		if (!started) {
			gaus.copyTo(old);
			started = true;
		}	
		absdiff(old,gaus,delta);	
		gaus.copyTo(old);
		threshold(delta,binary,15,255,THRESH_BINARY);
		if (do_mask) {
			//Input, Input, output
			bitwise_and(bin_mask, binary, binary);		
		}
		dilate(binary,dilated, element, Point(-1,-1), 2);
		vector<vector < Point > > contours0;
		findContours(dilated, contours0, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		ostringstream nameStream;
		ostringstream timeStream;

		auto now = chrono::system_clock::now();
		time_t time_now = chrono::system_clock::to_time_t(now);
		timeStream << ctime(&time_now);
		string time = timeStream.str();
		time.erase(std::remove(time.begin(), time.end(), '\n'), time.end());

		//Camera Name
		if (settings.cam_name != "") {
			putText(frame, settings.cam_name, Point2f(15,25), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255,255), 2);
		}

		//Add timestamp to video
		if (settings.timestamp) {
			putText(frame, time, Point2f(15,size.height - 15), FONT_HERSHEY_PLAIN, 1,  Scalar(0,0,255,255), 2);
		}

		for (size_t k = 0; k < contours0.size(); k++) {
			if (contourArea(contours0[k]) < settings.area) {
				continue;
			}
			
			motion++; /* Some contour big enough to be movement */
			t2 = 0;

			if (!record) {
				record = true;
				counter++;
				nameStream << settings.output_name << counter << ".avi";
				writer.open(nameStream.str().c_str(), CV_FOURCC('X','V','I','D'),15,size);
				log_event("Motion Start");
				log_event("Recording file: " + nameStream.str());
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
			if ( tf >= settings.seconds_after) {
				record = false;
				t2 = 0;
				writer.release();
				log_event("Motion Stop");
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
		default: //Should never happen
			break;
		}
		if(waitKey(33) == 0) return 0;
	}	
	return 0;
}
