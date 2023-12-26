#include "TagsService.h"

TagsService::TagsService(TagsRepository& tagsRepository):
	_tagsRepository(tagsRepository)
{
	_isLoading = false;
	GetAll();
}

std::shared_ptr<TagPresentationModel> TagsService::GetById(int id) {
	auto tag = GetFromHash(id);

	if (tag) {
		return tag;
	}

	auto model = _tagsRepository.GetById(id);

	if (model) {
		auto tag = std::make_shared<TagPresentationModel>(*model);
		tag->count = _tagsRepository.GetCount(model->id);

		AddToHash(tag->id, tag);

		delete model;

		return tag;
	}

	return nullptr;
}

shared_vector<TagPresentationModel> TagsService::GetAll() {
	if (_isLoading && GetHashList().size() > 0) {
		return GetHashList();
	}

	auto models = _tagsRepository.GetAll();

	for (auto& model : models) {
		if (!GetFromHash(model->id)) {
			auto tag = std::make_shared<TagPresentationModel>(*model);
			tag->count = _tagsRepository.GetCount(model->id);

			AddToHash(tag->id, tag);
		}
	}

	for (auto p : models) {
		delete p;
	}

	models.erase(models.begin(), models.end());

	_isLoading = true;

	return GetHashList();
}

shared_vector<TagPresentationModel> TagsService::GetBySearch(wxString search) {
	shared_vector<TagPresentationModel> result;

	for (auto& tag : GetAll()) {
		if (String::Search(tag->name, search)) {
			result.push_back(tag);
		}
	}

	return result;
}

std::shared_ptr<TagPresentationModel> TagsService::Save(TagPresentationModel& tag) {
	TagModel& model = tag;

	int id = _tagsRepository.Save(model);

	delete& model;

	RemoveFromHash(id);

	return GetById(id);
}
