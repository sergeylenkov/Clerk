#ifndef TagsPopup_h
#define TagsPopup_h

#include <wx/wx.h>
#include <wx/popupwin.h>
#include <wx/listctrl.h>
#include <vector>
#include <memory>

using namespace std;

class TagsPopup : public wxPopupWindow
{
public:
	TagsPopup(wxWindow *parent);
	~TagsPopup();

	void Update(vector<shared_ptr<wxString>> tags);	
	void SelectNext();
	void SelectPrev();
	wxString GetSelectedTag();

private:
	wxScrolledWindow *panel;
	wxListBox *list;
};

#endif