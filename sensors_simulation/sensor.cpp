#include "sensor.h"
#include "utils.h"
#include <iostream>
#include <atomic>
#include <thread>


id_t sensor::msg_num_ = 0;

void sensor::add_connection(sensor_p_t sensor) {
	connected_sensors_.push_back(sensor);
}

void sensor::start(unsigned& message_undelivered,  std::atomic<bool>& go, const message& message_to_send, std::chrono::steady_clock::time_point& tend) {

	message_list_.push(message_to_send);

	while (!go) {
		std::this_thread::yield();
	}

	while( message_undelivered > 0 && std::chrono::steady_clock::now() < tend ) {
		waitfree_queue<message>::node* x =  message_list_.pop_all();
		while (x) {

			waitfree_queue<message>::node* tmp = x;
			x = x->next;
			if(tmp->data_.reciever_id() == id()) {
				//std::cout << "message " << tmp->data_.id() << " delivered to " << id() << std::endl;
				--message_undelivered;
				delivered_msgs_.push_back(tmp->data_);
			}
			else {
				message msg_2_send = tmp->data_;
				msg_2_send.add_transmission();
				send_message(msg_2_send);
			}
			delete tmp;
		}
		std::this_thread::yield();
	}
}

void sensor::send_message(const message& msg_to_send) {
	sensor_p_t msg_receiver = get_sensor_to_send();
	msg_receiver->push_message(msg_to_send);
}

void sensor::push_message(const message& msg) {
	message_list_.push(msg);
}

void sensor::collect_messages(message_collector_t& messages) {
	messages.insert(delivered_msgs_.begin(), delivered_msgs_.end());
	waitfree_queue<message>::node* x =  message_list_.pop_all();
	while (x) {
		waitfree_queue<message>::node* tmp = x;
		x = x->next;
		messages.insert(tmp->data_);
		delete tmp;
	}
}

void sensor::print_connections() {
	std::cout << id() << "\t\t";
	for (connected_sensors_t::iterator it = connected_sensors_.begin(); it !=  connected_sensors_.end(); ++it) {
		std::cout << (*it)->id() << " ";
	}
	std::cout << std::endl;
}

sensor_p_t sensor::get_sensor_to_send() {
	if (connected_sensors_.empty()) {
		return nullptr;
	}
	return connected_sensors_[utils::generate_random_number(0, connected_sensors_.size() - 1)];
}