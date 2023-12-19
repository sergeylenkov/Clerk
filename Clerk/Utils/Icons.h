#pragma once

#include <wx/wx.h>
#include <vector>
#include <map>

namespace Clerk {
	namespace Utils {
		enum class IconType {
			Plus,
			Tab,
			TabRemove,
			Pencil,
			Copy,
			Delete,
			Split,
			Pause,
			Play,
			Duplicate,			
			ArrowRight,
			ArrowLeft,
			ArrowDown,
			ArrowUp,
			Wallet,
			CalendarMonth,
			Cached,
			Bell,
			BellActive,
			Merge,
			Undo
		};

		class Icons {
		public:
			Icons();
			~Icons();

			wxImageList* GetImageList();
			int GetIconIndexForAccount(int index);
			int GetAccountIconsCount();
			wxBitmap* GetAccountIcon(int index);
			wxBitmapBundle GetIconByType(IconType type);

		private:
			wxImageList* _imageList;
			int _menuIconsCount;
			int _accountIconsCount;
			int _defaultAccountIcon;
			std::vector<wxBitmap *> _images;
			std::map<IconType, wxBitmapBundle> _icons;

			void LoadMenuIcons();
			void LoadAccountsIcons();
			void LoadSvgIcons();
		};
	}
}