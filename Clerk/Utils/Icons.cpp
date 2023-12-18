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
	LoadSvgIcons();
}

Icons::~Icons() {
	for (auto p : _images) {
		delete p;
	}
	
	_images.erase(_images.begin(), _images.end());
	_imageList->Destroy();
}

int Icons::GetAccountIconsCount() {
	return _accountIconsCount;
}

wxImageList* Icons::GetImageList() {
	return _imageList;
}

int Icons::GetIconIndexForAccount(int index) {
	if (_menuIconsCount + index < _imageList->GetImageCount()) {
		return _menuIconsCount + index;
	}

	return _defaultAccountIcon;
}

wxBitmap* Icons::GetAccountIcon(int index) {
	int iconIndex = GetIconIndexForAccount(index);
	return _images[iconIndex];
}

wxBitmapBundle Icons::GetIconByType(IconType type) {
	return _icons[type];
}

void Icons::LoadMenuIcons() {
	for (int i = 0; i < _menuIconsCount; i++) {
		wxString path = wxString::Format("Resources\\Menu Icons\\%d.png", i);
		wxImage image(path);

		if (image.IsOk())
		{
			wxBitmap* bitmap = new wxBitmap(image);
			_imageList->Add(*bitmap);
			_images.push_back(bitmap);
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
			_images.push_back(bitmap);
		}
	}
}

void Icons::LoadSvgIcons() {
	_icons.clear();

	_icons[IconType::Plus] = wxBitmapBundle::FromSVGResource("PLUS_SVG", wxSize(16, 16));
	_icons[IconType::Tab] = wxBitmapBundle::FromSVGResource("TAB_SVG", wxSize(16, 16));
	_icons[IconType::Pencil] = wxBitmapBundle::FromSVGResource("PENCIL_SVG", wxSize(16, 16));
	_icons[IconType::Copy] = wxBitmapBundle::FromSVGResource("COPY_SVG", wxSize(16, 16));	
	_icons[IconType::Delete] = wxBitmapBundle::FromSVGResource("DELETE_SVG", wxSize(16, 16));
	_icons[IconType::Split] = wxBitmapBundle::FromSVGResource("SPLIT_SVG", wxSize(16, 16));
	_icons[IconType::Pause] = wxBitmapBundle::FromSVGResource("PAUSE_SVG", wxSize(16, 16));
	_icons[IconType::Play] = wxBitmapBundle::FromSVGResource("PLAY_SVG", wxSize(16, 16));
	_icons[IconType::Duplicate] = wxBitmapBundle::FromSVGResource("DUPLICATE_SVG", wxSize(16, 16));
	_icons[IconType::TabRemove] = wxBitmapBundle::FromSVGResource("TAB_REMOVE_SVG", wxSize(16, 16));
	_icons[IconType::ArrowRight] = wxBitmapBundle::FromSVGResource("ARROW_RIGHT_SVG", wxSize(16, 16));
	_icons[IconType::ArrowLeft] = wxBitmapBundle::FromSVGResource("ARROW_LEFT_SVG", wxSize(16, 16));
}