#include "CommandsInvoker.h"

using namespace Clerk::Commands;

CommandsInvoker::CommandsInvoker(
	QuitCommand& quitCommand,
	OpenPreferencesCommand& preferencesCommand, 
	AboutCommand& aboutCommand,
	NewTransactionCommand& newTransactionCommand, 
	CopyTransactionCommand& copyTransactionCommand,
	SplitTransactionCommand& splitTransactionCommand,
	EditTransactionCommand& editTransactionCommand,
	DeleteTransactionCommand& deleteTransactionCommand,
	NewAccountCommand& newAccountCommand,
	EditAccountCommand& editAccountCommand,
	OpenTabCommand& openTabCommand,
	OpenAccountTabCommand& openAccountTabCommand,
	OpenAccountsTabCommand& openAccountsTabCommand,
	OpenReportTabCommand& openReportTabCommand,
	EditAlertCommand& editAlertCommand,
	EditSchedulerCommand& editSchedulerCommand
):
	_quitCommand(quitCommand),
	_preferencesCommand(preferencesCommand),
	_aboutCommand(aboutCommand),
	_newTransactionCommand(newTransactionCommand),
	_copyTransactionCommand(copyTransactionCommand),
	_splitTransactionCommand(splitTransactionCommand),
	_editTransactionCommand(editTransactionCommand),
	_deleteTransactionCommand(deleteTransactionCommand),
	_newAccountCommand(newAccountCommand),
	_editAccountCommand(editAccountCommand),
	_openTabCommand(openTabCommand),
	_openAccountTabCommand(openAccountTabCommand),
	_openAccountsTabCommand(openAccountsTabCommand),
	_openReportTabCommand(openReportTabCommand),
	_editAlertCommand(editAlertCommand),
	_editSchedulerCommand(editSchedulerCommand)
{
	
}

CommandsInvoker::~CommandsInvoker() {
	delete& _quitCommand;
	delete& _preferencesCommand;
	delete& _aboutCommand;
	delete& _newTransactionCommand;
	delete& _copyTransactionCommand;
	delete& _splitTransactionCommand;
	delete& _editTransactionCommand;
	delete& _deleteTransactionCommand;
	delete& _newAccountCommand;
	delete& _editAccountCommand;
	delete& _openTabCommand;
	delete& _openAccountTabCommand;
	delete& _openAccountsTabCommand;
	delete& _openReportTabCommand;
	delete& _editAlertCommand;
	delete& _editSchedulerCommand;
}

void CommandsInvoker::Quit() {
	_quitCommand.Execute();
}

void CommandsInvoker::OpenPreferences() {
	_preferencesCommand.Execute();
}

void CommandsInvoker::OpenAbout() {
	_aboutCommand.Execute();
}

void CommandsInvoker::NewTransaction(int accountId) {
	_newTransactionCommand.SetAccountId(accountId);
	_newTransactionCommand.Execute();
}

void CommandsInvoker::CopyTransaction(int id) {
	_copyTransactionCommand.SetTransactionId(id);
	_copyTransactionCommand.Execute();
}

void CommandsInvoker::SplitTransaction(int id) {
	_splitTransactionCommand.SetTransactionId(id);
	_splitTransactionCommand.Execute();
}

void CommandsInvoker::EditTransaction(int id) {
	_editTransactionCommand.SetTransactionId(id);
	_editTransactionCommand.Execute();
}

void CommandsInvoker::DeleteTransaction(int id) {
	_deleteTransactionCommand.SetTransactionId(id);
	_deleteTransactionCommand.Execute();
}

void CommandsInvoker::NewAccount(AccountType type) {
	_newAccountCommand.SetAccountType(type);
	_newAccountCommand.Execute();
}

void CommandsInvoker::EditAccount(int id) {
	_editAccountCommand.SetAccountId(id);
	_editAccountCommand.Execute();
}

void CommandsInvoker::OpenTab(TabType type) {
	_openTabCommand.SetType(type);
	_openTabCommand.Execute();
}

void CommandsInvoker::OpenAccountTab(int id) {
	_openAccountTabCommand.SetAccountId(id);
	_openAccountTabCommand.Execute();
}

void CommandsInvoker::OpenAccountsTab() {
	_openAccountsTabCommand.Execute();
}

void CommandsInvoker::OpenAccountsTab(AccountType type) {
	_openAccountsTabCommand.SetAccountType(type);
	_openAccountsTabCommand.Execute();
}

void CommandsInvoker::OpenReportTab(int id) {
	_openReportTabCommand.SetReportId(id);
	_openReportTabCommand.Execute();
}

void CommandsInvoker::NewAlert() {

}

void CommandsInvoker::EditAlert(int id) {
	_editAlertCommand.SetAlertId(id);
	_editAlertCommand.Execute();
}

void CommandsInvoker::DeleteAlert(int id) {

}
void CommandsInvoker::NewScheduler() {

}

void CommandsInvoker::EditScheduler(int id) {
	_editSchedulerCommand.SetSchedulerId(id);
	_editSchedulerCommand.Execute();
}

void CommandsInvoker::DeleteScheduler(int id) {

}
