#pragma once

#include <wx/dataview.h>
#include "../../Utils/Utils.h"
#include "../PresentationModels/AlertPresentationModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;

class AlertsListDataModel : public wxDataViewListStore
{
public:
	enum class Columns
	{
		Name,
		Message,
		Type,
		Period,
		Condition,
		Importance,
		Amount,
		Last
	};

	AlertsListDataModel();
	~AlertsListDataModel();

	void SetItems(std::vector<std::shared_ptr<AlertPresentationModel>> alerts);
	
	virtual unsigned int GetColumnCount() const;
	virtual wxString GetColumnType(unsigned int column) const;
	virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const;
	virtual bool GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr &attr) const;
	virtual bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int column);

private:
	std::vector<std::shared_ptr<AlertPresentationModel>> _alerts;
};

