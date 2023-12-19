#include "CommandsReceiver.h"

using namespace Clerk::Commands;

CommandsReceiver::CommandsReceiver() {

}

CommandsReceiver::CommandsReceiver(DialogsController* dialogsController, TabsController* tabsController):
	_dialogsController( dialogsController),
	_tabsController(tabsController)
{
	
}

void CommandsReceiver::OpenPreferencesDialog() {
	_dialogsController->ShowPreferencesDialog();
}

void CommandsReceiver::OpenAboutDialog() {
	_dialogsController->ShowAboutDialog();
}

void CommandsReceiver::NewTransaction(int id) {
	_dialogsController->ShowNewTransactionDialog(id);
}

void CommandsReceiver::CopyTransaction(int id) {
	_dialogsController->ShowCopyTransactionDialog(id);
}

void CommandsReceiver::SplitTransaction(int id) {
	_dialogsController->ShowSplitTransactionDialog(id);
}

void CommandsReceiver::EditTransaction(int id) {
	_dialogsController->ShowEditTransactionDialog(id);
}

void CommandsReceiver::OpenNewAccountDialog(AccountType type) {
	_dialogsController->ShowNewAccountDialog(type);
}

void CommandsReceiver::OpenEditAccountDialog(int id) {
	_dialogsController->ShowEditAccountDialog(id);
}

void CommandsReceiver::OpenTab(TabType type) {
	_tabsController->OpenTab(type);
}

void CommandsReceiver::OpenAccountTab(int id) {
	_tabsController->OpenAccountTab(id);
}

void CommandsReceiver::OpenAccountsTab(std::optional<AccountType> type) {
	_tabsController->OpenAccountsTab(type);
}

void CommandsReceiver::OpenReportTab(int id) {
	_tabsController->OpenReportTab(id);
}

void CommandsReceiver::OpenEditAlertDialog(int id) {
	_dialogsController->ShowEditAlertDialog(id);
}

void CommandsReceiver::OpenEditSchedulerDialog(int id) {

}