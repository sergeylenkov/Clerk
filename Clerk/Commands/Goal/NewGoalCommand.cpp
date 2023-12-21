#include "NewGoalCommand.h"

using namespace Clerk::Commands;

NewGoalCommand::NewGoalCommand(ICommandsReceiver& receiver):
	_receiver(receiver) {
}

void NewGoalCommand::Execute() {
	_receiver.OpenNewGoalDialog();
}