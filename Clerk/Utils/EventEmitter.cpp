#include "EventEmitter.h"

EventEmitter::~EventEmitter() {
	_subscriptions.clear();
}

void EventEmitter::Subscribe(std::function<void()> function) {
	_subscriptions.push_back(function);
}

void EventEmitter::Emit() {
	for (auto& fn : _subscriptions) {
		fn();
	}
}