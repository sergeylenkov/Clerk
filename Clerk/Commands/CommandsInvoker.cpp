#include "CommandsInvoker.h"

using namespace Clerk::Commands;

CommandsInvoker::CommandsInvoker(QuitCommand& quitCommand, PreferencesCommand& preferencesCommand, AboutCommand& aboutCommand, NewTransactionCommand& newTransactionCommand,
	CopyTransactionCommand& copyTransactionCommand, EditAccountCommand& editAccountCommand) :
	_quitCommand(quitCommand),
	_preferencesCommand(preferencesCommand),
	_aboutCommand(aboutCommand),
	_newTransactionCommand(newTransactionCommand),
	_copyTransactionCommand(copyTransactionCommand),
	_editAccountCommand(editAccountCommand)
{
	
}

CommandsInvoker::~CommandsInvoker() {
	delete& _quitCommand;
	delete& _preferencesCommand;
	delete& _aboutCommand;
	delete& _newTransactionCommand;
	delete& _copyTransactionCommand;
	delete& _editAccountCommand;
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

void CommandsInvoker::OnEditAccount(int id) {
	_editAccountCommand.SetAccountId(id);
	_editAccountCommand.Execute();
}