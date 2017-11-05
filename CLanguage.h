#ifndef LANGUAGETRANSLATOR_CLANGUAGE_H
#define LANGUAGETRANSLATOR_CLANGUAGE_H

#include <iostream>
#include <string>
#include <cctype>
#include "Input/Input.h"
#include "Output/Output.h"

enum LinearGrammarType
{
	None,
	LeftLinear,
	RightLinear
};

class CLanguage
{
public:
	explicit CLanguage(const std::string &inputFile, std::ostream &outputStream = std::cout);

private:
	LinearGrammarType DetermineLinearGrammarType(const std::string &inputFilename);

	void ReadLinearGrammar();

	void AddToTransitionMap(wchar_t fromState, wchar_t signal, wchar_t toState);

	void ValidateLinearGrammar();

	void DeterminateAutomaton();

	CInput _input;
	COutput _output;
	LinearGrammarType _linearGrammarType;
	std::unordered_map<wchar_t, std::unordered_map<wchar_t, std::vector<wchar_t>>> _transitionMap;

	static wchar_t _startState;
	static wchar_t _endState;
};


#endif //LANGUAGETRANSLATOR_CLANGUAGE_H
