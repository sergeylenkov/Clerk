#pragma once
#include <wx/wx.h>
#include <wx/popupwin.h>
#include <wx/listctrl.h>
#include <vector>
#include <memory>
#include "../PresentationModels/TagPresentationModel.h"
#include "../../Utils/Types.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

class TagsPopup : public wxPopupWindow
{
public:
	TagsPopup(wxWindow *parent);

	void Position(wxPoint position, wxSize size);
	void Update(shared_vector<TagPresentationModel> tags);
	void SelectNext();
	void SelectPrev();
	std::shared_ptr<TagPresentationModel> GetSelectedTag();
	std::function<void(std::shared_ptr<TagPresentationModel>)> OnSelect;

private:
	wxScrolledWindow *_panel;
	wxListCtrl *_list;
	shared_vector<TagPresentationModel> _tags;

	void OnListItemDoubleClick(wxListEvent &event);
};