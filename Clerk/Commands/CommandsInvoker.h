#pragma once

#include "QuitCommand.h"
#include "PreferencesCommand.h"
#include "AboutCommand.h"
#include "AddTransactionCommand.h"

namespace Clerk {
	namespace Commands {
		class CommandsInvoker {
		public:
			CommandsInvoker(QuitCommand& quitCommand, PreferencesCommand& preferencesCommand, AboutCommand& aboutCommand, AddTransactionCommand& addTransactionCommand);
			~CommandsInvoker();

			void OnQuit();
			void OnPreferences();
			void OnAbout();
			void OnAddTransaction();

		private:
			QuitCommand& _quitCommand;
			PreferencesCommand& _preferencesCommand;
			AboutCommand& _aboutCommand;
			AddTransactionCommand& _addTransactionCommand;
		};
	}
}