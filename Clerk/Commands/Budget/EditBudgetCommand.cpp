#include "EditBudgetCommand.h"

using namespace Clerk::Commands;

EditBudgetCommand::EditBudgetCommand(ICommandsReceiver& receiver):
	_receiver(receiver),
	_budgetId(-1) {
}

void EditBudgetCommand::SetBudgetId(int id) {
	_budgetId = id;
}

void EditBudgetCommand::Execute() {
	_receiver.OpenEditBudgetDialog(_budgetId);
}