#ifndef Transaction_h
#define Transaction_h

#include <wx/wx.h>
#include <wx/tokenzr.h>
#include <memory>
#include "Model.h"

using namespace std;

class Transaction : public Model
{
public:
	Transaction();
	Transaction(int id);

	int id;
	int from_account_id;
	int to_account_id;
	shared_ptr<wxString> to_account_name;
	shared_ptr<wxString> from_account_name;
	float from_amount;
	float to_amount;
	shared_ptr<wxDateTime> paid_at;
	shared_ptr<wxString> note;
	shared_ptr<wxString> tags;

	void Load();
	void Save();
	void Delete();

private:
	void UpdateTags();
};

#endif