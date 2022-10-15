#pragma once

#include <wx/wx.h>
#include "Enums.h"

using namespace Clerk::UI;

namespace Clerk {
	namespace UI {
		class TabsContextMenu : public wxMenu {
		public:
			TabsContextMenu(int tabId, bool isLeftEnabled, bool isRightEnabled, bool isCloseEnabled);

			std::function<void(int tabId)> OnLeft;
			std::function<void(int tabId)> OnRight;
			std::function<void(int tabId)> OnClose;

		private:
			int _tabId = -1;
			bool _isLeftEnabled = true;
			bool _isRightEnabled = true;
			bool _isCloseEnabled = true;

			void OnMenuSelect(wxCommandEvent& event);
		};
	}
}