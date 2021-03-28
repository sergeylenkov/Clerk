#include "AboutCommand.h"

using namespace Clerk::Commands;

AboutCommand::AboutCommand(ICommandsReceiver* receiver) {
	_receiver = receiver;
}

void AboutCommand::Execute() {
	_receiver->OpenAboutDialog();
}