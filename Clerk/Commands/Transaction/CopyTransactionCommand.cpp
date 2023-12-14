#include "CopyTransactionCommand.h"

using namespace Clerk::Commands;

CopyTransactionCommand::CopyTransactionCommand(ICommandsReceiver& receiver):
	_receiver(receiver), 
	_transactionId(-1) {
}

void CopyTransactionCommand::SetTransactionId(int id) {
	_transactionId = id;
}

void CopyTransactionCommand::Execute() {
	_receiver.CopyTransaction(_transactionId);
}