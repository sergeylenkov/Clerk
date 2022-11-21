#pragma once

#include <wx/wx.h>

namespace Clerk {
	namespace Utils {
		class Icons {
		public:
			Icons();
			~Icons();

			wxImageList* GetImageList();
			unsigned int GetIconIndexForAccount(unsigned int index);
			unsigned int GetAccountIconsCount();
			wxBitmap* GetAccountIcon(unsigned int index);

		private:
			wxImageList* _imageList;
			unsigned int _menuIconsCount;
			unsigned int _accountIconsCount;
			unsigned int _defaultAccountIcon;
			std::vector<wxBitmap *> _images;

			void LoadMenuIcons();
			void LoadAccountsIcons();
		};
	}
}