#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <thread>
#include <memory>
#include "../../Data/PresentationModels/TagPresentationModel.h"
#include "DataPanel.h"

using namespace Clerk::Data;

enum class TagsPanelMenuTypes {
	Show = 1,
	Rename = 2,
	Delete = 3,	
};

class TagsPanel : public DataPanel
{
public:
	TagsPanel(wxWindow *parent, DataContext& context, Icons& icons);
	~TagsPanel();

	void Update();
	std::shared_ptr<TagPresentationModel> GetTag();

private:
	wxTextCtrl *searchField;
	wxListCtrl *list;
	std::vector<std::shared_ptr<TagPresentationModel>> tags;
	std::vector<std::shared_ptr<TagPresentationModel>> filteredTags;
	long editedIndex;

	void Filter();
	void UpdateList();
	void ShowTransactions();
	void Rename();
	void Delete();
	long GetSelectedIndex();
	void DeleteItemAtIndex(long index);
	void OnSearchChanged(wxCommandEvent &event);
	void OnListItemBeginEdit(wxListEvent &event);
	void OnListItemEndEdit(wxListEvent &event);
	void OnListItemDoubleClick(wxListEvent &event);
	void OnRightClick(wxContextMenuEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};

