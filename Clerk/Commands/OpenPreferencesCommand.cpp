#include "OpenPreferencesCommand.h"

using namespace Clerk::Commands;

OpenPreferencesCommand::OpenPreferencesCommand(ICommandsReceiver* receiver) {
	_receiver = receiver;
}

void OpenPreferencesCommand::Execute() {
	_receiver->OpenPreferencesDialog();
}