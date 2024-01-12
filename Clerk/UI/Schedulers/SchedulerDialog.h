#pragma once

#include <wx/wx.h>
#include "../../Utils/Icons.h"
#include "../../Data/DataContext.h"
#include "SchedulerViewModel.h"
#include "SchedulerPatternPanel.h"
#include "SchedulerTransactionPanel.h"

using namespace Clerk::Data;
using namespace Clerk::UI;
using namespace Clerk::Utils;

class SchedulerDialog : public wxFrame
{
public:
	SchedulerDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height, Icons& icons, DataContext& context);
	~SchedulerDialog();

	void SetViewModel(SchedulerViewModel* viewModel);

private:
	SchedulerViewModel* _viewModel;
	Icons& _icons;
	DataContext& _context;
	wxTextCtrl *_nameField;
	SchedulerPatternPanel* _schedulerPatternPanel;
	SchedulerTransactionPanel* _schedulerTransactionPanel;

	void Update();	
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
	void OnKeyDown(wxKeyEvent &event);
};
