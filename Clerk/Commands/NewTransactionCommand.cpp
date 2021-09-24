#include "NewTransactionCommand.h"

using namespace Clerk::Commands;

NewTransactionCommand::NewTransactionCommand(ICommandsReceiver* receiver): _transactionId(-1), _isSplit(false) {
	_receiver = receiver;
}

void NewTransactionCommand::SetTransactionId(int id) {
	_transactionId = id;
}

void NewTransactionCommand::SetIsSplit(bool isSplit) {
	_isSplit = isSplit;
}

void NewTransactionCommand::Execute() {
	_receiver->OpenTransactionDialog(_transactionId, _isSplit);
}