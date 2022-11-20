#pragma once

#include <wx/wx.h>
#include "../PresentationModels/TransactionPresentationModel.h"
#include "../ViewModels/TransactionsMenuViewModel.h"
#include "../../Commands/CommandsInvoker.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;

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