#include <iostream>
#include "Constant.h"
#include "CLanguage.h"

int main(int argc, char *argv[]) {
	if (argc < REQUIRED_ARGC)
	{
		std::cerr << "Ошибка: не указан файл ввода автомата\n";
		return 1;
	}
	try
	{
		const std::string nonDeterministicDiagramFileName = "beforeDiagram.png";
		const std::string deterministicDiagramFileName = "afterDiagram.png";
		CLanguage language(argv[1]);
		language.setShowStateComponents(true);
		language.CreateNonDeterministicDiagram(nonDeterministicDiagramFileName);
		language.CreateDeterministicDiagram(deterministicDiagramFileName);
	}
	catch (std::invalid_argument & invalidArgument)
	{
		std::cerr << "Неверный параметр: " << invalidArgument.what() << "\n";
		return 1;
	}

	return 0;
}
