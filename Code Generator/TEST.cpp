/****
*file:"TEST.cpp"
*synopsis: testing code generating functions
*author: R. Neshta
*written: 02/06/17
*last modified: 02/06/17
***/

#include<iostream>

#include"../Scaner/InformationTables.h"
#include"../Scaner/InformationTables.cpp"

#include"../Scaner/LexicalAnalyser.h"
#include"../Scaner/LexicalAnalyser.cpp"

#include"../Syntax Analyser/Syntax Analyser.h"
#include"../Syntax Analyser/Syntax Analyser.cpp"

#include"CodeGenerator.h"


using namespace InformationTables;


int main() {
	
	const char* FileName = "D:\\prog\\Microsoft Visual Studio\\Projects\\Software Engineering\\Scaner\\SimpleTask.txt";
	std::vector<Token> stream = Scaner::Scan(FileName);

	std::cout << "Token stream:" << std::endl;
	for (auto &i : stream)
		i.Print(std::cout);

	IdentifierTables::Instance().Print();


	parser::parse(stream);
	parser::tree::printTree();

	std::cout << "Program listing on Assembly language:\n";
	std::cout << "-------------------------------------" << std::endl;

	semanticProcessor::generate(std::cout, stream, parser::tree::root);

	std::cout << "-------------------------------------" << std::endl;


	semanticProcessor::processorTables::_i_Ts::getInstance().print(std::cout);


	
	return EXIT_SUCCESS;

}
