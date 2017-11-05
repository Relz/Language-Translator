#include <queue>
#include "CLanguage.h"

CLanguage::CLanguage(const std::string &inputFileName, std::ostream &outputStream)
		: _input(inputFileName), _output(outputStream)
{
	_linearGrammarType = DetermineLinearGrammarType(inputFileName);
	if (_linearGrammarType == None)
	{
		throw std::invalid_argument("не удалось определить тип грамматики");
	}
	ReadLinearGrammar();
	ValidateLinearGrammar();
	DeterminateAutomaton();
}

LinearGrammarType CLanguage::DetermineLinearGrammarType(const std::string &inputFileName)
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

void CLanguage::ReadLinearGrammar()
{
	while (true)
	{
		wchar_t argument0;
		_input.ReadArguments(argument0);
		_input.SkipArgument<std::wstring>();

		while (true)
		{
			_input.SkipSymbols({' '});
			wchar_t argument1;
			wchar_t argument2;
			_input.ReadArguments(argument1, argument2);

			wchar_t & fromState = (_linearGrammarType == LeftLinear) ? argument1 : argument0;
			wchar_t & signal = (_linearGrammarType == LeftLinear) ? argument2 : argument1;
			wchar_t & toState = (_linearGrammarType == LeftLinear) ? argument0 : argument2;
			AddToTransitionMap(fromState, signal, toState);

			_input.SkipSymbols({' '});
			if (!_input.SkipArgument<wchar_t>())
			{
				break;
			}
		}
		if (!_input.SkipLine())
		{
			break;
		}
	}
}

void CLanguage::AddToTransitionMap(wchar_t fromState, wchar_t signal, wchar_t toState)
{
	std::unordered_map<wchar_t, std::vector<wchar_t>> &transitions =
			(*_transitionMap.try_emplace(fromState, std::unordered_map<wchar_t, std::vector<wchar_t>>()).first).second;

	std::vector<wchar_t> &toStates = (*transitions.try_emplace(signal, std::vector<wchar_t>()).first).second;
	toStates.emplace_back(toState);
}

void CLanguage::ValidateLinearGrammar()
{
	switch(_linearGrammarType)
	{
		case LeftLinear:
			if (_transitionMap.find(CLanguage::_endState) == _transitionMap.end())
			{
				throw std::invalid_argument("грамматика не имеет конечного состояния");
			}
			break;
		case RightLinear:
			if (_transitionMap.find(CLanguage::_startState) == _transitionMap.end())
			{
				throw std::invalid_argument("грамматика не имеет начального состояния");
			}
			break;
		default:
			break;
	}
}

void CLanguage::DeterminateAutomaton()
{
	switch(_linearGrammarType)
	{
		case LeftLinear:
			break;
		case RightLinear:
			break;
		default:
			break;
	}
}

wchar_t CLanguage::_startState = 'S';
wchar_t CLanguage::_endState = 'H';
