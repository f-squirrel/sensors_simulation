#ifndef SENSOR_H
#define SENSOR_H

#include "message.h"
#include "waitfree_queue.h"
#include <chrono>
#include <list>
#include <set>
#include <vector>


class sensor;
typedef sensor* sensor_p_t;
typedef std::vector<sensor_p_t> connected_sensors_t;

typedef waitfree_queue<message> message_list_t;
typedef std::list<message> delivered_messages_t;
typedef std::multiset<message, message::compare_by_transmission> message_collector_t;

class sensor {

	message_list_t message_list_;
	const id_t id_;
	connected_sensors_t connected_sensors_;
public:
	explicit sensor() : id_(msg_num_++) {}

	void set_connections_cnt	(size_t size) { connected_sensors_.resize(size); }
	void add_connection			(sensor_p_t sensor);
	void push_message			(const message& msg);
	void print_connections		();

	void start					(	unsigned& message_undelivered,
									std::atomic<bool>& go,
									const message& message_to_send,
									std::chrono::steady_clock::time_point& tend);
	void collect_messages		(message_collector_t& messages);
	id_t id						() const { return id_; }
private:
	void send_message			(const message& msg_to_send);
	sensor_p_t get_sensor_to_send();
	delivered_messages_t delivered_msgs_;
	static id_t msg_num_;
};
#endif //SENSOR_H
