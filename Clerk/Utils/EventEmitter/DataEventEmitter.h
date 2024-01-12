#pragma once

#include <vector>
#include <functional>
#include <map>

namespace Clerk {
	namespace Utils {
		template <typename... Args>
		class DataEventEmitter {
		public:
			DataEventEmitter() {
				_subscribeId = 1;
			}

			~DataEventEmitter() {
				_subscriptions.clear();
			}

			unsigned int Subscribe(std::function<void(Args...)> function) {
				_subscribeId++;

				_subscriptions[_subscribeId] = function;

				return _subscribeId;
			}

			void Unsubscribe(unsigned int key) {
				_subscriptions.erase(key);
			}

			void Emit(Args... args) {
				for (const auto& [key, fn] : _subscriptions) {
					fn(args...);
				}
			}

			void Clear() {
				_subscriptions.clear();
			}

		private:
			unsigned int _subscribeId;
			std::map<unsigned int, std::function<void(Args...)>> _subscriptions;
		};
	}
}
