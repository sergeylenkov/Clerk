#include "SplitTransactionCommand.h"

using namespace Clerk::Commands;

SplitTransactionCommand::SplitTransactionCommand(ICommandsReceiver* receiver) : _transactionId(-1) {
	_receiver = receiver;
}

void SplitTransactionCommand::SetTransactionId(int id) {
	_transactionId = id;
}

void SplitTransactionCommand::Execute() {
	_receiver->SplitTransaction(_transactionId);
}