#include "EditTransactionCommand.h"

using namespace Clerk::Commands;

EditTransactionCommand::EditTransactionCommand(ICommandsReceiver* receiver) : _transactionId(-1) {
	_receiver = receiver;
}

void EditTransactionCommand::SetTransactionId(int id) {
	_transactionId = id;
}

void EditTransactionCommand::Execute() {
	_receiver->EditTransaction(_transactionId);
}