#include "NewAccountCommand.h"

using namespace Clerk::Commands;

NewAccountCommand::NewAccountCommand(ICommandsReceiver* receiver) : _accountType(AccountType::Deposit) {
	_receiver = receiver;
}

void NewAccountCommand::SetAccountType(AccountType type) {
	_accountType = type;
}

void NewAccountCommand::Execute() {
	_receiver->OpenNewAccountDialog(_accountType);
}