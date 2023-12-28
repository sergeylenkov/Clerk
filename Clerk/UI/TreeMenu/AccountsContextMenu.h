#pragma once

#include <wx/wx.h>
#include "../../Data/PresentationModels/TransactionPresentationModel.h"
#include "../../Data/PresentationModels/AccountPresentationModel.h"
#include "Enums.h"
#include "TreeContextMenu.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;
using namespace Clerk::UI;

namespace Clerk {
	namespace UI {
		class AccountsContextMenu : public TreeContextMenu {
		public:
			AccountsContextMenu(DataContext& context, Icons& icons, TreeMenuItemType type);

		private:
			TreeMenuItemType _type;

			void OnMenuSelect(wxCommandEvent& event);
			AccountType GetAccountTypeByMenuType(TreeMenuItemType type);
		};
	}
}