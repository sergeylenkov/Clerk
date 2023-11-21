#pragma once

#include <string>
#include <vector>
#include <memory>
#include "HashRepository.h"
#include "../Models/TagModel.h"

namespace Clerk {
	namespace Data {
		class TagsRepository : HashRepository<TagModel> {
		public:
			using HashRepository::HashRepository;

			std::vector<TagModel*> GetAll();
			TagModel* GetById(int id);
			int GetCount(int id);
			int Save(const TagModel& tag);
			void Delete(const TagModel& tag);

		private:
			TagModel* Load(int id);
		};
	}
}
#pragma once
