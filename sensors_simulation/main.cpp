#include "simulator.h"
#include "utils.h"

int main() {
	try {

		std::cout	<< "enter input data:" << std::endl
					<< "[number of sensors] [number of connections] [seconds]" << std::endl;
		unsigned int sensor_num_ = 0;
		unsigned int edges_per_sensor = 0;
		unsigned int seconds = 0;
		std::cin >> sensor_num_ >> edges_per_sensor >> seconds;
		std::cout << std::endl;
		if(!utils::check_input_data(sensor_num_, edges_per_sensor, seconds)) {
			utils::press_any_key();
			return EXIT_FAILURE;
		}
	
		simulator simulator_(sensor_num_, edges_per_sensor, seconds);
		simulator_.run();
	}
	catch(std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	catch(...) {
		std::cerr << "Unknown exception" << std::endl;
	}

	utils::press_any_key();
	return EXIT_SUCCESS;
}