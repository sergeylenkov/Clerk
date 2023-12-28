#include "CommandsReceiver.h"

using namespace Clerk::Commands;

CommandsReceiver::CommandsReceiver() {

}

CommandsReceiver::CommandsReceiver(DialogsController* dialogsController, TabsPanel* tabsPanel):
	_dialogsController( dialogsController),
	_tabsPanel(tabsPanel)
{
	
}

void CommandsReceiver::OpenTab(TabType type) {
	_tabsPanel->OpenTab(type);
}

void CommandsReceiver::OpenAccountTab(int id) {
	_tabsPanel->OpenAccountTab(id);
}

void CommandsReceiver::OpenAccountsTab(std::optional<AccountType> type) {
	_tabsPanel->OpenAccountsTab(type);
}

void CommandsReceiver::OpenReportTab(int id) {
	_tabsPanel->OpenReportTab(id);
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

void CommandsReceiver::OpenNewAlertDialog() {
	_dialogsController->ShowNewAlertDialog();
}

void CommandsReceiver::OpenEditAlertDialog(int id) {
	_dialogsController->ShowEditAlertDialog(id);
}

void CommandsReceiver::OpenEditSchedulerDialog(int id) {

}

void CommandsReceiver::OpenNewBudgetDialog() {
	_dialogsController->ShowNewBudgetDialog();
}

void CommandsReceiver::OpenEditBudgetDialog(int id) {
	_dialogsController->ShowEditBudgetDialog(id);
}

void CommandsReceiver::OpenNewGoalDialog() {
	_dialogsController->ShowNewGoalDialog();
}

void CommandsReceiver::OpenEditGoalDialog(int id) {
	_dialogsController->ShowEditGoalDialog(id);
}