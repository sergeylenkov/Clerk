#include "AmountField.h"

AmountField::AmountField(wxWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size) : wxTextCtrl(parent, id, value, pos, size, wxTE_RIGHT)
{
	wxString allowedChars[17] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", ",", " ", "+", "-", "*", "/" };
	wxArrayString chars(17, allowedChars);

	wxTextValidator validator(wxFILTER_INCLUDE_CHAR_LIST);
	validator.SetIncludes(chars);

	this->SetValidator(validator);
}

AmountField::~AmountField()
{
}

float AmountField::GetFloatValue()
{
	return GetValueFromString(this->GetValue());
}

wxString AmountField::ClearAmountValue(wxString& value)
{
	value.Trim(true);
	value.Trim(false);
	value.Replace(",", ".", true);
	value.Replace(" ", "", true);
	value.Replace("\xA0", "", true);

	return value;
}

float AmountField::GetValueFromString(wxString& value)
{
	wxString clearValue = ClearAmountValue(value);

	if (clearValue.Length() > 0) {
		wxStringTokenizer tokenizer(clearValue, "+-/*");
		std::vector<float> values;
		std::vector<wxChar> operands;

		while (tokenizer.HasMoreTokens()) {
			wxString token = tokenizer.GetNextToken().Trim(true).Trim(false);
			wxChar operand = tokenizer.GetLastDelimiter();
			
			double result = 0;
			token.ToCDouble(&result);

			values.push_back(result);
			operands.push_back(operand);
		}

		float result = 0.0;

		if (values.size() > 0) {
			result = values[0];

			for (unsigned int i = 1; i < values.size(); i++) {
				wxChar operand = operands[i - 1];

				switch (operand)
				{
					case '+':
						result = result + values[i];
						break;
					case '-':
						result = result - values[i];
						break;
					case '*':
						result = result * values[i];
						break;
					case '/':
						result = result / values[i];
						break;
					default:
						break;
				}				
			}
		}

		return result;
	}

	return 0.0;
}