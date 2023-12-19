#include "DuplicateTransactionCommand.h"

using namespace Clerk::Commands;

DuplicateTransactionCommand::DuplicateTransactionCommand(TransactionsService& service) :
	_service(service),
	_transactionId(-1)
{
}

void DuplicateTransactionCommand::SetTransactionId(int id) {
	_transactionId = id;
}

void DuplicateTransactionCommand::Execute() {
	auto transaction = _service.GetById(_transactionId);

	if (transaction) {
		_service.Duplicate(*transaction);
	}
}