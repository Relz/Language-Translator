#ifndef LANGUAGETRANSLATOR_CLANGUAGE_H
#define LANGUAGETRANSLATOR_CLANGUAGE_H

#include <iostream>
#include <string>
#include <cctype>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>
#include "Input/Input.h"
#include "UnorderedSetHash.h"

enum LinearGrammarType
{
	None,
	LeftLinear,
	RightLinear
};

using State = wchar_t;
using Signal = wchar_t;

class CLanguage
{
public:
	explicit CLanguage(const std::string &inputFile, std::ostream &outputStream = std::cout);

	void CreateNonDeterministicDiagram(const std::string & diagramFileName) const;

	void CreateDeterministicDiagram(const std::string & diagramFileName) const;

	bool isShowStateComponents() const;

	void setShowStateComponents(bool showStateComponents);

private:
	LinearGrammarType DetermineLinearGrammarType(const std::string &inputFilename) const;

	void ReadLinearGrammar();

	void AddToTransitionMap(State sourceState, Signal signal, State destinationState);

	void ValidateLinearGrammar() const;

	void DeterminateAutomaton();

	void InitStateQueue(std::queue<Signal> &stateQueue) const;

	State GetState(State state);

	State GetFreeState();

	void PrintAssociations() const;

	void CreateDiagram(
			const std::string & diagramFileName,
			const std::unordered_map<State, std::unordered_map<Signal, std::unordered_set<State>>> & transitionMap) const;

	CInput _input;

	LinearGrammarType _linearGrammarType;

	std::unordered_map<State, std::unordered_map<Signal, std::unordered_set<State>>> _transitionMap;

	std::unordered_map<State, std::unordered_set<State>> _associations;

	std::unordered_map<std::unordered_set<State>, State, UnorderedSetHash> _statesComponentsToAssociatedState;

	std::unordered_map<State, std::unordered_map<Signal, std::unordered_set<State>>> _determinatedTransitionMap;

	bool _showStateComponents = false;

	std::unordered_set<State> _freeStates = {
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U',
			'V', 'W', 'X', 'Y', 'Z'
	};

	std::unordered_set<wchar_t> _nonFreeStates;

	static const wchar_t _startState;

	static const wchar_t _endState;
};

#endif //LANGUAGETRANSLATOR_CLANGUAGE_H
