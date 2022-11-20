#include "TransactionsListDataModel.h"

TransactionsListDataModel::TransactionsListDataModel()
{
}

TransactionsListDataModel::~TransactionsListDataModel()
{
}

void TransactionsListDataModel::SetItems(std::vector<std::shared_ptr<TransactionPresentationModel>> transactions) {
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
			variant = FormatDate(transaction->date);
			break;
		case Columns::FromAccount:
			variant = transaction->fromAccount->name;
			break;
		case Columns::ToAccount:
			variant = transaction->toAccount->name;
			break;
		case Columns::Amount: {
			wxArrayString values;

			//TODO
			wxString fromValue = Format::Amount(transaction->fromAmount);
			wxString toValue = Format::Amount(transaction->toAmount);

			values.Add(toValue);

			if (transaction->fromAmount != transaction->toAmount) {
				values.Add(fromValue);
			}

			variant = values;
		}
			
			break;
		case Columns::Tags: {
			wxArrayString tags;

			for (auto &tag : transaction->tags) {
				tags.Add(tag->name);
			}

			variant = tags;
		}
			break;
		case Columns::Note:
			variant = transaction->note;
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

wxString TransactionsListDataModel::FormatDate(const wxDateTime& date) const
{
	wxString dateFormat = date.Format("%B %e");

	if (wxDateTime::Now().GetYear() != date.GetYear()) {
		dateFormat = date.Format("%B %e, %Y");
	}

	return dateFormat;
}

wxString TransactionsListDataModel::FormatAmount(const TransactionPresentationModel& transaction) const {
	wxString amount = "";
	// TODO
	if (transaction.fromAmount != transaction.toAmount) {
		wxString fromAmount = Format::Amount(transaction.fromAmount);
		wxString toAmount = Format::Amount(transaction.toAmount);

		amount = wxString::Format("%s %s", fromAmount, toAmount);
	}
	else {
		amount = Format::Amount(transaction.toAmount);
	}

	return amount;
}