#include "DeleteBudgetCommand.h"

using namespace Clerk::Commands;

DeleteBudgetCommand::DeleteBudgetCommand(BudgetsService& service):
	_service(service),
	_budgetId(-1) {
}

void DeleteBudgetCommand::SetBudgetId(int id) {
	_budgetId = id;
}

void DeleteBudgetCommand::Execute() {
	auto budget = _service.GetById(_budgetId);

	if (budget) {
		_service.Delete(*budget);
	}
}