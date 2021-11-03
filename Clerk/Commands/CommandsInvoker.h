#pragma once

#include "QuitCommand.h"
#include "PreferencesCommand.h"
#include "AboutCommand.h"
#include "NewTransactionCommand.h"
#include "CopyTransactionCommand.h"

namespace Clerk {
	namespace Commands {
		class CommandsInvoker {
		public:
			CommandsInvoker(QuitCommand& quitCommand, PreferencesCommand& preferencesCommand, AboutCommand& aboutCommand, NewTransactionCommand& newTransactionCommand, CopyTransactionCommand& copyTransactionCommand);
			~CommandsInvoker();
			
			void OnQuit();
			void OnPreferences();
			void OnAbout();
			void OnNewTransaction(int id);
			void OnCopyTransaction(int id);

		private:
			QuitCommand& _quitCommand;
			PreferencesCommand& _preferencesCommand;
			AboutCommand& _aboutCommand;
			NewTransactionCommand& _newTransactionCommand;
			CopyTransactionCommand& _copyTransactionCommand;
		};
	}
}