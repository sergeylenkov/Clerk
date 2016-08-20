#ifndef GraphPopup_h
#define GraphPopup_h

#include <wx/wx.h>
#include <wx/popupwin.h>
#include <wx/listctrl.h>
#include <vector>
#include <memory>

using namespace std;

class GraphPopup : public wxPopupWindow
{
public:
	GraphPopup(wxWindow *parent);
	~GraphPopup();

	void Update();

private:
	wxScrolledWindow *panel;
	wxListBox *list;
};

#endif
