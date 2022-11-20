#pragma once
#include <wx/wx.h>
#include <wx/popupwin.h>
#include <wx/listctrl.h>
#include <vector>
#include <memory>
#include "../PresentationModels/TagPresentationModel.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

class TagsPopup : public wxPopupWindow
{
public:
	TagsPopup(wxWindow *parent);
	~TagsPopup();

	void Update(std::vector<std::shared_ptr<TagPresentationModel>> tags);
	void SelectNext();
	void SelectPrev();
	std::shared_ptr<TagPresentationModel> GetSelectedTag();
	std::function<void()> OnSelectTag;

private:
	wxScrolledWindow *_panel;
	wxListCtrl *_list;
	std::vector<std::shared_ptr<TagPresentationModel>> _tags;

	void OnListItemDoubleClick(wxListEvent &event);
};