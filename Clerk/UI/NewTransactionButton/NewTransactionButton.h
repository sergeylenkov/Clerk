#pragma once

#include "../Controls/DropDownButton.h"
#include "../../Commands/CommandsInvoker.h"
#include "../../Utils/Icons.h"
#include "NewTransactionViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class NewTransactionButton : public DropDownButton
		{
		public:
			NewTransactionButton(NewTransactionViewModel& viewModel, CommandsInvoker& commandsInvoker, Icons& icons, wxWindow* parent, const wxPoint& pos, const wxSize& size);
			~NewTransactionButton();

		private:
			CommandsInvoker& _commandsInvoker;
			NewTransactionViewModel& _viewModel;
			Icons& _icons;

			void Update();
			void OnClick(wxCommandEvent& event);
			void OnMenuSelect(wxCommandEvent& event);

		};
	}
}