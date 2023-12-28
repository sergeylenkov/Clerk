#pragma once

#include "../../Data/DataContext.h"
#include "../../Utils/Icons.h"
#include "../Controls/DropDownButton.h"
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
			NewTransactionButton(NewTransactionViewModel& viewModel, DataContext& context, Icons& icons, wxWindow* parent, const wxPoint& pos, const wxSize& size);
			~NewTransactionButton();

		private:
			DataContext& _context;
			NewTransactionViewModel& _viewModel;
			Icons& _icons;

			void Update();
			void OnClick(wxCommandEvent& event);
			void OnMenuSelect(wxCommandEvent& event);

		};
	}
}