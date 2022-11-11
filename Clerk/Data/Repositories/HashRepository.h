#pragma once

#include <map>
#include <vector>
#include "BaseRepository.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace Data {
		template <class T>
		class HashRepository : public BaseRepository {
		public:
			using BaseRepository::BaseRepository;

			std::vector<std::shared_ptr<T>> GetHashList() {
				return _list;
			}

			void AddToHash(int id, std::shared_ptr<T> t) {
				if (t) {
					_hash[id] = t;
					_list.push_back(t);
				}
			}

			std::shared_ptr<T> GetFromHash(int id) {
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

					auto search = std::find_if(_list.begin(), _list.end(), [&model](std::shared_ptr<T> _t) { return (_t == model); });

					if (search != _list.end()) {
						_list.erase(search);
					}
				}
			}

		private:
			std::vector<std::shared_ptr<T>> _list;
			std::map<int, std::shared_ptr<T>> _hash;
		};
	}
}