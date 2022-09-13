#include "CommandsReceiver.h"

using namespace Clerk::Commands;

CommandsReceiver::CommandsReceiver() {

}

CommandsReceiver::CommandsReceiver(wxFrame* frame, DialogsController* dialogsController, TabsController* tabsController, TransactionController* transactionController) {
	_frame = frame;
	_dialogsController = dialogsController;
	_tabsController = tabsController;
	_transactionController = transactionController;
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

void CommandsReceiver::NewTransaction(int id) {
	_transactionController->New(id);
}

void CommandsReceiver::CopyTransaction(int id) {
	_transactionController->Copy(id);
}

void CommandsReceiver::SplitTransaction(int id) {
	_transactionController->Split(id);
}

void CommandsReceiver::EditTransaction(int id) {
	_transactionController->Edit(id);
}

void CommandsReceiver::DeleteTransaction(int id) {
	_transactionController->Delete(id);
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