#pragma once

#include "QuitCommand.h"
#include "PreferencesCommand.h"
#include "AboutCommand.h"
#include "NewTransactionCommand.h"
#include "CopyTransactionCommand.h"
#include "EditAccountCommand.h"

namespace Clerk {
	namespace Commands {
		class CommandsInvoker {
		public:
			CommandsInvoker(QuitCommand& quitCommand, PreferencesCommand& preferencesCommand, AboutCommand& aboutCommand, NewTransactionCommand& newTransactionCommand,
				            CopyTransactionCommand& copyTransactionCommand, EditAccountCommand& editAccountCommand);
			~CommandsInvoker();
			
			void OnQuit();
			void OnPreferences();
			void OnAbout();
			void OnNewTransaction(int id);
			void OnCopyTransaction(int id);
			void OnEditAccount(int id);

		private:
			QuitCommand& _quitCommand;
			PreferencesCommand& _preferencesCommand;
			AboutCommand& _aboutCommand;
			NewTransactionCommand& _newTransactionCommand;
			CopyTransactionCommand& _copyTransactionCommand;
			EditAccountCommand& _editAccountCommand;
		};
	}
}