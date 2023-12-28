#pragma once

#include <wx/wx.h>
#include "../../Data/PresentationModels/TransactionPresentationModel.h"
#include "../../Data/PresentationModels/AccountPresentationModel.h"
#include "TreeContextMenu.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class AccountContextMenu : public TreeContextMenu {
		public:
			AccountContextMenu(DataContext& context, Icons& icons, AccountPresentationModel& account, std::vector<std::shared_ptr<TransactionPresentationModel>> transactions);

		private:
			AccountPresentationModel& _account;
			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}