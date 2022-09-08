#pragma once

#include <wx/wx.h>
#include "../TabsPanel.h"
#include "../../Data/DataContext.h"
#include "../../Utils/Icons.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class TabsController {
		public:
			TabsController(DataContext& context, Icons& icons);

			void SetTabsPanel(TabsPanel* panel);
			void OpenNewTab(TabType type);

		private:
			TabsPanel* _panel;
			DataContext& _context;
			Icons& _icons;
		};
	}
}
