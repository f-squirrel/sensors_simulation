
#include "utils.h"
#include <chrono>
#include <random>
#include <functional>
#include <iostream>

namespace utils {

std::size_t generate_random_number(unsigned min, unsigned max) {
	
	std::mt19937 typedef generator_t;
	std::random_device generate_seed;
	generator_t gen(generate_seed());
	std::uniform_int<> distrib(min, max);
	std::function<size_t()> uni_random(std::bind(distrib, gen));
	return uni_random();
}

void press_any_key() {
	system("pause");
}

bool check_input_data(unsigned int sens_num, unsigned int connections_num, unsigned int seconds_num) {
	
	if (sens_num == 0) {
		std::cerr << "error: sensor number is 0\n";
		return false;
	}
	if ( connections_num == 0 ) {
		std::cerr << "error: connections number is 0\n";
		return false;
	}
	if (connections_num >= sens_num) {
		std::cerr << "error: sensors number must be greater that connections number\n";
		return false;
	}
	if ( (sens_num - connections_num) < 1) {
		std::cerr << "sensor number must be greater than conncetion number for at least 1\n";
		return false;
	}
	if(seconds_num <= 0) {
		std::cerr << "please set more seconds\n";
		return false;
	}
	return true;
}

} //namespace