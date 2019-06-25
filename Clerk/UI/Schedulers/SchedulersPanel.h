#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../../Data/DataHelper.h"
#include "../DataPanel.h"
#include "SchedulersListDataModel.h"

class SchedulersPanel : public DataPanel
{
public:
	enum class ContextMenuTypes {
		Add = 1,
		Edit = 2,
		Delete = 3,
		Run = 4,
		Pause = 5,
	};

	SchedulersPanel(wxWindow *parent, wxWindowID id);
	
	std::shared_ptr<Scheduler> GetScheduler();
	void Update();	

	std::function<void(std::shared_ptr<Scheduler> scheduler)> OnEdit;
	std::function<void()> OnAdd;

private:
	wxDataViewCtrl *list;
	wxObjectDataPtr<SchedulersListDataModel> model;
	std::vector<std::shared_ptr<Scheduler>> schedulers;

	void Add();
	void Edit();
	void Delete();
	void Run();
	void Pause();
	void OnListItemDoubleClick(wxDataViewEvent &event);
	void OnRightClick(wxDataViewEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};