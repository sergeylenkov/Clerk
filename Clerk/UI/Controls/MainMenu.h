#pragma once

#include <wx/wx.h>
#include "../../Data/ViewModels/TransactionViewModel.h"
#include "../../Commands/CommandsInvoker.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;

namespace Clerk {
	namespace UI {
		enum class MainMenuTypes {
			About = 1,
			AddTransaction = 2,
			AddAccount = 3,
			AddBudget = 4,
			AddScheduler = 5,
			Exit = 6,
			AddGoal = 7,
			Preferences = 8,
			AddAlert = 9
		};

		class MainMenu : public wxMenuBar {
		public:
			MainMenu(CommandsInvoker& commandsInvoker);

			void SetTransactions(std::vector<std::shared_ptr<TransactionViewModel>> transactions);

		private:
			wxMenu* _menuFile;
			wxMenu* _menuHelp;
			wxMenu* _menuTransaction;
			CommandsInvoker& _commandsInvoker;

			void OnQuit(wxCommandEvent& event);
			void OnAbout(wxCommandEvent& event);
			void OnPreferences(wxCommandEvent& event);
			void OnAddAccount(wxCommandEvent& event);
			void OnAddBudget(wxCommandEvent& event);
			void OnAddScheduler(wxCommandEvent& event);
			void OnAddGoal(wxCommandEvent& event);
			void OnAddAlert(wxCommandEvent& event);
			void OnAddTransaction(wxCommandEvent& event);
		};
	}
}