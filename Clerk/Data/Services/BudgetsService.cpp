#include "BudgetsService.h"

using namespace Clerk::Data;

BudgetsService::BudgetsService(BudgetsRepository& budgetsRepository) : _budgetsRepository(budgetsRepository) {
	_isLoading = false;
}

std::shared_ptr<BudgetPresentationModel> BudgetsService::GetById(int id) {
	auto budget = GetFromHash(id);

	if (budget) {
		return budget;
	}

	auto model = _budgetsRepository.GetById(id);

	if (model) {
		budget = std::make_shared<BudgetPresentationModel>(*model);
		budget->balance = GetExpenses(*budget, std::string(budget->periodDate.FormatISODate().ToUTF8()), std::string(wxDateTime::Now().FormatISODate().ToUTF8()));

		AddToHash(budget->id, budget);

		delete model;

		return budget;
	}

	return nullptr;
}

shared_vector<BudgetPresentationModel> BudgetsService::GetAll() {
	if (_isLoading && GetHashList().size() > 0) {
		return GetHashList();
	}

	auto models = _budgetsRepository.GetAll();

	for (auto& model : models) {
		if (!GetFromHash(model->id)) {
			auto budget = std::make_shared<BudgetPresentationModel>(*model);

			budget->balance = GetExpenses(*budget, std::string(budget->periodDate.FormatISODate().ToUTF8()), std::string(wxDateTime::Now().FormatISODate().ToUTF8()));

			AddToHash(budget->id, budget);
		}
	}

	for (auto p : models) {
		delete p;
	}

	models.erase(models.begin(), models.end());

	_isLoading = true;

	return GetHashList();
}

void BudgetsService::UpdateBalance() {
	for (auto& budget : GetHashList()) {
		budget->balance = GetExpenses(*budget, std::string(budget->periodDate.FormatISODate().ToUTF8()), std::string(wxDateTime::Now().FormatISODate().ToUTF8()));
	}
}

float BudgetsService::GetExpenses(BudgetPresentationModel& budget, std::string& fromDate, std::string& toDate) {	
	return _budgetsRepository.GetExpenses(budget.GetAccountsIdsString().ToStdString(), std::string(budget.periodDate.FormatISODate().ToUTF8()), std::string(wxDateTime::Now().FormatISODate().ToUTF8()));
}