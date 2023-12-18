#pragma once

#include <wx/wx.h>
#include "../../Utils/Icons.h"
#include "Enums.h"

using namespace Clerk::UI;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class TabsContextMenu : public wxMenu {
		public:
			TabsContextMenu(Icons& icons, int tabId, bool isLeftEnabled, bool isRightEnabled, bool isCloseEnabled);

			std::function<void(int tabId)> OnLeft;
			std::function<void(int tabId)> OnRight;
			std::function<void(int tabId)> OnClose;

		private:
			Icons& _icons;
			int _tabId = -1;
			bool _isLeftEnabled = true;
			bool _isRightEnabled = true;
			bool _isCloseEnabled = true;

			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}