#include "ReportsService.h"

ReportsService::ReportsService(ReportsRepository& reportsRepository) :
	_reportsRepository(reportsRepository)
{
}

std::vector<std::shared_ptr<ReportPresentationModel>> ReportsService::GetAll() {
	auto reports = _reportsRepository.GetAll();

	std::vector<std::shared_ptr<ReportPresentationModel>> result;

	std::transform(reports.begin(), reports.end(), std::back_inserter(result), [&](const std::shared_ptr<ReportModel>& report) {
		auto model = std::make_shared<ReportPresentationModel>(*report);

		return model;
	});

	return result;
}

std::shared_ptr<ReportPresentationModel> ReportsService::GetById(int id) {
	auto report = _reportsRepository.GetById(id);

	if (report) {
		auto model = std::make_shared<ReportPresentationModel>(*report);

		return model;
	}

	return nullptr;
}
