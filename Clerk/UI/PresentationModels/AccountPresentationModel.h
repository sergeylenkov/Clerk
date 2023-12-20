#pragma once

#include <wx/wx.h>
#include "../PresentationModels/CurrencyPresentationModel.h"
#include "../../Data/Models/AccountModel.h"
#include "../../Data/Enums.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace UI {
		class AccountPresentationModel {
		public:
			AccountPresentationModel();
			AccountPresentationModel(AccountModel& model);

			int id;
			wxString name;
			wxString note;
			AccountType type;
			int icon;
			int order;
			float creditLimit;
			bool isCredit = false;
			std::shared_ptr<CurrencyPresentationModel> currency;
			float balance;
			float expenses;
			float receipts;
			boolean isActive = false;
			wxDateTime date;

			operator AccountModel& ();
				
		private:
			AccountModel& GetModel();
		};
	}
}