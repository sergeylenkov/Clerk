#pragma once

#include <wx/wx.h>
#include "../Models/TransactionModel.h"
#include "../ViewModels/TagViewModel.h"
#include "../ViewModels/AccountViewModel.h"

namespace Clerk {
	namespace Data {
		class TransactionViewModel {
		public:
			TransactionViewModel();
			TransactionViewModel(TransactionModel& transaction);

			int id;
			std::shared_ptr<AccountViewModel> fromAccount;
			std::shared_ptr<AccountViewModel> toAccount;
			float fromAmount;
			float toAmount;
			wxString note;
			wxDateTime date;
			std::vector<std::shared_ptr<TagViewModel>> tags;
			wxString tagsString;
			
			operator TransactionModel& ();

		private:
			TransactionModel* GetModel();
		};
	}
}