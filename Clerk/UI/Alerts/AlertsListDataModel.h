#pragma once

#include <wx/dataview.h>
#include "../../Data/DataHelper.h"
#include "../../Utils/Utils.h"

class AlertsListDataModel : public wxDataViewListStore
{
public:
	enum class Columns
	{
		Name,
		Type,
		Period,
		Condition,
		Amount,
		Last
	};

	AlertsListDataModel();
	~AlertsListDataModel();

	void SetItems(std::vector<std::shared_ptr<Alert>> budgets);
	
	virtual unsigned int GetColumnCount() const;
	virtual wxString GetColumnType(unsigned int column) const;
	virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const;
	virtual bool GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr &attr) const;
	virtual bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int column);

private:
	std::vector<std::shared_ptr<Alert>> _alerts;
};

