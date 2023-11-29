#include "ReportsService.h"

ReportsService::ReportsService(ReportsRepository& reportsRepository) :
	_reportsRepository(reportsRepository)
{
	_isLoading = false;
}

std::shared_ptr<ReportPresentationModel> ReportsService::GetById(int id) {
	auto report = GetFromHash(id);

	if (report) {
		return report;
	}

	auto model = _reportsRepository.GetById(id);

	if (model) {
		report = std::make_shared<ReportPresentationModel>(*model);

		AddToHash(report->id, report);

		delete model;

		return report;
	}

	return nullptr;
}

shared_vector<ReportPresentationModel> ReportsService::GetAll() {
	if (_isLoading && GetHashList().size() > 0) {
		return GetHashList();
	}

	auto models = _reportsRepository.GetAll();

	for (auto& model : models) {
		if (!GetFromHash(model->id)) {
			auto report = std::make_shared<ReportPresentationModel>(*model);
			AddToHash(report->id, report);
		}
	}

	for (auto p : models) {
		delete p;
	}

	models.erase(models.begin(), models.end());

	_isLoading = true;

	return GetHashList();
}