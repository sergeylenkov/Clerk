#include "NewBudgetCommand.h"

using namespace Clerk::Commands;

NewBudgetCommand::NewBudgetCommand(ICommandsReceiver& receiver):
	_receiver(receiver) {
}

void NewBudgetCommand::Execute() {
	//
}