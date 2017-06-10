/****
*file:"TEST.cpp"
*synopsis: testing lexical analyzer functions
*author: R. Neshta
*written: 07/03/17
*last modified: 31/03/17
***/

#include<iostream>

#include"../Scaner/InformationTables.h"
#include"../Scaner/LexicalAnalyser.h"

using namespace InformationTables;

int main() {
	const char* FileName = "SimpleTask.txt";
	std::vector<Token> Stream = Scaner::Scan(FileName);

	for (auto &i : Stream)
		i.Print(std::cout);

	IdentifierTables::Instance().Print(std::cout/*, Attributes::NUMBER*/);

	return EXIT_SUCCESS;
}
