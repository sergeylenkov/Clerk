#pragma once

#include "DropDownButton.h"
#include "../../Data/ViewModels/TransactionViewModel.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class AddTransactionButton : public DropDownButton
		{
		public:
			AddTransactionButton(wxWindow* parent);

			void SetTransactions(std::vector<std::shared_ptr<TransactionViewModel>> transactions);

		private:
			void OnAddTransaction(wxCommandEvent& event);
			void OnMenuAddTransaction(wxCommandEvent& event);

		};
	}
}