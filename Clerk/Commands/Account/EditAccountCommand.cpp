#include "EditAccountCommand.h"

using namespace Clerk::Commands;

EditAccountCommand::EditAccountCommand(ICommandsReceiver& receiver):
	_receiver(receiver), 
	_accountId(-1) {
}

void EditAccountCommand::SetAccountId(int id) {
	_accountId = id;
}

void EditAccountCommand::Execute() {
	_receiver.OpenEditAccountDialog(_accountId);
}