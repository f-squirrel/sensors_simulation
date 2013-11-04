#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
typedef unsigned int id_t;

class message {

	const id_t id_;
	const id_t reciever_id_;
	size_t transmissions_cnt_;
public:
	message(id_t id, id_t reciever_id)
		: id_(id), reciever_id_(reciever_id), transmissions_cnt_(0) {
	}

	const id_t id() const			{ return id_; }
	const id_t reciever_id() const	{ return reciever_id_; }
	size_t transmassions() const	{ return transmissions_cnt_; }
	void add_transmission()			{ ++transmissions_cnt_; }

	struct compare_by_transmission {
		bool operator () (const message& lhs, const message& rhs) {
			return lhs.transmissions_cnt_ > rhs.transmissions_cnt_;
		}
	};
};

#endif // !MESSAGE_H
