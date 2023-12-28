#pragma once

#include <wx/wx.h>
#include "../../Data/DataContext.h"
#include "../../Utils/Icons.h"

using namespace Clerk::Commands;
using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class TreeContextMenu : public wxMenu {
		public:
			TreeContextMenu(DataContext& context, Icons& icons);
			
		protected:
			DataContext& _context;
			Icons& _icons;
		};
	}
}