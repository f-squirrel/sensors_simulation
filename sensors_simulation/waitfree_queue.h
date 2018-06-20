#ifndef WAITFREE_QUEUE_H
#define WAITFREE_QUEUE_H

#include <atomic>
#include <memory>


template<typename T>
class waitfree_queue {
public:
	struct node {
		node(const T &data) : data_(data) {}
		T data_;
		node * next;
	};
	void push(const T &data) {
		node * n = new node(data);
		node * stale_head = head_.load(std::memory_order_relaxed);
		do {
			n->next = stale_head;
		} while (!head_.compare_exchange_weak(stale_head, n, std::memory_order_release, std::memory_order_relaxed));
	}

	node * pop_all(void) {
		node * last = pop_all_reverse(), * first = 0;
		while(last) {
			node * tmp = last;
			last = last->next;
			tmp->next = first;
			first = tmp;
		}
		return first;
	}

  waitfree_queue() : head_(0) {}

  // alternative interface if ordering is of no importance
	node * pop_all_reverse(void) {
		return head_.exchange(0, std::memory_order_consume);
		//return head_.exchange(head_, std::memory_order_consume);
  }
private:
	std::atomic<node *> head_;
};

#endif //WAITFREE_QUEUE_H
