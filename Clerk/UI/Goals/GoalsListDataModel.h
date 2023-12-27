#pragma once

#include <wx/dataview.h>
#include "../../Utils/Utils.h"
#include "../../Utils/Types.h"
#include "../../Data/PresentationModels/GoalPresentationModel.h"
#include "Enums.h"

using namespace Clerk::Utils;
using namespace Clerk::Data;
using namespace Clerk::UI;

class GoalsListDataModel : public wxDataViewIndexListModel
{
public:
	GoalsListDataModel();

	void SetItems(shared_vector<GoalPresentationModel> goals);

	unsigned int GetColumnCount() const;
	wxString GetColumnType(unsigned int column) const;
	virtual void GetValueByRow(wxVariant& variant, unsigned int row, unsigned int column) const;
	virtual bool SetValueByRow(const wxVariant& variant, unsigned int row, unsigned int column);
	virtual bool GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr& attr) const;
	virtual int Compare(const wxDataViewItem& item1, const wxDataViewItem& item2, unsigned int column, bool	ascending) const;
	virtual bool HasDefaultCompare() const;
	unsigned int GetCount() const override;

private:
	shared_vector<GoalPresentationModel> _goals;	
};

