#pragma once

#include <wx/dataview.h>
#include "../PresentationModels/TransactionPresentationModel.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class TransactionsListDataModel : public wxDataViewListStore
{
public:
	enum class Columns
	{
		Date,
		FromAccount,
		ToAccount,
		Tags,
		Note,
		Amount,		
		Last
	};

	TransactionsListDataModel();
	~TransactionsListDataModel();

	void SetItems(std::vector<std::shared_ptr<TransactionPresentationModel>> transactions);

	virtual unsigned int GetColumnCount() const;
	virtual wxString GetColumnType(unsigned int column) const;
	virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const;
	virtual bool GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr &attr) const;
	virtual bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int column);

private:
	std::vector<std::shared_ptr<TransactionPresentationModel>> _transactions;
	wxString FormatDate(const wxDateTime& date) const;
	wxString FormatAmount(const TransactionPresentationModel& transaction) const;
};

