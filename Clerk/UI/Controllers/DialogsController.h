#pragma once

#include <wx/wx.h>
#include "../AboutDialog.h"
#include "../PreferencesDialog.h"

namespace Clerk {
	namespace UI {
		class DialogsController {
		public:
			DialogsController(wxFrame* frame);

			void ShowAboutDialog();
			void ShowPreferencesDialog();

		private:
			wxFrame* _parent;
		};
	}
}
