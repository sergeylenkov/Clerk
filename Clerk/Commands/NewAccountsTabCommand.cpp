#include "NewAccountsTabCommand.h"

using namespace Clerk::Commands;

NewAccountsTabCommand::NewAccountsTabCommand(ICommandsReceiver* receiver) : _accountType(AccountType::Deposit) {
	_receiver = receiver;
}

void NewAccountsTabCommand::SetAccountType(AccountType type) {
	_accountType = type;
}

void NewAccountsTabCommand::Execute() {
	_receiver->OpenNewAccountsTab(_accountType);
}