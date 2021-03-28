#pragma once

#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <memory>
#include <cctype>
#include "HashRepository.h"
#include "../Models/TagModel.h"

namespace Clerk {
	namespace Data {
		class TagsRepository : HashRepository<TagModel> {
		public:
			using HashRepository::HashRepository;

			std::vector<std::shared_ptr<TagModel>> GetAll();
			std::shared_ptr<TagModel> GetById(int id);
			std::vector<std::shared_ptr<TagModel>> TagsRepository::GetBySearch(std::string search);
			int GetCount(int id);
			void Save(TagModel& tag);
			void Delete(const TagModel& tag);

		private:
			std::shared_ptr<TagModel> Load(int id);
		};
	}
}
#pragma once
