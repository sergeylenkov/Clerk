#include "EditAlertCommand.h"

using namespace Clerk::Commands;

EditAlertCommand::EditAlertCommand(ICommandsReceiver& receiver):
	_receiver(receiver),
	_alertId(-1) {
}

void EditAlertCommand::SetAlertId(int id) {
	_alertId = id;
}

void EditAlertCommand::Execute() {
	_receiver.OpenEditAlertDialog(_alertId);
}