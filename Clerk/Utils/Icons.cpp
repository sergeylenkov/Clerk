#include "Icons.h"

using namespace Clerk::Utils;

Icons::Icons() {
	wxInitAllImageHandlers();

	_imageList = new wxImageList(16, 16, false);
	_menuIconsCount = 13;
	_accountIconsCount = 63;
	_defaultAccountIcon = 27;

	LoadMenuIcons();
	LoadAccountsIcons();
}

wxImageList* Icons::GetImageList() {
	return _imageList;
}

int Icons::GetIconForAccount(int index) {
	if (_menuIconsCount + index < _imageList->GetImageCount()) {
		return _menuIconsCount + index;
	}

	return _defaultAccountIcon;
}

void Icons::LoadMenuIcons() {
	for (int i = 0; i < _menuIconsCount; i++) {
		wxString path = wxString::Format("Resources\\Menu Icons\\%d.png", i);
		wxImage image(path);

		if (image.IsOk())
		{
			wxBitmap* bitmap = new wxBitmap(image);
			_imageList->Add(*bitmap);

			delete bitmap;
		}
	}
}

void Icons::LoadAccountsIcons() {
	for (int i = 0; i < _accountIconsCount; i++) {
		wxString path = wxString::Format("Resources\\Accounts Icons\\%d.png", i);
		wxImage image(path);

		if (image.IsOk())
		{
			wxBitmap* bitmap = new wxBitmap(image);
			_imageList->Add(*bitmap);

			delete bitmap;
		}
	}
}