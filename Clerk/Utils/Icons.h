#pragma once

#include <wx/wx.h>

namespace Clerk {
	namespace Utils {
		class Icons {
		public:
			Icons();

			wxImageList* GetImageList();
			int GetIconForAccount(int index);

		private:
			wxImageList* _imageList;
			int _menuIconsCount;
			int _accountIconsCount;
			int _defaultAccountIcon;

			void LoadMenuIcons();
			void LoadAccountsIcons();
		};
	}
}