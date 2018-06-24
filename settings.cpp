#include "settings.h"
#include <unistd.h>

using namespace std;

Settings::Settings()
{
	input_source ="";
	output_name ="recording";
	cam_name ="";
	show_input = false;
	seconds_after = DEFAULT_SECONDS_AFTER;
	counter_start = 0;
	debug = 0;
	area = 600;
	noise = 21;
	fps = 5;
	frames_trigger = 2;
	timestamp = false;
	mask_template = false;
	mask_file = "";
	command = "";
	draw_contours = false;
	max_width = 640;
}

/** @brief Load settings from command line arguments
 * This function parses the command line arguments into
 * the settings class.
 * Also a valid setting configuration is validated.
 */
int Settings::load_from_args(int argc, char* argv[])
{
	int opt;
	bool will_print_usage = false;

	while ((opt = getopt(argc, argv, "i:o:sa:c:d:ht:n:l:Dgm:f:k:x:CM:")) != -1) {
		switch (opt) {
		case 'i':
			input_source = optarg;
			break;
		case 'o':
			output_name = optarg;
			break;
		case 's':
			show_input = true;
			break;
		case 'D':
			timestamp = true;
			break;
		case 'a':
			seconds_after = atoi(optarg);
			break;
		case 'c':
			counter_start = atoi(optarg);
			break;
		case 'd':
			debug = atoi(optarg);
			break;
		case 'h':
			will_print_usage = true;
			break;
		case 't':
			area = atoi(optarg);
			break;
		case 'n':
			noise = atoi(optarg);
			break;
		case 'l':
			cam_name = optarg;
			break;
		case 'g':
			mask_template = true;
			break;
		case 'm':
			mask_file = optarg;
			break;
		case 'f':
			fps = atoi(optarg);
			break;
		case 'k':
			frames_trigger = atoi(optarg);
			break;
		case 'x':
			command = optarg;
			break;
		case 'C':
			draw_contours = true;
			break;
		case 'M':
			max_width = atoi(optarg);
			break;
		default: //Should never happen
			will_print_usage = true;
			break;
		}
	}

	//Validate the configuration
	if (debug > 4) {
		cerr << "Error: debug step doesn't correspond to a valid number." << endl;
		will_print_usage = true;
	}

	//odd gaussian matrix size
	if (noise % 2 == 0) {
		noise++;
	}

	if (will_print_usage) {
		print_usage(argv[0]);
		return -1;
	}

	return 0;
}

/** @brief Prints the command line usage
 * This function prints the command line usage of the application.
 * @param name The name of the application, usually argv[0].
 */
void Settings::print_usage(char* name)
{
	cerr << endl << "Usage: " << name << " [options]" << endl
	<< "Options:" << endl
	<< "    -h              Print this help message." << endl
	<< "    -i input_video: The source for motion detection." << endl
	<< "    -o output_name: The name for the output recordings." << endl
	<< "                    A number and extension will be automatically added after it:" << endl
	<< "                      e.g. output_name23.avi" << endl
	<< "    -s:             Open window showing the input video." << endl
	<< "    -D:             Date and time labelled to video." << endl
	<< "    -a seconds:     Seconds to record after the motion has stopped." << endl
	<< "    -c number:      Counter number to skip using in the output name (Default 0)." << endl
	<< "    -g              Generate template image for ROI mask." << endl
	<< "    -l cam_name:    Label camera name on video." << endl
	<< "    -m mask_image:  Mask image to use for ROI motion detection. Black areas are ignored, White areas checked." << endl
	<< "    -t number:      Threshold area (sqare pixels) to trigger detection." << endl 
	<< "                    Movements below this area are ignored (Default 600)." << endl
	<< "    -n number:      Noise reduction level (Default 21)." << endl
	<< "    -f number:      FPS to process from input source (Default 5)" << endl
	<< "    -k number:      Number of consecutive frames with motion to trigger motion event (Default 2)" << endl
	<< "    -x command:     Command to be executed when motion is detected" << endl
	<< "    -M number:      Max width of frame to be processed for motion detection, if input is larger it will be downsized to this value. (Default 640)" << endl
	<< "    -d number:      Show intermediate images in a debug window. Number can be:" << endl
	<< "                    1: noise reduction | 2: frames difference | 3: threshold | 4:dilated(final)" << endl;
}
