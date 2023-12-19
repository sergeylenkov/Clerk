#pragma once

#include "../Repositories/TagsRepository.h"
#include "../../UI/PresentationModels/TagPresentationModel.h"
#include "../../Utils/Types.h"
#include "HashService.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Data {
		class TagsService : HashService<std::shared_ptr<TagPresentationModel>> {
		public:
			TagsService(TagsRepository& tagsRepository);

			shared_vector<TagPresentationModel> GetAll();
			std::shared_ptr<TagPresentationModel> GetById(int id);
			shared_vector<TagPresentationModel> GetBySearch(wxString search);
			std::shared_ptr<TagPresentationModel> Save(TagPresentationModel& tag);

		private:
			TagsRepository& _tagsRepository;
		};
	}
}


