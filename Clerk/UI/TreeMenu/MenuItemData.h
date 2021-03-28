#pragma once

#include <wx/treectrl.h>
#include "Enums.h"

namespace Clerk {
	namespace UI {
		class TreeMenuItemData : public wxTreeItemData {
		public:
			TreeMenuItemType type;
			std::shared_ptr<void> object;
		};
	}
}