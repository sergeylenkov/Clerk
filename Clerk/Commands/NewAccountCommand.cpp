#include "NewAccountCommand.h"

using namespace Clerk::Commands;

NewAccountCommand::NewAccountCommand(ICommandsReceiver& receiver):
	_receiver(receiver), 
	_accountType(AccountType::Deposit) {
}

void NewAccountCommand::SetAccountType(AccountType type) {
	_accountType = type;
}

void NewAccountCommand::Execute() {
	_receiver.OpenNewAccountDialog(_accountType);
}