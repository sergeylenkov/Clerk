#pragma once

#include <wx/wx.h>

namespace Clerk {
	namespace Utils {
		class Icons {
		public:
			Icons();
			~Icons();

			wxImageList* GetImageList();
			int GetIconForAccount(int index);
			wxBitmap* GetBitmapForIcon(int index);

		private:
			wxImageList* _imageList;
			int _menuIconsCount;
			int _accountIconsCount;
			int _defaultAccountIcon;
			std::vector<wxBitmap *> _images;

			void LoadMenuIcons();
			void LoadAccountsIcons();
		};
	}
}