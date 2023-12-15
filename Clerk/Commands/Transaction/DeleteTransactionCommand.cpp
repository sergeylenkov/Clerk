#include "DeleteTransactionCommand.h"

using namespace Clerk::Commands;

DeleteTransactionCommand::DeleteTransactionCommand(TransactionsService& service):
	_service(service),
	_transactionId(-1)
{
}

void DeleteTransactionCommand::SetTransactionId(int id) {
	_transactionId = id;
}

void DeleteTransactionCommand::Execute() {
	auto transaction = _service.GetById(_transactionId);

	if (transaction) {
		_service.Delete(*transaction);
	}
}