#pragma once

#include <wx/wx.h>
#include "../../Data/Models/TransactionModel.h"
#include "./TagPresentationModel.h"
#include "./AccountPresentationModel.h"
#include "../../Utils/Types.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class TransactionPresentationModel {
		public:
			TransactionPresentationModel();
			TransactionPresentationModel(TransactionModel& model);

			int id;
			std::shared_ptr<AccountPresentationModel> fromAccount;
			std::shared_ptr<AccountPresentationModel> toAccount;
			float fromAmount;
			float toAmount;
			wxString note;
			wxDateTime date;
			wxDateTime created;
			shared_vector<TagPresentationModel> tags;
			wxString tagsString;			

			operator TransactionModel& ();

		private:
			TransactionModel& GetModel();
		};
	}
}