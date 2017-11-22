#include "settings.h"
#include <unistd.h>

using namespace std;

Settings::Settings()
{
	input_source ="";
	output_name ="recording";
	show_input = false;
	seconds_after = DEFAULT_SECONDS_AFTER;
	counter_start = 0;
	debug = 0;
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

	while ((opt = getopt(argc, argv, "i:o:sa:c:d:")) != -1) {
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
		case 'a':
			seconds_after = atoi(optarg);
			break;
		case 'c':
			counter_start = atoi(optarg);
			break;
		case 'd':
			debug = atoi(optarg);
			break;
		default: /* '?' */
			will_print_usage = true;
			break;
		}
	}

	//Validate the configuration
	if (output_name == "") {
		cerr << "Error: output name for recordings needed." << endl;
		will_print_usage = true;
	} else if (debug < 0 || debug > 4) {
		cerr << "Error: debug step doesn't correspond to a valid number." << endl;
		will_print_usage = true;
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
	cerr << endl << "Usage: " << name << " [-i input_video]"
	<< " [-o output_name] [-s] [-a seconds] [-c num] [-d num]" << endl
	<< "    -i input_video: The source for motion detection." << endl
	<< "    -o output_name: The name for the output recordings." << endl
	<< "                    A number and extension will be automatically added after it:" << endl
	<< "                    	e.g. output_name23.avi" << endl
	<< "    -s:             Open window showing the input video." << endl
	<< "    -a seconds:     Seconds to record after the motion has stopped." << endl
	<< "    -c number:      Counter number to start using in the output name." << endl
	<< "    -d number:      Show intermediate images in a debug window. Number can be:" << endl
	<< "                    1: noise reduction | 2: frames difference | 3: threshold | 4:dilated(final)" << endl;
}
