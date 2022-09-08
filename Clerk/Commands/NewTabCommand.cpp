#include "NewTabCommand.h"

using namespace Clerk::Commands;

NewTabCommand::NewTabCommand(ICommandsReceiver* receiver) : _tabType(TabType::Dashboard) {
	_receiver = receiver;
}

void NewTabCommand::SetType(TabType type) {
	_tabType = type;
}

void NewTabCommand::Execute() {
	_receiver->OpenNewTab(_tabType);
}