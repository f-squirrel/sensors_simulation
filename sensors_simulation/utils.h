#ifndef UTILS_H
#define UTILS_H


#ifdef __APPLE__
#include <stdlib.h>
#endif

namespace utils {

	size_t	generate_random_number(unsigned min, unsigned max);
	void	press_any_key();
	bool	check_input_data(unsigned int sens_num, unsigned int connections_num, unsigned int seconds_num);

} //namespace

#endif //UTILS_H