#include "BudgetsService.h"

using namespace Clerk::Data;

BudgetsService::BudgetsService(BudgetsRepository& budgetsRepository):
	_budgetsRepository(budgetsRepository)
{
	_eventEmitter = new EventEmitter();
	_isLoading = false;
}

BudgetsService::~BudgetsService() {
	delete _eventEmitter;
}

unsigned int BudgetsService::Subscribe(std::function<void()> fn) {
	return _eventEmitter->Subscribe(fn);
}

void BudgetsService::Unsubscribe(unsigned int subscriptionId) {
	_eventEmitter->Unsubscribe(subscriptionId);
}

std::shared_ptr<BudgetPresentationModel> BudgetsService::GetById(int id) {
	auto budget = GetFromHash(id);

	if (budget) {
		return budget;
	}

	auto model = _budgetsRepository.GetById(id);

	if (model) {
		budget = std::make_shared<BudgetPresentationModel>(*model);

		budget->balance = GetExpenses(*budget, budget->periodDate.FormatISODate().ToStdString(), wxDateTime::Now().FormatISODate().ToStdString());
		budget->remainAmount = budget->amount - budget->balance;
		budget->remainPercent = budget->balance / (budget->amount / 100.0);

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

			budget->balance = GetExpenses(*budget, budget->periodDate.FormatISODate().ToStdString(), wxDateTime::Now().FormatISODate().ToStdString());
			budget->remainAmount = budget->amount - budget->balance;
			budget->remainPercent = budget->balance / (budget->amount / 100.0);

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
		budget->balance = GetExpenses(*budget, budget->periodDate.FormatISODate().ToStdString(), wxDateTime::Now().FormatISODate().ToStdString());
	}
}

float BudgetsService::GetExpenses(BudgetPresentationModel& budget, std::string& fromDate, std::string& toDate) {	
	return _budgetsRepository.GetExpenses(String::Join(budget.accountsIds, ","), budget.periodDate.FormatISODate().ToStdString(), wxDateTime::Now().FormatISODate().ToStdString());
}

std::shared_ptr<BudgetPresentationModel> BudgetsService::Save(BudgetPresentationModel& budget) {
	BudgetModel& model = budget;

	int id = _budgetsRepository.Save(model);

	delete& model;

	_eventEmitter->Emit();

	RemoveFromHash(id);

	return GetById(id);
}

void BudgetsService::Delete(BudgetPresentationModel& budget) {
	BudgetModel& model = budget;

	_budgetsRepository.Delete(model);

	delete& model;

	RemoveFromHash(budget.id);

	_eventEmitter->Emit();
}