#include "CommandsInvoker.h"

using namespace Clerk::Commands;

CommandsInvoker::CommandsInvoker(
	QuitCommand& quitCommand,
	OpenPreferencesCommand& preferencesCommand, 
	AboutCommand& aboutCommand,
	OpenTabCommand& openTabCommand,
	OpenAccountTabCommand& openAccountTabCommand,
	OpenAccountsTabCommand& openAccountsTabCommand,
	OpenReportTabCommand& openReportTabCommand,
	NewTransactionCommand& newTransactionCommand, 
	CopyTransactionCommand& copyTransactionCommand,
	SplitTransactionCommand& splitTransactionCommand,
	EditTransactionCommand& editTransactionCommand,
	DeleteTransactionCommand& deleteTransactionCommand,
	DuplicateTransactionCommand& duplicateTransactionCommand,
	NewAccountCommand& newAccountCommand,	
	EditAccountCommand& editAccountCommand,	
	NewAlertCommand& newAlertCommand,
	EditAlertCommand& editAlertCommand,
	DeleteAlertCommand& deleteAlertCommand,
	NewBudgetCommand& newBudgetCommand,
	EditBudgetCommand& editBudgetCommand,
	DeleteBudgetCommand& deleteBudgetCommand,
	NewSchedulerCommand& newSchedulerCommand,
	EditSchedulerCommand& editSchedulerCommand,
	DeleteSchedulerCommand& deleteSchedulerCommand,
	RunSchedulerCommand& runSchedulerCommand,
	PauseSchedulerCommand& pauseSchedulerCommand
):
	_quitCommand(quitCommand),
	_preferencesCommand(preferencesCommand),
	_aboutCommand(aboutCommand),
	_openTabCommand(openTabCommand),
	_openAccountTabCommand(openAccountTabCommand),
	_openAccountsTabCommand(openAccountsTabCommand),
	_openReportTabCommand(openReportTabCommand),
	_newTransactionCommand(newTransactionCommand),
	_copyTransactionCommand(copyTransactionCommand),
	_splitTransactionCommand(splitTransactionCommand),
	_editTransactionCommand(editTransactionCommand),
	_deleteTransactionCommand(deleteTransactionCommand),
	_duplicateTransactionCommand(duplicateTransactionCommand),
	_newAccountCommand(newAccountCommand),
	_editAccountCommand(editAccountCommand),	
	_newAlertCommand(newAlertCommand),
	_editAlertCommand(editAlertCommand),
	_deleteAlertCommand(deleteAlertCommand),
	_newBudgetCommand(newBudgetCommand),
	_editBudgetCommand(editBudgetCommand),
	_deleteBudgetCommand(deleteBudgetCommand),
	_newSchedulerCommand(newSchedulerCommand),
	_editSchedulerCommand(editSchedulerCommand),
	_deleteSchedulerCommand(deleteSchedulerCommand),
	_runSchedulerCommand(runSchedulerCommand),
	_pauseSchedulerCommand(pauseSchedulerCommand)
{
	
}

CommandsInvoker::~CommandsInvoker() {
	delete& _quitCommand;
	delete& _preferencesCommand;
	delete& _aboutCommand;
	delete& _openTabCommand;
	delete& _openAccountTabCommand;
	delete& _openAccountsTabCommand;
	delete& _openReportTabCommand;
	delete& _newTransactionCommand;
	delete& _copyTransactionCommand;
	delete& _splitTransactionCommand;
	delete& _editTransactionCommand;
	delete& _deleteTransactionCommand;
	delete& _duplicateTransactionCommand;
	delete& _newAccountCommand;
	delete& _editAccountCommand;	
	delete& _newAlertCommand;
	delete& _editAlertCommand;
	delete& _deleteAlertCommand;
	delete& _newBudgetCommand;
	delete& _editBudgetCommand;
	delete& _deleteBudgetCommand;
	delete& _newSchedulerCommand;
	delete& _editSchedulerCommand;
	delete& _deleteSchedulerCommand;
	delete& _runSchedulerCommand;
	delete& _pauseSchedulerCommand;
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

void CommandsInvoker::DuplicateTransaction(int id) {
	_duplicateTransactionCommand.SetTransactionId(id);
	_duplicateTransactionCommand.Execute();
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

void CommandsInvoker::DuplicateTransactio(int id) {
	_duplicateTransactionCommand.SetTransactionId(id);
	_duplicateTransactionCommand.Execute();
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
	_newAlertCommand.Execute();
}

void CommandsInvoker::EditAlert(int id) {
	_editAlertCommand.SetAlertId(id);
	_editAlertCommand.Execute();
}

void CommandsInvoker::DeleteAlert(int id) {
	_deleteAlertCommand.SetAlertId(id);
	_deleteAlertCommand.Execute();
}

void CommandsInvoker::NewBudget() {
	_newBudgetCommand.Execute();
}

void CommandsInvoker::EditBudget(int id) {
	_editBudgetCommand.SetBudgetId(id);
	_editBudgetCommand.Execute();
}

void CommandsInvoker::DeleteBudget(int id) {
	_deleteBudgetCommand.SetBudgetId(id);
	_deleteBudgetCommand.Execute();
}

void CommandsInvoker::NewScheduler() {
	_newSchedulerCommand.Execute();
}

void CommandsInvoker::EditScheduler(int id) {
	_editSchedulerCommand.SetSchedulerId(id);
	_editSchedulerCommand.Execute();
}

void CommandsInvoker::DeleteScheduler(int id) {
	_deleteSchedulerCommand.SetSchedulerId(id);
	_deleteSchedulerCommand.Execute();
}

void CommandsInvoker::RunScheduler(int id) {
	_runSchedulerCommand.SetSchedulerId(id);
	_runSchedulerCommand.Execute();
}

void CommandsInvoker::PauseScheduler(int id) {
	_pauseSchedulerCommand.SetSchedulerId(id);
	_pauseSchedulerCommand.Execute();
}