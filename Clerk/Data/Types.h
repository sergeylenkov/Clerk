#pragma once


struct TabSettings {
	int type;
	int id;
};

struct ListColumnsSettings {
	int index;
	int order;
	wxString title;
	int width;
	bool sorted;
};

struct ListFilterSettings {
	int type;
	int id;
	int period;
	wxDateTime fromDate;
	wxDateTime toDate;
};

enum class TransactionsListType {
	All = 0,
	Receipts = 1,
	Deposits = 2,
	Expenses = 3,
};

struct ReportFilterSettings {
	int id;
	wxString accountIds;
	int period;
	wxDateTime fromDate;
	wxDateTime toDate;
	bool average;
};