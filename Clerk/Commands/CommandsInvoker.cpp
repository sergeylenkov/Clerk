#include "CommandsInvoker.h"

using namespace Clerk::Commands;

CommandsInvoker::CommandsInvoker(QuitCommand& quitCommand, OpenPreferencesCommand& preferencesCommand, AboutCommand& aboutCommand,
	NewTransactionCommand& newTransactionCommand, CopyTransactionCommand& copyTransactionCommand,
	SplitTransactionCommand& splitTransactionCommand, EditTransactionCommand& editTransactionCommand, DeleteTransactionCommand& deleteTransactionCommand,
	NewAccountCommand& newAccountCommand, EditAccountCommand& editAccountCommand, OpenTabCommand& newTabCommand, OpenAccountTabCommand& newAccountTabCommand,
	OpenAccountsTabCommand& newAccountsTabCommand, OpenReportTabCommand& openReportTabCommand) :
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
	_openTabCommand(newTabCommand),
	_openAccountTabCommand(newAccountTabCommand),
	_openAccountsTabCommand(newAccountsTabCommand),
	_openReportTabCommand(openReportTabCommand)
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

void CommandsInvoker::NewTransaction(int id) {
	_newTransactionCommand.SetAccountId(id);	
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

void CommandsInvoker::OpenAccountTab(int accountId) {
	_openAccountTabCommand.SetAccountId(accountId);
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