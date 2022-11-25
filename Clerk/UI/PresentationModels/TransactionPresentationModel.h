#pragma once

#include <wx/wx.h>
#include "../../Data/Models/TransactionModel.h"
#include "./TagPresentationModel.h"
#include "./AccountPresentationModel.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class TransactionPresentationModel {
		public:
			TransactionPresentationModel();
			TransactionPresentationModel(TransactionModel& transaction);

			int id;
			std::shared_ptr<AccountPresentationModel> fromAccount;
			std::shared_ptr<AccountPresentationModel> toAccount;
			float fromAmount;
			float toAmount;
			wxString note;
			wxDateTime date;
			wxDateTime created;
			std::vector<std::shared_ptr<TagPresentationModel>> tags;
			wxString tagsString;			

			std::shared_ptr<TransactionModel> GetModel();
		};
	}
}