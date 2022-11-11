#pragma once

#include <wx/wx.h>
#include "../../Data/ViewModels/TransactionViewModel.h"
#include "../../Data/ViewModels/TransactionsMenuViewModel.h"
#include "../../Commands/CommandsInvoker.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;

namespace Clerk {
	namespace UI {
		class MainMenu : public wxMenuBar {
		public:
			MainMenu(CommandsInvoker& commandsInvoker);
			~MainMenu();

			void SetViewModel(TransactionsMenuViewModel* viewModel);

		private:
			CommandsInvoker& _commandsInvoker;
			TransactionsMenuViewModel* _viewModel{};
			wxMenu* _menuFile;

			void Update();
			void OnMenuSelect(wxCommandEvent& event);		
			void OnTransactionSelect(wxCommandEvent& event);
		};
	}
}