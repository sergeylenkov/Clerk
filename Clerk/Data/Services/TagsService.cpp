#include "TagsService.h"

TagsService::TagsService(TagsRepository& tagsRepository):
	_tagsRepository(tagsRepository)
{
	_isLoading = false;
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

	std::wstring wSearch = search.ToStdWstring();
	auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());
	f.tolower(&wSearch[0], &wSearch[0] + wSearch.size());

	for (auto& tag : GetAll()) {
		std::wstring tagName = tag->name;
		f.tolower(&tagName[0], &tagName[0] + tagName.size());

		std::size_t found = tagName.find(wSearch);

		if (found != std::string::npos) {
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
