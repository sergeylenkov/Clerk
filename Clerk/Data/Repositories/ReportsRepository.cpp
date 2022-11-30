#include "ReportsRepository.h"

using namespace Clerk::Data;

ReportsRepository::ReportsRepository(DataConnection& connection) : HashRepository(connection) {
	//TODO: Get reports from db
	auto report = std::make_shared<ReportModel>();

	report->id = 1;
	report->name = std::wstring(L"Expenses By Month");

	AddToHash(1, report);

	report = std::make_shared<ReportModel>();

	report->id = 2;
	report->name = std::wstring(L"Balance By Month");

	AddToHash(2, report);

	report = std::make_shared<ReportModel>();

	report->id = 3;
	report->name = std::wstring(L"Expenses For Period");

	AddToHash(3, report);
}

std::vector<std::shared_ptr<ReportModel>> ReportsRepository::GetAll() {
	return GetHashList();
}

std::shared_ptr<ReportModel> ReportsRepository::GetById(int id) {
	return GetFromHash(id);
}