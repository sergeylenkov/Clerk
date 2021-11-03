#include "CopyTransactionCommand.h"

using namespace Clerk::Commands;

CopyTransactionCommand::CopyTransactionCommand(ICommandsReceiver* receiver) : _transactionId(-1) {
	_receiver = receiver;
}

void CopyTransactionCommand::SetTransactionId(int id) {
	_transactionId = id;
}

void CopyTransactionCommand::Execute() {
	_receiver->OpenCopyTransactionDialog(_transactionId);
}