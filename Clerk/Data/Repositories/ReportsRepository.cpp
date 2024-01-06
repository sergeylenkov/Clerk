#include "ReportsRepository.h"

using namespace Clerk::Data;

std::vector<ReportModel*> ReportsRepository::GetAll() {
	std::vector<ReportModel*> result;

	ReportModel* report = new ReportModel();

	report->id = 1;
	report->name = std::wstring(L"Expenses By Month");

	result.push_back(report);

	report = new ReportModel();

	report->id = 2;
	report->name = std::wstring(L"Expenses For Period");

	result.push_back(report);

	report = new ReportModel();

	report->id = 3;
	report->name = std::wstring(L"Balance By Month");

	result.push_back(report);	

	report = new ReportModel();

	report->id = 4;
	report->name = std::wstring(L"Receipts By Month");

	result.push_back(report);

	report = new ReportModel();

	report->id = 5;
	report->name = std::wstring(L"Receipts And Expenses By Month");

	result.push_back(report);

	return result;
}

ReportModel* ReportsRepository::GetById(int id) {
	return nullptr;
}