#pragma once

#include <wx/wx.h>
#include "../AboutDialog.h"
#include "../PreferencesDialog.h"
#include "../Transactions/TransactionDialog.h"

namespace Clerk {
	namespace UI {
		class DialogsController {
		public:
			DialogsController(wxFrame* frame);

			void ShowAboutDialog();
			void ShowPreferencesDialog();
			void ShowTransactionDialog();

		private:
			wxFrame* _parent;
		};
	}
}
