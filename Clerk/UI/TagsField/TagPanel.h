#pragma once

#include "wx/wx.h"
#include "../PresentationModels/TagPresentationModel.h"
#include "../../Utils/Types.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

class TagPanel : public wxPanel
{
public:
	TagPanel(wxWindow* parent, const wxPoint& position = wxDefaultPosition, const wxSize& size = wxDefaultSize);

	void SetTag(std::shared_ptr<TagPresentationModel> tag);
	std::function<void(std::shared_ptr<TagPresentationModel>)> OnDelete;

private:
	std::shared_ptr<TagPresentationModel> _tag;

	wxBoxSizer* _mainSizer;
	wxStaticText* _label;

	void OnDeleteClick(wxCommandEvent& event);
};
