#include "ArchiveAccountCommand.h"

using namespace Clerk::Commands;

ArchiveAccountCommand::ArchiveAccountCommand(AccountsService& service):
	_service(service),
	_accountId(-1) {
}

void ArchiveAccountCommand::SetAccountId(int id) {
	_accountId = id;
}

void ArchiveAccountCommand::Execute() {
	auto account = _service.GetById(_accountId);

	if (account) {
		_service.Archive(*account);
	}
}