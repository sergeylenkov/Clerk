#pragma once

#include <wx/wx.h>
#include "../../Utils/Types.h"
#include "../Models/TransactionModel.h"
#include "TagPresentationModel.h"
#include "AccountPresentationModel.h"

namespace Clerk {
	namespace Data {
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