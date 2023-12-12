#pragma once

#include "QuitCommand.h"
#include "OpenPreferencesCommand.h"
#include "AboutCommand.h"
#include "NewTransactionCommand.h"
#include "CopyTransactionCommand.h"
#include "SplitTransactionCommand.h"
#include "EditTransactionCommand.h"
#include "DeleteTransactionCommand.h"
#include "NewAccountCommand.h"
#include "EditAccountCommand.h"
#include "OpenTabCommand.h"
#include "OpenAccountTabCommand.h"
#include "OpenAccountsTabCommand.h"
#include "OpenReportTabCommand.h"
#include "EditAlertCommand.h"
#include "../Data/Enums.h"
#include "../UI/Enums.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Commands {
		class CommandsInvoker {
		public:
			CommandsInvoker(QuitCommand& quitCommand, OpenPreferencesCommand& preferencesCommand, AboutCommand& aboutCommand,
				           NewTransactionCommand& newTransactionCommand, CopyTransactionCommand& copyTransactionCommand,
						   SplitTransactionCommand& splitTransactionCommand, EditTransactionCommand& editTransactionCommand, DeleteTransactionCommand& deleteTransactionCommand,
				           NewAccountCommand& newAccountCommand, EditAccountCommand& editAccountCommand, OpenTabCommand& openTabCommand, OpenAccountTabCommand& openAccountTabCommand,
						   OpenAccountsTabCommand& openAccountsTabCommand, OpenReportTabCommand& openReportTabCommand, EditAlertCommand& editAlertCommand);
			~CommandsInvoker();

			void Quit();
			void OpenPreferences();
			void OpenAbout();
			void NewTransaction(int id);			
			void CopyTransaction(int id);
			void SplitTransaction(int id);
			void EditTransaction(int id);
			void DeleteTransaction(int id);
			void NewAccount(AccountType type);
			void EditAccount(int id);
			void OpenTab(TabType type);
			void OpenAccountTab(int id);
			void OpenAccountsTab();
			void OpenAccountsTab(AccountType type);
			void OpenReportTab(int id);
			void NewAlert(int id);
			void EditAlert(int id);
			void DeleteAlert(int id);

		private:
			QuitCommand& _quitCommand;
			OpenPreferencesCommand& _preferencesCommand;
			AboutCommand& _aboutCommand;
			NewTransactionCommand& _newTransactionCommand;
			CopyTransactionCommand& _copyTransactionCommand;
			SplitTransactionCommand& _splitTransactionCommand;
			EditTransactionCommand& _editTransactionCommand;
			DeleteTransactionCommand& _deleteTransactionCommand;
			NewAccountCommand& _newAccountCommand;
			EditAccountCommand& _editAccountCommand;			
			OpenTabCommand& _openTabCommand;
			OpenAccountTabCommand& _openAccountTabCommand;
			OpenAccountsTabCommand& _openAccountsTabCommand;
			OpenReportTabCommand& _openReportTabCommand;
			EditAlertCommand& _editAlertCommand;
		};
	}
}