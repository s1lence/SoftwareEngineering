/****
*file:"LexicalAnalyser.h"
*synopsis: declarations functions for lexical analyzer
*author: R. Neshta
*written: 01/03/17
*last modified: 05/06/17
***/

#ifndef _LEXICALANALYSER_H_
#define _LEXICALANALYSER_H_

#include"InformationTables.h"
#include<fstream>
#include<vector>

namespace Scaner{

	//function that read one character & returns his attribute value
	InformationTables::Symbol GetChar(std::streambuf& Iterator);

	//function that handle error 
	void HandleException(const char* Message,
		const char* FileName,
		InformationTables::Symbol &ExceptionItem,
		int Line,
		int Column,
		const char* author = "Lexical analyser"
		);

	//function that create token stream
	std::vector<InformationTables::Token> Scan(const char* FileName);

};

#endif //_LEXICALANALYSER_H_
