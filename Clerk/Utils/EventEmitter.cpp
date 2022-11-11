#include "EventEmitter.h"

EventEmitter::~EventEmitter() {
	_subscriptions.clear();
}

void EventEmitter::Subscribe(std::function<void()> function) {
	_subscriptions.push_back(function);
}

void EventEmitter::Unsubscribe() {
	//
}

void EventEmitter::Emit() {
	for (auto& fn : _subscriptions) {
		fn();
	}
}

void EventEmitter::Clear() {
	_subscriptions.clear();
}