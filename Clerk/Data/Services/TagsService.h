#pragma once

#include "../Repositories/TagsRepository.h"
#include "../ViewModels/TagViewModel.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace Data {
		class TagsService {
		public:
			TagsService(TagsRepository& tagsRepository);

			std::vector<std::shared_ptr<TagViewModel>> GetAll();
			std::shared_ptr<TagViewModel> GetById(int id);
			std::vector<std::shared_ptr<TagViewModel>> GetBySearch(wxString search);

		private:
			TagsRepository& _tagsRepository;
		};
	}
}


