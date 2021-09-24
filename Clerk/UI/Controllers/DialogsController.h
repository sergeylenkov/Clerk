#pragma once

#include <wx/wx.h>
#include "../AboutDialog.h"
#include "../PreferencesDialog.h"
#include "../Transactions/TransactionDialog.h"
#include "../../Data/ViewModels/TransactionEditViewModel.h"
#include "../../Data/DataContext.h"
#include "../../Utils/Icons.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class DialogsController {
		public:
			DialogsController(wxFrame* frame, DataContext& context, Icons& icons);

			void ShowAboutDialog();
			void ShowPreferencesDialog();
			void ShowTransactionDialog(int id, bool isSplit);

		private:
			wxFrame* _parent;
			DataContext& _context;
			Icons& _icons;
		};
	}
}
