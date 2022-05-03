#pragma once

namespace Clerk {
	namespace Commands {
		class ICommandsReceiver {
		public:
			virtual void Quit() = 0;			
			virtual void OpenPreferencesDialog() = 0;
			virtual void OpenAboutDialog() = 0;
			virtual void OpenNewTransactionDialog(int id) = 0;
			virtual void OpenCopyTransactionDialog(int id) = 0;
			virtual void OpenEditAccountDialog(int id) = 0;
		};
	}
}