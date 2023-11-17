#pragma once

#include "QuitCommand.h"
#include "PreferencesCommand.h"
#include "AboutCommand.h"
#include "NewTransactionCommand.h"
#include "CopyTransactionCommand.h"
#include "SplitTransactionCommand.h"
#include "EditTransactionCommand.h"
#include "DeleteTransactionCommand.h"
#include "NewAccountCommand.h"
#include "EditAccountCommand.h"
#include "NewTabCommand.h"
#include "NewAccountTabCommand.h"
#include "NewAccountsTabCommand.h"
#include "../Data/Enums.h"
#include "../UI/Enums.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Commands {
		class CommandsInvoker {
		public:
			CommandsInvoker(QuitCommand& quitCommand, PreferencesCommand& preferencesCommand, AboutCommand& aboutCommand,
				           NewTransactionCommand& newTransactionCommand, CopyTransactionCommand& copyTransactionCommand,
						   SplitTransactionCommand& splitTransactionCommand, EditTransactionCommand& editTransactionCommand, DeleteTransactionCommand& deleteTransactionCommand,
				           NewAccountCommand& newAccountCommand, EditAccountCommand& editAccountCommand, NewTabCommand& newTabCommand, NewAccountTabCommand& newAccountTabCommand,
							NewAccountsTabCommand& newAccountsTabCommand);
			~CommandsInvoker();

			void OnQuit();
			void OnPreferences();
			void OnAbout();
			void OnNewTransaction(int id);			
			void OnCopyTransaction(int id);
			void OnSplitTransaction(int id);
			void OnEditTransaction(int id);
			void OnDeleteTransaction(int id);
			void OnNewAccount(AccountType type);
			void OnEditAccount(int id);
			void OnNewTab(TabType type);
			void OnNewAccountTab(int accountId);
			void OnNewAccountsTab(AccountType id);

		private:
			QuitCommand& _quitCommand;
			PreferencesCommand& _preferencesCommand;
			AboutCommand& _aboutCommand;
			NewTransactionCommand& _newTransactionCommand;
			CopyTransactionCommand& _copyTransactionCommand;
			SplitTransactionCommand& _splitTransactionCommand;
			EditTransactionCommand& _editTransactionCommand;
			DeleteTransactionCommand& _deleteTransactionCommand;
			NewAccountCommand& _newAccountCommand;
			EditAccountCommand& _editAccountCommand;			
			NewTabCommand& _newTabCommand;
			NewAccountTabCommand& _newAccountTabCommand;
			NewAccountsTabCommand& _newAccountsTabCommand;
		};
	}
}