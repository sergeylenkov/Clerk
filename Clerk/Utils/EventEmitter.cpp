#include "EventEmitter.h"

EventEmitter::EventEmitter() {
	_key = 1;
}
EventEmitter::~EventEmitter() {
	_subscriptions.clear();
}

unsigned int EventEmitter::Subscribe(std::function<void()> function) {
	_key++;

	_subscriptions[_key] = function;

	return _key;
}

void EventEmitter::Unsubscribe(unsigned int key) {
	_subscriptions.erase(key);
}

void EventEmitter::Emit() {
	for (const auto& [key, fn] : _subscriptions) {
		fn();
	}
}

void EventEmitter::Clear() {
	_subscriptions.clear();
}