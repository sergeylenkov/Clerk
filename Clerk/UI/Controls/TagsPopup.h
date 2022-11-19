#pragma once
#include <wx/wx.h>
#include <wx/popupwin.h>
#include <wx/listctrl.h>
#include <vector>
#include <memory>
#include "../../Data/ViewModels/TagViewModel.h"

using namespace Clerk::Data;

class TagsPopup : public wxPopupWindow
{
public:
	TagsPopup(wxWindow *parent);
	~TagsPopup();

	void Update(std::vector<std::shared_ptr<TagViewModel>> tags);
	void SelectNext();
	void SelectPrev();
	std::shared_ptr<TagViewModel> GetSelectedTag();
	std::function<void()> OnSelectTag;

private:
	wxScrolledWindow *_panel;
	wxListCtrl *_list;
	std::vector<std::shared_ptr<TagViewModel>> _tags;

	void OnListItemDoubleClick(wxListEvent &event);
};