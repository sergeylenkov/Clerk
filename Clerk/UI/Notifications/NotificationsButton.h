#pragma once

#include <wx/wx.h>
#include "../ViewModels/NotificationsViewModel.h"
#include "../Controls/ToolbarButton.h"
#include "NotificationsPopup.h"

namespace Clerk {
	namespace UI {
		class NotificationsButton : public ToolbarButton
		{
		public:
			NotificationsButton(NotificationsViewModel& viewModel, wxWindow* parent, const wxPoint& pos, const wxSize& size);
			~NotificationsButton();

		protected:
			void OnPaint(wxPaintEvent& event);

		private:
			NotificationsViewModel& _viewModel;
			wxBitmap _image;
			NotificationsPopup* _popup;

			void Update();
			void OnClick(wxCommandEvent& event);
		};
	}
}