#include "TransactionsListDataModel.h"

TransactionsListDataModel::TransactionsListDataModel()
{
}

TransactionsListDataModel::~TransactionsListDataModel()
{
}

void TransactionsListDataModel::SetItems(std::vector<std::shared_ptr<Transaction>> transactions) {
	_transactions = transactions;
	Reset(_transactions.size());
}

unsigned int TransactionsListDataModel::GetColumnCount() const
{
	return 7;
}

wxString TransactionsListDataModel::GetColumnType(unsigned int column) const
{
	return wxT("string");
}

void TransactionsListDataModel::GetValueByRow(wxVariant &variant, unsigned int row, unsigned int column) const
{
	auto transaction = _transactions[row];

	switch (static_cast<Columns>(column))
	{
		case Columns::Date:
			variant = FormatDate(transaction->paidAt.get());
			break;
		case Columns::FromAccount:
			variant = *transaction->fromAccount->name;
			break;
		case Columns::ToAccount:
			variant = *transaction->toAccount->name;
			break;
		case Columns::Amount:
			variant = Utils::FormatAmount(transaction->fromAmount);
			break;
		case Columns::Tags:
			variant = transaction->GetTagsString();
			break;
		case Columns::Note:
			variant = *transaction->note;
			break;
	}
}

bool TransactionsListDataModel::GetAttrByRow(unsigned int row, unsigned int column, wxDataViewItemAttr &attr) const
{
	return false;
}

bool TransactionsListDataModel::SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int column)
{
	return false;
}

wxString TransactionsListDataModel::FormatDate(wxDateTime *date) const
{
	wxString dateFormat = date->Format("%B %e");

	if (wxDateTime::Now().GetYear() != date->GetYear()) {
		dateFormat = date->Format("%B %e, %Y");
	}

	return dateFormat;
}