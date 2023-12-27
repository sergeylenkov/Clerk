#pragma once

#include <wx/wx.h>
#include "../../Commands/CommandsInvoker.h"
#include "../../Utils/Icons.h"
#include "Enums.h"
#include "MainMenuViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class MainMenu : public wxMenuBar {
		public:
			MainMenu(MainMenuViewModel& viewModel, CommandsInvoker& commandsInvoker, Icons& icons);
			~MainMenu();

		private:
			MainMenuViewModel& _viewModel;
			CommandsInvoker& _commandsInvoker;
			Icons& _icons;			
			wxMenu* _menuFile;

			void Update();
			void OnMenuSelect(wxCommandEvent& event);		
			void OnTransactionSelect(wxCommandEvent& event);
		};
	}
}