#include "EditGoalCommand.h"

using namespace Clerk::Commands;

EditGoalCommand::EditGoalCommand(ICommandsReceiver& receiver):
	_receiver(receiver),
	_goalId(-1) {
}

void EditGoalCommand::SetGoalId(int id) {
	_goalId = id;
}

void EditGoalCommand::Execute() {
	_receiver.OpenEditGoalDialog(_goalId);
}