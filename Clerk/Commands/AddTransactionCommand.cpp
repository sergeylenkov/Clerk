#include "AddTransactionCommand.h"

using namespace Clerk::Commands;

AddTransactionCommand::AddTransactionCommand(ICommandsReceiver* receiver) {
	_receiver = receiver;
}

void AddTransactionCommand::Execute() {
	_receiver->OpenTransactionDialog();
}