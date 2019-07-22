#pragma once

#include <wx/wx.h>
#include <wx/tokenzr.h>
#include <memory>
#include "Model.h"
#include "Account.h"

using namespace std;

class Transaction : public Model
{
public:
	int id;
	shared_ptr<Account> toAccount;
	shared_ptr<Account> fromAccount;
	float fromAmount;
	float toAmount;
	shared_ptr<wxDateTime> paidAt;
	shared_ptr<wxString> note;	
	vector<wxString> tags;

	Transaction();
	Transaction(int id);
	void Load();
	void Save();
	void Delete();
	void DeleteCompletely();
	void Restore();	
	wxString GetTagsString();
	void SetTagsString(wxString tagsString);

private:	
	void UpdateTags();
};