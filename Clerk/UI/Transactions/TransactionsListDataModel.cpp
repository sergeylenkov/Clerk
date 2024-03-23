#include "TransactionsListDataModel.h"

TransactionsListDataModel::TransactionsListDataModel()
{
}

void TransactionsListDataModel::SetItems(shared_vector<TransactionPresentationModel> transactions) {
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

	switch (static_cast<TransactionsListColumns>(column))
	{
		case TransactionsListColumns::Date:
			variant = Format::Date(transaction->date);
			break;
		case TransactionsListColumns::FromAccount:
			variant = transaction->fromAccount->name;
			break;
		case TransactionsListColumns::ToAccount:
			variant = transaction->toAccount->name;
			break;
		case TransactionsListColumns::Amount: {
			wxArrayString values;

			wxString fromValue = Format::Amount(transaction->fromAmount);
			wxString toValue = Format::Amount(transaction->toAmount);

			values.Add(toValue);

			if (transaction->fromAmount != transaction->toAmount) {
				values.Add(fromValue);
			}

			variant = values;
		}
			
			break;
		case TransactionsListColumns::Tags: {
			wxArrayString tags;

			for (auto &tag : transaction->tags) {
				tags.Add(tag->name);
			}

			variant = tags;
		}
			break;
		case TransactionsListColumns::Note:
			variant = transaction->note;
			break;
	}
}

bool TransactionsListDataModel::SetValueByRow(const wxVariant& variant, unsigned int row, unsigned int column) {
	return false;
}

int TransactionsListDataModel::Compare(const wxDataViewItem& item1, const wxDataViewItem& item2, unsigned int column, bool ascending) const {
	wxUIntPtr index1 = wxPtrToUInt(item1.GetID()) - 1;
	wxUIntPtr index2 = wxPtrToUInt(item2.GetID()) - 1;

	auto v1 = _transactions[index1];
	auto v2 = _transactions[index2];

	if (static_cast<TransactionsListColumns>(column) == TransactionsListColumns::Date) {
		if (v1->date.IsEqualTo(v2->date)) {
			return ascending ? v1->id > v2->id : v2->id > v1->id;
		}

		return (ascending == v1->date.IsLaterThan(v2->date)) ? 1 : -1;
	}

	if (static_cast<TransactionsListColumns>(column) == TransactionsListColumns::FromAccount) {
		return ascending ? v1->fromAccount->name.Cmp(v2->fromAccount->name) : v2->fromAccount->name.Cmp(v1->fromAccount->name);
	}

	if (static_cast<TransactionsListColumns>(column) == TransactionsListColumns::ToAccount) {
		return ascending ? v1->toAccount->name.Cmp(v2->toAccount->name) : v2->toAccount->name.Cmp(v1->toAccount->name);
	}

	if (static_cast<TransactionsListColumns>(column) == TransactionsListColumns::Tags) {
		wxString tag1 = v1->tagsString.Lower();
		wxString tag2 = v2->tagsString.Lower();

		return ascending ? tag1.CmpNoCase(tag2) : tag2.CmpNoCase(tag1);
	}

	if (static_cast<TransactionsListColumns>(column) == TransactionsListColumns::Note) {		
		return ascending ? v1->note.Cmp(v2->note) : v2->note.Cmp(v1->note);
	}

	if (static_cast<TransactionsListColumns>(column) == TransactionsListColumns::Amount) {
		if (v1->fromAmount == v2->fromAmount) {
			return 0;
		}

		return ascending ? v1->fromAmount > v2->fromAmount : v2->fromAmount > v1->fromAmount;
	}

	return 0;
}

unsigned int TransactionsListDataModel::GetCount() const {
	return _transactions.size();
}

bool TransactionsListDataModel::HasDefaultCompare() const {
	return false;
}