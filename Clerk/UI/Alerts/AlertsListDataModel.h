#pragma once

#include <wx/dataview.h>
#include "../../Utils/Utils.h"
#include "../../Utils/Types.h"
#include "../../Data/PresentationModels/AlertPresentationModel.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;

class AlertsListDataModel : public wxDataViewIndexListModel
{
public:
	AlertsListDataModel();

	void SetItems(shared_vector<AlertPresentationModel> alerts);
	
	unsigned int GetColumnCount() const;
	wxString GetColumnType(unsigned int column) const;
	virtual void GetValueByRow(wxVariant& variant, unsigned int row, unsigned int column) const;
	virtual bool SetValueByRow(const wxVariant& variant, unsigned int row, unsigned int column);
	virtual int Compare(const wxDataViewItem& item1, const wxDataViewItem& item2, unsigned int column, bool	ascending) const;
	virtual bool HasDefaultCompare() const;
	unsigned int GetCount() const override;

private:
	shared_vector<AlertPresentationModel> _alerts;
};

