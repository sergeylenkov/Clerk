#include "QuitCommand.h"

using namespace Clerk::Commands;

QuitCommand::QuitCommand(ICommandsReceiver* receiver) {
	_receiver = receiver;
}

void QuitCommand::Execute() {
	_receiver->Quit();
}