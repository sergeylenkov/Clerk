#pragma once

#include "wx/wx.h"
#include "../../Data/Services/TagsService.h"
#include "../PresentationModels/TagPresentationModel.h"
#include "../../Utils/Types.h"
#include "TagsPopup.h"
#include "TagPanel.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

class TagsField : public wxPanel
{
public:
	TagsField(wxWindow* parent, TagsService& tagsService, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	
	void SetTags(shared_vector<TagPresentationModel> tags);
	shared_vector<TagPresentationModel> GetTags();
	std::function<void()> OnChange;

private:
	TagsService& _tagsService;
	shared_vector<TagPresentationModel> _tags;

	wxBoxSizer* _tagsSizer;
	wxTextCtrl* _textField;
	TagsPopup* _popup;

	void Update();
	void AddTag(std::shared_ptr<TagPresentationModel> tag);
	void DeleteTag(std::shared_ptr<TagPresentationModel> tag);
	void OnTextChanged(wxKeyEvent& event);
	void OnTextKeyDown(wxKeyEvent& event);
};

