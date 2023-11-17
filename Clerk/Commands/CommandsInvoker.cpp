#include "CommandsInvoker.h"

using namespace Clerk::Commands;

CommandsInvoker::CommandsInvoker(QuitCommand& quitCommand, PreferencesCommand& preferencesCommand, AboutCommand& aboutCommand,
	NewTransactionCommand& newTransactionCommand, CopyTransactionCommand& copyTransactionCommand,
	SplitTransactionCommand& splitTransactionCommand, EditTransactionCommand& editTransactionCommand, DeleteTransactionCommand& deleteTransactionCommand,
	NewAccountCommand& newAccountCommand, EditAccountCommand& editAccountCommand, NewTabCommand& newTabCommand, NewAccountTabCommand& newAccountTabCommand,
	NewAccountsTabCommand& newAccountsTabCommand) :
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
	_newTabCommand(newTabCommand),
	_newAccountTabCommand(newAccountTabCommand),
	_newAccountsTabCommand(newAccountsTabCommand)
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
	delete& _newTabCommand;
	delete& _newAccountTabCommand;
	delete& _newAccountsTabCommand;
}

void CommandsInvoker::OnQuit() {
	_quitCommand.Execute();
}

void CommandsInvoker::OnPreferences() {
	_preferencesCommand.Execute();
}

void CommandsInvoker::OnAbout() {
	_aboutCommand.Execute();
}

void CommandsInvoker::OnNewTransaction(int id) {
	_newTransactionCommand.SetAccountId(id);	
	_newTransactionCommand.Execute();
}

void CommandsInvoker::OnCopyTransaction(int id) {
	_copyTransactionCommand.SetTransactionId(id);
	_copyTransactionCommand.Execute();
}

void CommandsInvoker::OnSplitTransaction(int id) {
	_splitTransactionCommand.SetTransactionId(id);
	_splitTransactionCommand.Execute();
}

void CommandsInvoker::OnEditTransaction(int id) {
	_editTransactionCommand.SetTransactionId(id);
	_editTransactionCommand.Execute();
}

void CommandsInvoker::OnDeleteTransaction(int id) {
	_deleteTransactionCommand.SetTransactionId(id);
	_deleteTransactionCommand.Execute();
}

void CommandsInvoker::OnNewAccount(AccountType type) {
	_newAccountCommand.SetAccountType(type);
	_newAccountCommand.Execute();
}

void CommandsInvoker::OnEditAccount(int id) {
	_editAccountCommand.SetAccountId(id);
	_editAccountCommand.Execute();
}

void CommandsInvoker::OnNewTab(TabType type) {
	_newTabCommand.SetType(type);
	_newTabCommand.Execute();
}

void CommandsInvoker::OnNewAccountTab(int accountId) {
	_newAccountTabCommand.SetAccountId(accountId);
	_newAccountTabCommand.Execute();
}

void CommandsInvoker::OnNewAccountsTab(AccountType type) {
	_newAccountsTabCommand.SetAccountType(type);
	_newAccountsTabCommand.Execute();
}