#pragma once

#include "../Repositories/TagsRepository.h"
#include "../../UI/PresentationModels/TagPresentationModel.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class TagsService {
		public:
			TagsService(TagsRepository& tagsRepository);

			std::vector<std::shared_ptr<TagPresentationModel>> GetAll();
			std::shared_ptr<TagPresentationModel> GetById(int id);
			std::vector<std::shared_ptr<TagPresentationModel>> GetBySearch(wxString search);
			std::shared_ptr<TagPresentationModel> Save(std::shared_ptr<TagPresentationModel> tag);

		private:
			TagsRepository& _tagsRepository;
		};
	}
}


