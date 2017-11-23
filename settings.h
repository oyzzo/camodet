#ifndef __SETTINGS_H__
#define __SETTINGS_H__
#include <string>
#include <iostream>

#define DEFAULT_SECONDS_AFTER 5 /** @see seconds_after */

using namespace std;

class Settings {
public:
	string input_source; /** The video we'll detect motion */
	string output_name; /** The name used for output recordings */
	unsigned int seconds_after; /** Seconds to record after motion stops */
	unsigned int counter_start; /** Output filename counter start */
	unsigned int debug; /** Debug window showing intermediate images */
	unsigned int area; /** The threshold area to detect movement */
	unsigned int noise; /** The noise reduction gaussian size */
	bool show_input; /** Shall we show the input video */
	int load_from_args(int argc, char* argv[]); /** Load settings from arguments */
	void print_usage(char* name); /** Load settings from arguments */
	Settings();
};

#endif //__SETTINGS_H__
