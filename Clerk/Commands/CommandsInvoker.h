#pragma once

#include "QuitCommand.h"
#include "OpenPreferencesCommand.h"
#include "AboutCommand.h"
#include "Tabs/OpenTabCommand.h"
#include "Tabs/OpenAccountTabCommand.h"
#include "Tabs/OpenAccountsTabCommand.h"
#include "Tabs/OpenReportTabCommand.h"
#include "Transaction/NewTransactionCommand.h"
#include "Transaction/CopyTransactionCommand.h"
#include "Transaction/SplitTransactionCommand.h"
#include "Transaction/EditTransactionCommand.h"
#include "Transaction/DeleteTransactionCommand.h"
#include "Transaction/DuplicateTransactionCommand.h"
#include "Account/NewAccountCommand.h"
#include "Account/EditAccountCommand.h"
#include "Alert//NewAlertCommand.h"
#include "Alert/EditAlertCommand.h"
#include "Alert/DeleteAlertCommand.h"
#include "Budget/NewBudgetCommand.h"
#include "Budget/EditBudgetCommand.h"
#include "Budget/DeleteBudgetCommand.h"
#include "Scheduler/NewSchedulerCommand.h"
#include "Scheduler/EditSchedulerCommand.h"
#include "Scheduler/DeleteSchedulerCommand.h"
#include "Scheduler/RunSchedulerCommand.h"
#include "Scheduler/PauseSchedulerCommand.h"
#include "../Data/Enums.h"
#include "../UI/Tabs/Enums.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Commands {
		class CommandsInvoker {
		public:
			CommandsInvoker(
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
			);
			~CommandsInvoker();

			void Quit();
			void OpenPreferences();
			void OpenAbout();
			void NewTransaction(int accountId);			
			void CopyTransaction(int id);
			void DuplicateTransaction(int id);
			void SplitTransaction(int id);
			void EditTransaction(int id);
			void DeleteTransaction(int id);
			void DuplicateTransactio(int id);
			void NewAccount(AccountType type);
			void EditAccount(int id);
			void OpenTab(TabType type);
			void OpenAccountTab(int id);
			void OpenAccountsTab();
			void OpenAccountsTab(AccountType type);
			void OpenReportTab(int id);
			void NewAlert();
			void EditAlert(int id);
			void DeleteAlert(int id);			
			void NewBudget();
			void EditBudget(int id);
			void DeleteBudget(int id);
			void NewScheduler();
			void EditScheduler(int id);
			void DeleteScheduler(int id);
			void RunScheduler(int id);
			void PauseScheduler(int id);

		private:
			QuitCommand& _quitCommand;
			OpenPreferencesCommand& _preferencesCommand;
			AboutCommand& _aboutCommand;
			OpenTabCommand& _openTabCommand;
			OpenAccountTabCommand& _openAccountTabCommand;
			OpenAccountsTabCommand& _openAccountsTabCommand;
			OpenReportTabCommand& _openReportTabCommand;
			NewTransactionCommand& _newTransactionCommand;
			CopyTransactionCommand& _copyTransactionCommand;
			SplitTransactionCommand& _splitTransactionCommand;
			EditTransactionCommand& _editTransactionCommand;
			DeleteTransactionCommand& _deleteTransactionCommand;
			DuplicateTransactionCommand& _duplicateTransactionCommand;
			NewAccountCommand& _newAccountCommand;
			EditAccountCommand& _editAccountCommand;						
			NewAlertCommand& _newAlertCommand;
			EditAlertCommand& _editAlertCommand;
			DeleteAlertCommand& _deleteAlertCommand;
			NewBudgetCommand& _newBudgetCommand;
			EditBudgetCommand& _editBudgetCommand;
			DeleteBudgetCommand& _deleteBudgetCommand;
			NewSchedulerCommand& _newSchedulerCommand;
			EditSchedulerCommand& _editSchedulerCommand;
			DeleteSchedulerCommand& _deleteSchedulerCommand;
			RunSchedulerCommand& _runSchedulerCommand;
			PauseSchedulerCommand& _pauseSchedulerCommand;
		};
	}
}