#include "OpenAccountsTabCommand.h"

using namespace Clerk::Commands;

OpenAccountsTabCommand::OpenAccountsTabCommand(ICommandsReceiver* receiver) : _accountType(AccountType::Deposit) {
	_receiver = receiver;
	_accountType = std::nullopt;
}

void OpenAccountsTabCommand::SetAccountType(AccountType type) {
	_accountType = type;
}

void OpenAccountsTabCommand::Execute() {	
	_receiver->OpenAccountsTab(_accountType);

	_accountType = std::nullopt;
}