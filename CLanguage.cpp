#include "CLanguage.h"

CLanguage::CLanguage(const std::string &inputFileName, std::ostream &outputStream)
		: _input(inputFileName)
{
	_linearGrammarType = DetermineLinearGrammarType(inputFileName);
	if (_linearGrammarType == None)
	{
		throw std::invalid_argument("не удалось определить тип грамматики");
	}
	ReadLinearGrammar();
	ValidateLinearGrammar();
	DeterminateAutomaton();
	PrintAssociations();
}

void CLanguage::CreateNonDeterministicDiagram(const std::string & diagramFileName) const
{
	CreateDiagram(diagramFileName, _transitionMap);
}

void CLanguage::CreateDeterministicDiagram(const std::string & diagramFileName) const
{
	CreateDiagram(diagramFileName, _determinatedTransitionMap);
}

bool CLanguage::isShowStateComponents() const
{
	return _showStateComponents;
}

void CLanguage::setShowStateComponents(bool showStateComponents)
{
	_showStateComponents = showStateComponents;
}

LinearGrammarType CLanguage::DetermineLinearGrammarType(const std::string &inputFileName) const
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
		State argument0;
		_input.ReadArguments(argument0);
		_input.SkipArgument<std::wstring>();
		GetState(argument0);

		while (true)
		{
			_input.SkipSymbols({ ' ' });
			wchar_t argument1;
			wchar_t argument2;
			_input.ReadArguments(argument1, argument2);

			State &sourceState = _linearGrammarType == LeftLinear ? argument1 : argument0;
			Signal &signal = _linearGrammarType == LeftLinear ? argument2 : argument1;
			State &destinationState = _linearGrammarType == LeftLinear ? argument0 : argument2;
			AddToTransitionMap(sourceState, signal, destinationState);

			_input.SkipSymbols({ ' ' });
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
	switch (_linearGrammarType)
	{
		case LeftLinear:
			_transitionMap.try_emplace(CLanguage::_startState);
			break;
		case RightLinear:
			_transitionMap.try_emplace(CLanguage::_endState);
			break;
		default:
			break;
	}
}

void CLanguage::AddToTransitionMap(State sourceState, Signal signal, State destinationState)
{
	std::unordered_map<Signal, std::unordered_set<State>> &transitions =
			(*_transitionMap.try_emplace(sourceState, std::unordered_map<Signal, std::unordered_set<State>>()).first).second;

	std::unordered_set<State> &destinationStates =
			(*transitions.try_emplace(signal, std::unordered_set<State>()).first).second;
	destinationStates.emplace(destinationState);
}

void CLanguage::ValidateLinearGrammar() const
{
	switch (_linearGrammarType)
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
	std::queue<State> statesQueue;
	std::unordered_set<std::unordered_set<State>, UnorderedSetHash> processedStatesComponents;
	InitStateQueue(statesQueue);
	_associations.emplace(statesQueue.front(), std::unordered_set<State>({statesQueue.front()}));
	while (!statesQueue.empty())
	{
		State associatedState = statesQueue.front();
		statesQueue.pop();
		_determinatedTransitionMap.try_emplace(associatedState);
		std::unordered_set<State> stateComponents = _associations.at(associatedState);
		if (processedStatesComponents.find(stateComponents) != processedStatesComponents.end())
		{
			continue;
		}
		std::unordered_map<Signal, std::unordered_set<State>> newStateTransitions;
		for (State sourceState : stateComponents)
		{
			if (_transitionMap.find(sourceState) == _transitionMap.end())
			{
				continue;
			}
			for (const std::pair<const Signal, std::unordered_set<State>> &stateSignalToTransitions : _transitionMap.at(sourceState))
			{
				Signal signal = stateSignalToTransitions.first;
				newStateTransitions.try_emplace(signal, std::unordered_set<State>());
				for (State destinationState : stateSignalToTransitions.second)
				{
					std::unordered_set<State> &signalStates = newStateTransitions.at(signal);
					signalStates.emplace(destinationState);
				}
			}
		}
		if (!newStateTransitions.empty())
		{
			for (std::pair<const Signal, std::unordered_set<State>> &newStateSignalTransitions : newStateTransitions)
			{
				State newState;
				if (processedStatesComponents.find(newStateSignalTransitions.second) != processedStatesComponents.end())
				{
					newState = _statesComponentsToAssociatedState.at(newStateSignalTransitions.second);
				}
				else if (newStateSignalTransitions.second == stateComponents)
				{
					newState = _statesComponentsToAssociatedState.at(stateComponents);
				}
				else
				{
					newState = GetFreeState();
				}
				std::unordered_map<Signal, std::unordered_set<State>> &stateTransitions =
						_determinatedTransitionMap.at(associatedState);
				std::unordered_set<State> &signalStates =
						(*stateTransitions.try_emplace(newStateSignalTransitions.first).first).second;
				signalStates.emplace(newState);
				std::unordered_set<State> asd;
				_associations.emplace(newState, newStateSignalTransitions.second);
				_statesComponentsToAssociatedState.emplace(newStateSignalTransitions.second, newState);
				statesQueue.push(newState);
			}
		}
		processedStatesComponents.emplace(stateComponents);
	}
}

void CLanguage::InitStateQueue(std::queue<wchar_t> &stateQueue) const
{
	switch (_linearGrammarType)
	{
		case LeftLinear:
			stateQueue.emplace(CLanguage::_endState);
			break;
		case RightLinear:
			stateQueue.emplace(CLanguage::_startState);
			break;
		default:
			break;
	}
}

State CLanguage::GetState(State state)
{
	if (_freeStates.find(state) == _freeStates.end())
	{
		throw std::runtime_error("это состояние уже занято");
	}
	auto frontSymbol = _freeStates.extract(state);
	_nonFreeStates.emplace(frontSymbol.value());

	return frontSymbol.value();
}

State CLanguage::GetFreeState()
{
	if (_freeStates.empty())
	{
		throw std::runtime_error(
				(std::string("превышен лимит состояний (") + std::to_string(_nonFreeStates.size()) + ")").c_str()
		);
	}
	auto frontSymbol = _freeStates.extract(_freeStates.begin());
	_nonFreeStates.emplace(frontSymbol.value());

	return frontSymbol.value();
}

void CLanguage::PrintAssociations() const
{
	for (const std::pair<const State, std::unordered_set<State>> &association : _associations)
	{
		std::wcout << association.first << L": ";
		for (State associationElement : association.second)
		{
			std::wcout << associationElement;
		}
		std::wcout << L"\n";
	}
}

void CLanguage::CreateDiagram(
		const std::string & diagramFileName,
		const std::unordered_map<State, std::unordered_map<Signal, std::unordered_set<State>>> & transitionMap) const
{
	using Graph = boost::adjacency_list<
			boost::vecS,
			boost::vecS,
			boost::directedS,
			boost::property<boost::vertex_name_t, std::string>,
			boost::property<boost::edge_name_t, std::string>
	>;

	using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
	using Edge = boost::graph_traits<Graph>::edge_descriptor;

	using VerticesName = boost::property_map<Graph, boost::vertex_name_t>::type;
	using EdgesName = boost::property_map<Graph, boost::edge_name_t>::type;

	Graph graph;
	VerticesName verticesName = boost::get(boost::vertex_name, graph);
	EdgesName edgesName = boost::get(boost::edge_name, graph);

	std::unordered_map<State, Vertex> vertices;
	for (const std::pair<const State, std::unordered_map<Signal, std::unordered_set<State>>> & stateTransitions : transitionMap)
	{
		vertices[stateTransitions.first] = boost::add_vertex(graph);
	}
	for (const std::pair<const State, Vertex> &vertex : vertices)
	{
		State sourceState = vertex.first;
		if (_associations.find(sourceState) == _associations.end() || !_showStateComponents)
		{
			verticesName[vertex.second] =  sourceState;
		}
		else
		{
			std::string stateComponentsString;
			for (State stateComponent : _associations.at(sourceState))
			{
				stateComponentsString += stateComponent;
			}
			verticesName[vertex.second] = stateComponentsString;
		}
		for (const std::pair<const Signal, std::unordered_set<State>> &transition : transitionMap.at(sourceState))
		{
			Signal signal = transition.first;
			const std::unordered_set<State> &destinationStates = transition.second;
			for (State destinationState : destinationStates)
			{
				Edge edge;
				bool inserted;
				std::tie(edge, inserted) = boost::add_edge(vertex.second, vertices.at(destinationState), graph);
				if (!inserted)
				{
					throw std::invalid_argument(
							"Не удалось проложить дугу между вершинами: "
							+ std::to_string(vertex.first)
							+ " и "
							+ std::to_string(_statesComponentsToAssociatedState.at(transition.second))
					);
				}
				edgesName[edge] = signal;
			}
		}
	}
	boost::dynamic_properties dp;
	dp.property("node_id", verticesName);
	dp.property("label", edgesName);
	std::ofstream ofs("tmp.dot");
	boost::write_graphviz_dp(ofs, graph, dp);
	ofs.close();
	if (system(("dot -Tpng -o " + diagramFileName + " tmp.dot").c_str()) != 0)
	{
		throw std::runtime_error("Не удалось создать диаграмму. Возможно, программа \"dot\" не установлена");
	}
	if (remove("tmp.dot") == -1)
	{
		throw std::runtime_error("Не удалось удалить мусор за собой (файл \"tmp.dot\")");
	}
}

const wchar_t CLanguage::_startState = 'S';

const wchar_t CLanguage::_endState = 'H';
