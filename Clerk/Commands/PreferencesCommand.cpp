#include "PreferencesCommand.h"

using namespace Clerk::Commands;

PreferencesCommand::PreferencesCommand(ICommandsReceiver* receiver) {
	_receiver = receiver;
}

void PreferencesCommand::Execute() {
	_receiver->OpenPreferencesDialog();
}