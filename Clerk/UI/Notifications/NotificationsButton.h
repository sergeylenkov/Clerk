#pragma once

#include <wx/wx.h>
#include "../Controls/ToolbarButton.h"
#include "NotificationsPopup.h"

namespace Clerk {
	namespace UI {
		class NotificationsButton : public ToolbarButton
		{
		public:
			NotificationsButton(wxWindow* parent, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

		protected:
			void OnPaint(wxPaintEvent& event);

		private:
			wxBitmap _image;
			wxBitmap _imageActive;
			NotificationsPopup* _popup;

			void OnClick(wxCommandEvent& event);
		};
	}
}