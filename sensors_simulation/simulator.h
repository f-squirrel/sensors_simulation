#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "message.h"
#include "sensor.h"

#include <chrono>
#include <boost/graph/adjacency_list.hpp>


class simulator {

public:
	simulator(unsigned int sensor_num, unsigned int edges_per_sensor, unsigned int seconds );
	~simulator();
	void run();

private:
	simulator(const simulator&);
	simulator& operator = (const simulator&);
	class item {
		sensor* sensor_;
	public:
		item() : sensor_(new sensor) {}
		sensor* get_sensor() { return sensor_; }
	};

	typedef boost::adjacency_list <	boost::setS, boost::vecS,
						boost::undirectedS, item,
						boost::no_property, boost::no_property/*, boost::setS*/> network;

	typedef network::vertex_descriptor							item_descriptor;
	typedef boost::graph_traits<network>::adjacency_iterator	connection_iterator;
	typedef boost::graph_traits<network>::vertex_iterator		item_iterator;

	void generate_random_connections();
	void show_sensors_connections();
	bool is_network_ready();
	bool is_item_degree_expected();
	bool is_network_connected();
	void start_simulation();
	void output_messages();
	void clean_sensor_network();

	network g;
	const unsigned int sensor_num_;
	const unsigned int edges_per_sensor_;
	const unsigned int seconds_;
};

#endif //SIMULATOR_H
