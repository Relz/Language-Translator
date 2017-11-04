#ifndef LANGUAGETRANSLATOR_CLANGUAGE_H
#define LANGUAGETRANSLATOR_CLANGUAGE_H

#include <iostream>
#include <string>
#include <cctype>
#include "Input/Input.h"
#include "Output/Output.h"

enum GrammarType
{
	None,
	LeftLinear,
	RightLinear
};

class CLanguage {
public:
	explicit CLanguage(const std::string & inputFile, std::ostream & outputStream = std::cout);
private:
	GrammarType DetermineGrammarType(const std::string & inputFilename);
	void ReadLeftLinearGrammar();
	void ReadRightLinearGrammar();

	CInput _input;
	COutput _output;
	GrammarType _grammarType;
	std::unordered_map<wchar_t, std::pair<wchar_t, wchar_t>> _transitionMap;
};


#endif //LANGUAGETRANSLATOR_CLANGUAGE_H