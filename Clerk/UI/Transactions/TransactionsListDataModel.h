#pragma once

#include <wx/dataview.h>
#include "../PresentationModels/TransactionPresentationModel.h"
#include "../../Utils/Utils.h"
#include "../../Utils/Types.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class TransactionsListDataModel : public wxDataViewIndexListModel
{
public:
	TransactionsListDataModel();

	void SetItems(shared_vector<TransactionPresentationModel> transactions);

	unsigned int GetColumnCount() const;
	wxString GetColumnType(unsigned int column) const;
	virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const;
	virtual bool SetValueByRow(const wxVariant& variant, unsigned int row, unsigned int column);
	virtual int Compare(const wxDataViewItem& item1, const wxDataViewItem& item2, unsigned int column, bool	ascending) const;
	virtual bool HasDefaultCompare() const;
	unsigned int GetCount() const override;

private:
	shared_vector<TransactionPresentationModel> _transactions;
};

