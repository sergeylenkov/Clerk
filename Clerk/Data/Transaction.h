#pragma once

#include <wx/wx.h>
#include <wx/tokenzr.h>
#include <memory>
#include "Model.h"

using namespace std;

class Transaction : public Model
{
public:
	int id;
	int fromAccountId;
	int toAccountId;
	shared_ptr<wxString> toAccountName;
	shared_ptr<wxString> fromAccountName;
	float fromAmount;
	float toAmount;
	shared_ptr<wxDateTime> paidAt;
	shared_ptr<wxString> note;
	shared_ptr<wxString> tags;

	Transaction();
	Transaction(int id);
	void Load();
	void Save();
	void Delete();
	void DeleteCompletely();
	void Restore();
	vector<wxString> GetTags();

private:
	void UpdateTags();
};