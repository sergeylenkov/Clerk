#include "TagsService.h"

TagsService::TagsService(TagsRepository& tagsRepository):
	_tagsRepository(tagsRepository)
{
}

std::vector<std::shared_ptr<TagViewModel>> TagsService::GetAll() {
	auto tags = _tagsRepository.GetAll();

	std::vector<std::shared_ptr<TagViewModel>> result;

	std::transform(tags.begin(), tags.end(), std::back_inserter(result), [this](const std::shared_ptr<TagModel>& tag) {
		auto model = std::make_shared<TagViewModel>(*tag);

		model->count = _tagsRepository.GetCount(tag->id);

		return model;
	});

	return result;
}

std::shared_ptr<TagViewModel> TagsService::GetById(int id) {
	auto tag = _tagsRepository.GetById(id);

	if (tag) {
		auto model = std::make_shared<TagViewModel>(*tag);

		return model;
	}

	return nullptr;
}
