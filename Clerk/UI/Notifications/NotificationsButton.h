#pragma once

#include <wx/wx.h>
#include "../Controls/ToolbarButton.h"
#include "../../Utils/Icons.h"
#include "NotificationsPopup.h"
#include "NotificationsViewModel.h"

using namespace Clerk::Utils;

namespace Clerk {
	namespace UI {
		class NotificationsButton : public ToolbarButton
		{
		public:
			NotificationsButton(NotificationsViewModel& viewModel, Icons& icons, wxWindow* parent, const wxPoint& pos, const wxSize& size);
			~NotificationsButton();

		protected:
			void OnPaint(wxPaintEvent& event);

		private:
			NotificationsViewModel& _viewModel;
			Icons& _icons;
			NotificationsPopup* _popup;
			wxBitmap _image;

			void Update();
			void OnClick(wxCommandEvent& event);
		};
	}
}