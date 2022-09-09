#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <thread>
#include <memory>
#include "DataPanel.h"
#include "../Data/ViewModels/TagViewModel.h"
#include "../Commands/CommandsInvoker.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;

enum class TagsPanelMenuTypes {
	Show = 1,
	Rename = 2,
	Delete = 3,	
};

class TagsPanel : public DataPanel
{
public:
	TagsPanel(wxWindow *parent, DataContext& context, CommandsInvoker& commandsInvoker);
	~TagsPanel();

	void Update();
	std::shared_ptr<TagViewModel> GetTag();

private:
	wxTextCtrl *searchField;
	wxListCtrl *list;
	std::vector<std::shared_ptr<TagViewModel>> tags;
	std::vector<std::shared_ptr<TagViewModel>> filteredTags;
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

