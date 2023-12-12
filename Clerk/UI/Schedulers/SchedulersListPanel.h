#pragma once

#include <wx/wx.h>
#include "wx/dataview.h"
#include "../PresentationModels/SchedulerPresentationModel.h"
#include "../DataPanel.h"
#include "SchedulersListDataModel.h"

using namespace Clerk::Data;

class SchedulersListPanel : public DataPanel
{
public:
	SchedulersListPanel(wxWindow *parent, DataContext& context);
	
	std::shared_ptr<SchedulerPresentationModel> GetScheduler();
	void Update();	

	std::function<void(std::shared_ptr<SchedulerPresentationModel> scheduler)> OnEdit;
	std::function<void()> OnAdd;

private:
	wxDataViewCtrl *list;
	wxObjectDataPtr<SchedulersListDataModel> model;
	std::vector<std::shared_ptr<SchedulerPresentationModel>> schedulers;

	void Add();
	void Edit();
	void Delete();
	void Run();
	void Pause();
	void OnListItemDoubleClick(wxDataViewEvent &event);
	void OnRightClick(wxDataViewEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};