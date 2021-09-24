#include "CommandsInvoker.h"

using namespace Clerk::Commands;

CommandsInvoker::CommandsInvoker(QuitCommand& quitCommand, PreferencesCommand& preferencesCommand, AboutCommand& aboutCommand, NewTransactionCommand& newTransactionCommand):
	_quitCommand(quitCommand),
	_preferencesCommand(preferencesCommand),
	_aboutCommand(aboutCommand),
	_newTransactionCommand(newTransactionCommand)
{
	
}

CommandsInvoker::~CommandsInvoker() {
	delete& _quitCommand;
	delete& _preferencesCommand;
	delete& _aboutCommand;
	delete& _newTransactionCommand;
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
	_newTransactionCommand.SetTransactionId(id);
	_newTransactionCommand.SetIsSplit(false);
	_newTransactionCommand.Execute();
}