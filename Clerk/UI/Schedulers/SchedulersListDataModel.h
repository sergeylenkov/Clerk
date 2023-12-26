#pragma once

#include <wx/dataview.h>
#include "../../Utils/Utils.h"
#include "../../Utils/Types.h"
#include "../PresentationModels/SchedulerPresentationModel.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;

class SchedulersListDataModel : public wxDataViewIndexListModel
{
public:
	SchedulersListDataModel();

	void SetItems(shared_vector<SchedulerPresentationModel> schedulers);

	unsigned int GetColumnCount() const;
	wxString GetColumnType(unsigned int column) const;
	virtual void GetValueByRow(wxVariant& variant, unsigned int row, unsigned int column) const;
	virtual bool SetValueByRow(const wxVariant& variant, unsigned int row, unsigned int column);
	virtual bool GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr& attr) const;
	virtual int Compare(const wxDataViewItem& item1, const wxDataViewItem& item2, unsigned int column, bool	ascending) const;
	virtual bool HasDefaultCompare() const;
	unsigned int GetCount() const override;

private:
	shared_vector<SchedulerPresentationModel> _schedulers;
};

