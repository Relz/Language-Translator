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

	AddToTransitionMap(fromState, signal, toState);
}

void CLanguage::ReadRightLinearGrammar()
{
	wchar_t fromState;
	_input.ReadArguments(fromState);
	_input.SkipArgument<std::wstring>();

	wchar_t toState;
	_input.ReadArguments(toState);

	wchar_t signal;
	_input.ReadArguments(signal);

	AddToTransitionMap(fromState, signal, toState);
}

void CLanguage::AddToTransitionMap(wchar_t fromState, wchar_t signal, wchar_t toState)
{
	std::unordered_map<wchar_t, std::vector<wchar_t>> transitions =
			(*_transitionMap.try_emplace(fromState, std::unordered_map<wchar_t, std::vector<wchar_t>>()).first).second;

	std::vector<wchar_t> toStates = (*transitions.try_emplace(signal, std::vector<wchar_t>()).first).second;
	toStates.emplace_back(toState);
}
