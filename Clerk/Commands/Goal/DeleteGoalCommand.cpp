#include "DeleteGoalCommand.h"

using namespace Clerk::Commands;

DeleteGoalCommand::DeleteGoalCommand(GoalsService& service):
	_service(service),
	_goalId(-1) {
}

void DeleteGoalCommand::SetGoalId(int id) {
	_goalId = id;
}

void DeleteGoalCommand::Execute() {
	auto goal = _service.GetById(_goalId);

	if (goal) {
		_service.Delete(*goal);
	}
}