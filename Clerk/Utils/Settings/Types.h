#pragma once

struct TabSettings {
	int type;
	int id;
};

struct ListColumnsSettings {
	int model;
	int order;
	int width;
	bool sorted;
	bool sortedAscending;
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

struct Language {
	int id;
	wxString name;
};