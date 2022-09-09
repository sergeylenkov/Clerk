#pragma once

#include "QuitCommand.h"
#include "PreferencesCommand.h"
#include "AboutCommand.h"
#include "NewTransactionCommand.h"
#include "CopyTransactionCommand.h"
#include "NewAccountCommand.h"
#include "EditAccountCommand.h"
#include "NewTabCommand.h"
#include "../Data/Enums.h"
#include "../UI/Enums.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Commands {
		class CommandsInvoker {
		public:
			CommandsInvoker(QuitCommand& quitCommand, PreferencesCommand& preferencesCommand, AboutCommand& aboutCommand, NewTransactionCommand& newTransactionCommand,
				            CopyTransactionCommand& copyTransactionCommand, NewAccountCommand& newAccountCommand, EditAccountCommand& editAccountCommand,
							NewTabCommand& newTabCommand);
			~CommandsInvoker();

			void OnQuit();
			void OnPreferences();
			void OnAbout();
			void OnNewTransaction(int id);
			void OnCopyTransaction(int id);
			void OnNewAccount(AccountType type);
			void OnEditAccount(int id);
			void OnNewTab(TabType id);
			
		private:
			QuitCommand& _quitCommand;
			PreferencesCommand& _preferencesCommand;
			AboutCommand& _aboutCommand;
			NewTransactionCommand& _newTransactionCommand;
			CopyTransactionCommand& _copyTransactionCommand;
			NewAccountCommand& _newAccountCommand;
			EditAccountCommand& _editAccountCommand;			
			NewTabCommand& _newTabCommand;
		};
	}
}