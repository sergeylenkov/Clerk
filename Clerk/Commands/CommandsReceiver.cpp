#include "CommandsReceiver.h"

using namespace Clerk::Commands;

CommandsReceiver::CommandsReceiver() {

}

CommandsReceiver::CommandsReceiver(wxFrame* frame, DialogsController* dialogsController, TabsController* tabsController) {
	_frame = frame;
	_dialogsController = dialogsController;
	_tabsController = tabsController;
}

void CommandsReceiver::Quit() {
	_frame->Close(TRUE);
}

void CommandsReceiver::OpenPreferencesDialog() {
	_dialogsController->ShowPreferencesDialog();
}

void CommandsReceiver::OpenAboutDialog() {
	_dialogsController->ShowAboutDialog();
}

void CommandsReceiver::OpenNewTransactionDialog(int id) {
	_dialogsController->ShowNewTransactionDialog(id);
}

void CommandsReceiver::OpenCopyTransactionDialog(int id) {
	_dialogsController->ShowCopyTransactionDialog(id);
}

void CommandsReceiver::OpenNewAccountDialog(AccountType type) {
	_dialogsController->ShowNewAccountDialog(type);
}

void CommandsReceiver::OpenEditAccountDialog(int id) {
	_dialogsController->ShowEditAccountDialog(id);
}

void CommandsReceiver::OpenNewTab(TabType type) {
	_tabsController->OpenNewTab(type);
}