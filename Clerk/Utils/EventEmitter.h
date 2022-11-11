#pragma once

#include <vector>
#include <functional>

class EventEmitter {
public:
	~EventEmitter();

	void Subscribe(std::function<void()> function);
	void Unsubscribe();
	void Emit();
	void Clear();

private:
	std::vector<std::function<void()>> _subscriptions;
};
