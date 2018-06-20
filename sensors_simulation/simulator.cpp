#include "simulator.h"
#include "utils.h"

#include <boost/graph/connected_components.hpp>
#include <boost/graph/random.hpp>

#include <iostream>
#include <random>
#include <thread>


using namespace boost;

simulator::simulator(unsigned int sensor_num, unsigned int edges_per_sensor, unsigned int seconds ) :
		g(sensor_num), sensor_num_(sensor_num), edges_per_sensor_(edges_per_sensor), seconds_(seconds) {
}

simulator::~simulator() {
	try {
		clean_sensor_network();
	}
	catch(...) {
		std::cerr << "failed to clean network\n";
	}
}

void simulator::run() {
	generate_random_connections();
	show_sensors_connections();

	if (!is_network_ready()) {
		utils::press_any_key();
		return;
	}

	start_simulation();
	output_messages();
}

void  simulator::generate_random_connections() {

	std::random_device generate_seed;
	std::mt19937 gen(generate_seed());

	item_descriptor v_end = *vertices(g).second;
	size_t Gsz = num_vertices(g);
	for (unsigned int conn_per_sensor = 1; conn_per_sensor <= edges_per_sensor_; ++conn_per_sensor) {
		for ( item_descriptor v = *vertices(g).first; v != v_end; ++v ) { //
			//std::cout << "generating for vertex: " << v << "  candidate: ";
			if(degree(v, g) >= conn_per_sensor) {
				//std::cout << std::endl;
				continue;
			}

			item_descriptor u;
			std::set<id_t> tries;
			do {
				u = random_vertex(g, gen);
				//std::cout << u << "  ";
				id_t id = g[u].get_sensor()->id();
				tries.insert(id);
				if ( tries.size() == Gsz ) {
					u = v_end;
					break;
				}
			} while( v == u || u == v_end || ( degree(u, g) == edges_per_sensor_ ) );
			if( u != v_end ) {
				auto pp = add_edge(v, u, g);
				//std::cout << std::boolalpha << pp.second << " ";
				if(pp.second) {
					g[v].get_sensor()->add_connection(g[u].get_sensor());
					g[u].get_sensor()->add_connection(g[v].get_sensor());
				}
			}
			//std::cout << std::endl;
		}
	}
}

void simulator::show_sensors_connections() {
	connection_iterator ai, ai_end;
	item_iterator v, v_end;
	std::cout << "sensor ID    connected sensors\n";
	for ( std::tie(v, v_end) = vertices(g); v != v_end; ++v ) {
		g[*v].get_sensor()->print_connections();
	}
	std::cout << std::endl;
}

bool simulator::is_network_ready() {
	bool is_connected = is_network_connected();
	bool is_vertex_degree = is_item_degree_expected();
	//std::cout << "is network connected: " << std::boolalpha << is_connected  << std::endl;
	//std::cout << "does every vertex have " << edges_per_sensor_ << " connection(s)? " << std::boolalpha << is_vertex_degree << std::endl;
	return (is_connected && is_vertex_degree);
}

bool simulator::is_network_connected() {
	std::vector<network::vertices_size_type> component(num_vertices(g));
    network::vertices_size_type num = connected_components(g, &component[0]);
   	return (num <= 1);
}

bool simulator::is_item_degree_expected() {
	item_descriptor v_end = *vertices(g).second;
	for ( item_descriptor v = *vertices(g).first; v != v_end; ++v ) { //
		if (degree(v, g) != edges_per_sensor_) {
			return false;
		}
	}
	return true;
}

void simulator::start_simulation() {

	connection_iterator ai, ai_end;
	item_iterator v, v_end;

	//std::cout << "starting simulation...\n";
	unsigned int undelivered_msg_num = num_vertices(g);
	std::vector<std::thread> thread_vector(undelivered_msg_num);

	std::chrono::steady_clock::time_point tend = std::chrono::steady_clock::now()
		+ std::chrono::seconds(seconds_);
	std::atomic<bool> go(false);
	int i = 0;
	for ( std::tie(v, v_end) = vertices(g); v != v_end; ++v, ++i ) {
		sensor_p_t sensor = g[*v].get_sensor();
		message msg(sensor->id(), utils::generate_random_number(0, undelivered_msg_num - 1));
		thread_vector[i] = std::thread(&sensor::start, std::ref(sensor), std::ref(undelivered_msg_num), std::ref(go), std::ref(msg), std::ref(tend));
	}

	go = true;
	for (std::vector<std::thread>::iterator it = thread_vector.begin(); it != thread_vector.end(); it++) {
		(*it).join();
	}
	//std::cout << "simulation stoped...\n";
}

void simulator::output_messages() {
	message_collector_t messages;

	item_iterator v, v_end;
	for ( std::tie(v, v_end) = vertices(g); v != v_end; ++v ) {
		sensor_p_t sensor = g[*v].get_sensor();
		sensor->collect_messages(messages);
	}

	std::cout << "message ID   receiver ID   transmissions\n";
	message_collector_t::iterator end = messages.end();
	for (message_collector_t::iterator it = messages.begin(); it != end; ++it) {
		std::cout << (*it).id() << "\t\t" << (*it).reciever_id() << "\t\t" << (*it).transmassions() << std::endl;
	}
}

void simulator::clean_sensor_network() {
	for ( item_descriptor v = *vertices(g).first; v!=*vertices(g).second; ++v) {
		delete g[v].get_sensor();
	}
}
