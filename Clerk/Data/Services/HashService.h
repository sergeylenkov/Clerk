#pragma once

#include <map>
#include <vector>

using namespace Clerk::Data;

namespace Clerk {
	namespace Data {
		template <class T>
		class HashService {
		public:
			std::vector<T> GetHashList() {
				return _list;
			}

			void AddToHash(int id, T t) {
				if (t) {
					_hash[id] = t;
					_list.push_back(t);
				}
			}

			T GetFromHash(int id) {
				auto search = _hash.find(id);

				if (search != _hash.end()) {
					return search->second;
				}

				return nullptr;
			}

			void RemoveFromHash(int id) {
				auto model = GetFromHash(id);

				if (model) {
					_hash.erase(id);

					auto search = std::find_if(_list.begin(), _list.end(), [&model](T _t) { return (_t == model); });

					if (search != _list.end()) {
						_list.erase(search);
					}
				}
			}

		protected:
			bool _isLoading;
			std::vector<T> _list;
			std::map<int, T> _hash;
		};
	}
}