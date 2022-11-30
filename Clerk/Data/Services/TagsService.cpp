#include "TagsService.h"

TagsService::TagsService(TagsRepository& tagsRepository):
	_tagsRepository(tagsRepository)
{
}

std::vector<std::shared_ptr<TagPresentationModel>> TagsService::GetAll() {
	auto tags = _tagsRepository.GetAll();

	std::vector<std::shared_ptr<TagPresentationModel>> result;

	std::transform(tags.begin(), tags.end(), std::back_inserter(result), [&](const std::shared_ptr<TagModel>& tag) {
		auto model = std::make_shared<TagPresentationModel>(*tag);

		model->count = _tagsRepository.GetCount(tag->id);

		return model;
	});

	return result;
}

std::shared_ptr<TagPresentationModel> TagsService::GetById(int id) {
	auto tag = _tagsRepository.GetById(id);

	if (tag) {
		auto model = std::make_shared<TagPresentationModel>(*tag);

		return model;
	}

	return nullptr;
}

std::vector<std::shared_ptr<TagPresentationModel>> TagsService::GetBySearch(wxString search) {
	auto tags = _tagsRepository.GetBySearch(search.ToStdWstring());
	
	std::vector<std::shared_ptr<TagPresentationModel>> result;

	std::transform(tags.begin(), tags.end(), std::back_inserter(result), [&](const std::shared_ptr<TagModel>& tag) {
		return std::make_shared<TagPresentationModel>(*tag);
	});

	return result;
}

std::shared_ptr<TagPresentationModel> TagsService::Save(std::shared_ptr<TagPresentationModel> tag) {
	auto model = tag->GetModel();

	auto savedModel = _tagsRepository.Save(model);

	return GetById(savedModel->id);
}
