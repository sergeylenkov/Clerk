#pragma once

#include "../../Utils/Types.h"
#include "../../Utils/Utils.h"
#include "../Repositories/TagsRepository.h"
#include "../PresentationModels/TagPresentationModel.h"
#include "HashService.h"

using namespace Clerk::Utils;

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


