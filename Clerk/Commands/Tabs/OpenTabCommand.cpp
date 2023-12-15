#include "OpenTabCommand.h"

using namespace Clerk::Commands;

OpenTabCommand::OpenTabCommand(ICommandsReceiver& receiver):
	_receiver(receiver), 
	_tabType(TabType::Dashboard) {
}

void OpenTabCommand::SetType(TabType type) {
	_tabType = type;
}

void OpenTabCommand::Execute() {
	_receiver.OpenTab(_tabType);
}