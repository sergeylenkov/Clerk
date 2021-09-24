#pragma once

namespace Clerk {
	namespace Commands {
		class ICommandsReceiver {
		public:
			virtual void Quit() = 0;			
			virtual void OpenPreferencesDialog() = 0;
			virtual void OpenAboutDialog() = 0;
			virtual void OpenTransactionDialog(int id, bool isSplit) = 0;
		};
	}
}