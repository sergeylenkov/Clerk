#include "ReportsService.h"

ReportsService::ReportsService(ReportsRepository& reportsRepository) :
	_reportsRepository(reportsRepository)
{
}

std::vector<std::shared_ptr<ReportViewModel>> ReportsService::GetAll() {
	auto reports = _reportsRepository.GetAll();

	std::vector<std::shared_ptr<ReportViewModel>> result;

	std::transform(reports.begin(), reports.end(), std::back_inserter(result), [this](const std::shared_ptr<ReportModel>& report) {
		auto model = std::make_shared<ReportViewModel>(*report);

		return model;
	});

	return result;
}

std::shared_ptr<ReportViewModel> ReportsService::GetById(int id) {
	auto report = _reportsRepository.GetById(id);

	if (report) {
		auto model = std::make_shared<ReportViewModel>(*report);

		return model;
	}

	return nullptr;
}
