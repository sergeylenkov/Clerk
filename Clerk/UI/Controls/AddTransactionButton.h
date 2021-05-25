#pragma once

#include "DropDownButton.h"
#include "../../Data/ViewModels/TransactionViewModel.h"
#include "../../Commands/CommandsInvoker.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;

namespace Clerk {
	namespace UI {
		class AddTransactionButton : public DropDownButton
		{
		public:
			AddTransactionButton(wxWindow* parent, CommandsInvoker& commandsInvoker);

			void SetTransactions(std::vector<std::shared_ptr<TransactionViewModel>> transactions);

		private:
			CommandsInvoker& _commandsInvoker;

			void OnAddTransaction(wxCommandEvent& event);
			void OnMenuAddTransaction(wxCommandEvent& event);

		};
	}
}