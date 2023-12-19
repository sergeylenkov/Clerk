#pragma once

#include "QuitCommand.h"
#include "OpenPreferencesCommand.h"
#include "AboutCommand.h"
#include "Transaction/NewTransactionCommand.h"
#include "Transaction/CopyTransactionCommand.h"
#include "Transaction/SplitTransactionCommand.h"
#include "Transaction/EditTransactionCommand.h"
#include "Transaction/DeleteTransactionCommand.h"
#include "Transaction/DuplicateTransactionCommand.h"
#include "Account/NewAccountCommand.h"
#include "Account/EditAccountCommand.h"
#include "Alert/EditAlertCommand.h"
#include "Alert/DeleteAlertCommand.h"
#include "Tabs/OpenTabCommand.h"
#include "Tabs/OpenAccountTabCommand.h"
#include "Tabs/OpenAccountsTabCommand.h"
#include "Tabs/OpenReportTabCommand.h"
#include "Scheduler/EditSchedulerCommand.h"
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
				NewTransactionCommand& newTransactionCommand,
				CopyTransactionCommand& copyTransactionCommand,
				SplitTransactionCommand& splitTransactionCommand,
				EditTransactionCommand& editTransactionCommand,
				DeleteTransactionCommand& deleteTransactionCommand,
				DuplicateTransactionCommand& duplicateTransactionCommand,
				NewAccountCommand& newAccountCommand,
				EditAccountCommand& editAccountCommand,
				OpenTabCommand& openTabCommand,
				OpenAccountTabCommand& openAccountTabCommand,
				OpenAccountsTabCommand& openAccountsTabCommand,
				OpenReportTabCommand& openReportTabCommand,
				EditAlertCommand& editAlertCommand,
				DeleteAlertCommand& deleteAlertCommand,
				EditSchedulerCommand& editSchedulerCommand
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
			void NewScheduler();
			void EditScheduler(int id);
			void DeleteScheduler(int id);

		private:
			QuitCommand& _quitCommand;
			OpenPreferencesCommand& _preferencesCommand;
			AboutCommand& _aboutCommand;
			NewTransactionCommand& _newTransactionCommand;
			CopyTransactionCommand& _copyTransactionCommand;
			SplitTransactionCommand& _splitTransactionCommand;
			EditTransactionCommand& _editTransactionCommand;
			DeleteTransactionCommand& _deleteTransactionCommand;
			DuplicateTransactionCommand& _duplicateTransactionCommand;
			NewAccountCommand& _newAccountCommand;
			EditAccountCommand& _editAccountCommand;			
			OpenTabCommand& _openTabCommand;
			OpenAccountTabCommand& _openAccountTabCommand;
			OpenAccountsTabCommand& _openAccountsTabCommand;
			OpenReportTabCommand& _openReportTabCommand;
			EditAlertCommand& _editAlertCommand;
			DeleteAlertCommand& _deleteAlertCommand;
			EditSchedulerCommand& _editSchedulerCommand;
		};
	}
}