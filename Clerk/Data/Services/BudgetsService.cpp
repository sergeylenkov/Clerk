#include "BudgetsService.h"

using namespace Clerk::Data;

BudgetsService::BudgetsService(BudgetsRepository& budgetsRepository) : _budgetsRepository(budgetsRepository) {

}

std::shared_ptr<BudgetPresentationModel> BudgetsService::GetById(int id) {
	auto budget = _budgetsRepository.GetById(id);

	if (budget) {
		return std::make_shared<BudgetPresentationModel>(*budget);
	}

	return nullptr;
}

std::vector<std::shared_ptr<BudgetPresentationModel>> BudgetsService::GetAll() {
	auto budgets = _budgetsRepository.GetAll();

	std::vector<std::shared_ptr<BudgetPresentationModel>> result;

	std::transform(budgets.begin(), budgets.end(), std::back_inserter(result), [&](const std::shared_ptr<BudgetModel>& budget) {
		auto model = std::make_shared<BudgetPresentationModel>(*budget);
		model->balance = _budgetsRepository.GetExpenses(*budget, std::string(model->periodDate.FormatISODate().ToUTF8()), std::string(wxDateTime::Now().FormatISODate().ToUTF8()));

		return model;
	});

	return result;
}