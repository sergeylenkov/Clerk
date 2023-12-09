#include "OpenTabCommand.h"

using namespace Clerk::Commands;

OpenTabCommand::OpenTabCommand(ICommandsReceiver* receiver) : _tabType(TabType::Dashboard) {
	_receiver = receiver;
}

void OpenTabCommand::SetType(TabType type) {
	_tabType = type;
}

void OpenTabCommand::Execute() {
	_receiver->OpenTab(_tabType);
}