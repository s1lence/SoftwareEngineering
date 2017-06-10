/****
*file:"TEST.cpp"
*synopsis: testing lexical analyzer functions
*author: R. Neshta
*written: 06/03/17
*last modified: 30/05/17
***/

#include<iostream>

#include"InformationTables.h"
#include"LexicalAnalyser.h"

using namespace InformationTables;

int main() {

	const char* FileName = "D:\\prog\\Microsoft Visual Studio\\Projects\\Software Engineering\\Scaner\\SimpleTask.txt";
	std::vector<Token> stream = Scaner::Scan(FileName);

	std::cout << "Token stream:" << std::endl;
	for (auto &i : stream)
		i.Print(std::cout);

	IdentifierTables::Instance().Print();


	return EXIT_SUCCESS;
}
