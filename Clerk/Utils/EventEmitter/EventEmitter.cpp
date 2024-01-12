#include "EventEmitter.h"

using namespace Clerk::Utils;

EventEmitter::EventEmitter() {
	_subscribeId = 1;
}
EventEmitter::~EventEmitter() {
	_subscriptions.clear();
}

unsigned int EventEmitter::Subscribe(std::function<void()> function) {
	_subscribeId++;

	_subscriptions[_subscribeId] = function;

	return _subscribeId;
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