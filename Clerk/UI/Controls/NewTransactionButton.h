#pragma once

#include "DropDownButton.h"
#include "../../Data/ViewModels/TransactionViewModel.h"
#include "../../Data/ViewModels/TransactionsMenuViewModel.h"
#include "../../Commands/CommandsInvoker.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;

namespace Clerk {
	namespace UI {
		class NewTransactionButton : public DropDownButton
		{
		public:
			NewTransactionButton(wxWindow* parent, CommandsInvoker& commandsInvoker);
			~NewTransactionButton();

			void SetViewModel(TransactionsMenuViewModel* viewModel);

		private:
			CommandsInvoker& _commandsInvoker;
			TransactionsMenuViewModel* _viewModel{};

			void Update();
			void OnAddTransaction(wxCommandEvent& event);
			void OnMenuSelect(wxCommandEvent& event);

		};
	}
}