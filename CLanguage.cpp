#include "CLanguage.h"

CLanguage::CLanguage(const std::string & inputFileName, std::ostream & outputStream)
		: _input(inputFileName)
		, _output(outputStream)
{
	_grammarType = DetermineGrammarType(inputFileName);
	if (_grammarType == LeftLinear)
	{
		ReadLeftLinearGrammar();
	}
	else
	{
		ReadRightLinearGrammar();
	}
}

GrammarType CLanguage::DetermineGrammarType(const std::string & inputFileName)
{
	CInput input(inputFileName);
	input.SkipArguments<std::wstring>(2);

	std::wstring firstRule;
	if (!input.ReadArguments(firstRule))
	{
		return None;
	}
	return (islower(firstRule[0])) ? RightLinear : LeftLinear;
}

void CLanguage::ReadLeftLinearGrammar()
{
	wchar_t toState;
	_input.ReadArguments(toState);
	_input.SkipArgument<std::wstring>();

	wchar_t fromState;
	_input.ReadArguments(fromState);

	wchar_t signal;
	_input.ReadArguments(signal);
}

void CLanguage::ReadRightLinearGrammar()
{

}