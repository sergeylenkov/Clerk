#pragma once

#include <vector>
#include <functional>
#include <map>

namespace Clerk {
	namespace Utils {
		class EventEmitter {
		public:
			EventEmitter();
			~EventEmitter();
			unsigned int Subscribe(std::function<void()> function);
			void Unsubscribe(unsigned int key);
			void Emit();
			void Clear();

		private:
			unsigned int _subscribeId;
			std::map<unsigned int, std::function<void()>> _subscriptions;
		};
	}
}
