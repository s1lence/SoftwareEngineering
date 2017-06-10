/****
*file:"TEST.cpp"
*synopsis: testing syntax analyzer functions
*author: R. Neshta
*written: 15/05/17
*last modified: 05/06/17
***/

#include<iostream>

#include"../Scaner/InformationTables.h"
#include"../Scaner/LexicalAnalyser.h"
#include"../Scaner/InformationTables.cpp"
#include"../Scaner/LexicalAnalyser.cpp"

#include"Syntax Analyser.h"

using namespace InformationTables;

int main(){

	const char* FileName = "D:\\prog\\Microsoft Visual Studio\\Projects\\Software Engineering\\Scaner\\SimpleTask.txt";
	std::vector<Token> stream = Scaner::Scan(FileName);

	std::cout << "Token stream:" << std::endl;
	for (auto &i : stream)
		i.Print(std::cout);

	IdentifierTables::Instance().Print();


	parser::parse(stream);
	parser::tree::printTree();

	return EXIT_SUCCESS;
}
