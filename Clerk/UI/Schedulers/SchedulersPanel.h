#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../../Data/ViewModels/SchedulerViewModel.h"
#include "../DataPanel.h"
#include "../../Commands/CommandsInvoker.h"
#include "SchedulersListDataModel.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;

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

	SchedulersPanel(wxWindow *parent, DataContext& context, CommandsInvoker& commandsInvoker);
	
	std::shared_ptr<SchedulerViewModel> GetScheduler();
	void Update();	

	std::function<void(std::shared_ptr<SchedulerViewModel> scheduler)> OnEdit;
	std::function<void()> OnAdd;

private:
	wxDataViewCtrl *list;
	wxObjectDataPtr<SchedulersListDataModel> model;
	std::vector<std::shared_ptr<SchedulerViewModel>> schedulers;

	void Add();
	void Edit();
	void Delete();
	void Run();
	void Pause();
	void OnListItemDoubleClick(wxDataViewEvent &event);
	void OnRightClick(wxDataViewEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};