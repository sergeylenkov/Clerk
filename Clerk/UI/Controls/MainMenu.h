#pragma once

#include <wx/wx.h>
#include "../PresentationModels/TransactionPresentationModel.h"
#include "../ViewModels/TransactionsMenuViewModel.h"
#include "../../Commands/CommandsInvoker.h"
#include "../../Utils/Icons.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class MainMenu : public wxMenuBar {
		public:
			MainMenu(TransactionsMenuViewModel& viewModel, CommandsInvoker& commandsInvoker, Icons& icons);
			~MainMenu();

		private:
			TransactionsMenuViewModel& _viewModel;
			CommandsInvoker& _commandsInvoker;
			Icons& _icons;			
			wxMenu* _menuFile;

			void Update();
			void OnMenuSelect(wxCommandEvent& event);		
			void OnTransactionSelect(wxCommandEvent& event);
		};
	}
}