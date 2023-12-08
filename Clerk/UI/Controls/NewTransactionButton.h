#pragma once

#include "DropDownButton.h"
#include "../PresentationModels/TransactionPresentationModel.h"
#include "../ViewModels/TransactionsMenuViewModel.h"
#include "../../Commands/CommandsInvoker.h"
#include "../../Utils/Icons.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class NewTransactionButton : public DropDownButton
		{
		public:
			NewTransactionButton(TransactionsMenuViewModel& viewModel, CommandsInvoker& commandsInvoker, Icons& icons, wxWindow* parent, const wxPoint& pos, const wxSize& size);
			~NewTransactionButton();

		private:
			CommandsInvoker& _commandsInvoker;
			TransactionsMenuViewModel& _viewModel;
			Icons& _icons;

			void Update();
			void OnClick(wxCommandEvent& event);
			void OnMenuSelect(wxCommandEvent& event);

		};
	}
}