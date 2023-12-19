#include "RestoreAccountCommand.h"

using namespace Clerk::Commands;

RestoreAccountCommand::RestoreAccountCommand(AccountsService& service) :
	_service(service),
	_accountId(-1) {
}

void RestoreAccountCommand::SetAccountId(int id) {
	_accountId = id;
}

void RestoreAccountCommand::Execute() {
	auto account = _service.GetById(_accountId);

	if (account) {
		_service.Restore(*account);
	}
}