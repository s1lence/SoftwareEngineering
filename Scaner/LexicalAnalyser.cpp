/****
*file:"LexicalAnalyser.cpp"
*synopsis: definitions functions for lexical analyzer
*author: R. Neshta
*written: 01/03/17
*last modified: 05/06/17
***/


#include"LexicalAnalyser.h"
#include"InformationTables.h"

#include<iostream>
#include<fstream>

using namespace InformationTables;

Symbol Scaner::GetChar(std::streambuf& Iterator) {
	char SymbolExactly = Iterator.sbumpc();
	if (Whitespace.end() != Whitespace.find(SymbolExactly))
		return Symbol(SymbolExactly, Attributes::WHITESPACE);
	if (Letters.end() != Letters.find(SymbolExactly))
		return Symbol(SymbolExactly, Attributes::LETTER);
	if (Numbers.end() != Numbers.find(SymbolExactly))
		return Symbol(SymbolExactly, Attributes::NUMBER);
	if (Delimiters.end() != Delimiters.find(SymbolExactly))
		return Symbol(SymbolExactly, Attributes::DELIMITER);
	if (Comment.end() != Comment.find(SymbolExactly))
		return Symbol(SymbolExactly, Attributes::COMMENT);
	return Symbol(SymbolExactly, Attributes::ERROR);
}

void Scaner::HandleException(const char* Message, const char* FileName, Symbol &ExceptionItem, int DynamicLine, int DynamicColumn, const char* author) {
	std::cerr << "----------------" << std::endl;
	std::cerr << author << " error report:" << std::endl;
	std::cerr << " ERROR FOUND:\n   unhandled exception matched in \"" << FileName << "\": " << Message;
	if (Message == DefaultErrors::BadItem || Message == DefaultErrors::BadToken)
		std::cerr << "'" << ExceptionItem.GetSymbol() << "' ";
	std::cerr << "at " << DynamicLine << " line, " << DynamicColumn << " column." << std::endl;
	std::cerr << "----------------" << std::endl;
}

std::vector<Token> Scaner::Scan(const char* FileName) {
	std::string buffer = "";
	std::vector<Token> Stream;
	std::ifstream IFile(FileName);
	std::streambuf* Iterator = IFile.rdbuf();
	Iterator->pubsetbuf(nullptr, 0);
	if (Iterator->sgetc() == EOF) {
		std::cerr << "Error opening file" << std::endl;
		IFile.close();
		return Stream;
	}
	InformationTables::IdentifierTables::Instance().saveTranslatedFileName(FileName);
	Token Temporary;
	Symbol CurrentLetter = GetChar(*Iterator);
	int DynamicLine = 1, DynamicColumn = 1, TokenColumn;
	do {
		switch (CurrentLetter.GetAttribute()) {
		case Attributes::WHITESPACE:
			if (CurrentLetter.GetSymbol() == '\t')
				DynamicColumn += 3;
			do {
				if (CurrentLetter.GetSymbol() == '\n') {
					++DynamicLine;
					DynamicColumn = 0;
				}
				if (CurrentLetter.GetSymbol() == '\t')
					DynamicColumn += TABULATION_EQUIVALENT;
				CurrentLetter = GetChar(*Iterator);
				++DynamicColumn;
			} while (CurrentLetter.GetAttribute() == Attributes::WHITESPACE && (Iterator->sgetc() != EOF));
			break;
		case Attributes::NUMBER:
			TokenColumn = DynamicColumn;
			do {
				buffer.push_back(CurrentLetter.GetSymbol());
				CurrentLetter = GetChar(*Iterator);
				++DynamicColumn;
			} while (CurrentLetter.GetAttribute() == Attributes::NUMBER && (Iterator->sgetc() != EOF));
			if (CurrentLetter.GetAttribute() == Attributes::LETTER) {
				HandleException(DefaultErrors::BadItem, FileName, CurrentLetter, DynamicLine, DynamicColumn);
				break;
			}
			if (IdentifierTables::Instance().Exist(buffer, Attributes::NUMBER))
				Stream.push_back(Token(IdentifierTables::Instance().Find(buffer, Attributes::NUMBER)->second, DynamicLine, TokenColumn));
			else
				Stream.push_back(Token(IdentifierTables::Instance().Add(buffer, Attributes::NUMBER), DynamicLine, TokenColumn));
			buffer.clear();
			break;
		case Attributes::LETTER:
			TokenColumn = DynamicColumn;
			do {
				buffer.push_back(CurrentLetter.GetSymbol());
				CurrentLetter = GetChar(*Iterator);
				++DynamicColumn;
			} while ((CurrentLetter.GetAttribute() == Attributes::NUMBER || CurrentLetter.GetAttribute() == Attributes::LETTER) && (Iterator->sgetc() != EOF));
			if (KeyWords.find(buffer) == KeyWords.end())
				if (IdentifierTables::Instance().Exist(buffer, Attributes::LETTER))
					Stream.push_back(Token(IdentifierTables::Instance().Find(buffer, Attributes::LETTER)->second, DynamicLine, TokenColumn));
				else
					Stream.push_back(Token(IdentifierTables::Instance().Add(buffer, Attributes::LETTER), DynamicLine, TokenColumn));
			else
				Stream.push_back(Token(KeyWords.find(buffer)->second, DynamicLine, TokenColumn));
			buffer.clear();
			break;
		case Attributes::COMMENT:
			TokenColumn = DynamicColumn;
			if (CurrentLetter.GetSymbol() == '(') {
				buffer.push_back(CurrentLetter.GetSymbol());
				CurrentLetter = GetChar(*Iterator);
				++DynamicColumn;
				if (CurrentLetter.GetSymbol() == '*') {
					while (Iterator->sgetc() != EOF) {
						CurrentLetter = GetChar(*Iterator);
						++DynamicColumn;
						if (CurrentLetter.GetSymbol() == static_cast<char>(13)) {
							++DynamicLine;
							DynamicColumn = 0;
						}
						if (CurrentLetter.GetSymbol() == '*') {
							CurrentLetter = GetChar(*Iterator);
							++DynamicColumn;
							if (CurrentLetter.GetSymbol() == ')') {
								CurrentLetter = GetChar(*Iterator);
								++DynamicColumn;
								if (CurrentLetter.GetSymbol() == static_cast<char>(13)) {
									++DynamicLine;
									DynamicColumn = 0;
								}
								break;
							}
						}
					}
					if (Iterator->sgetc() == EOF)
						HandleException(DefaultErrors::BadComment, FileName, CurrentLetter, DynamicLine, DynamicColumn);
				}
				else
					Stream.push_back(Token(buffer[0], DynamicLine, TokenColumn));
			}
			else {
				buffer.push_back(CurrentLetter.GetSymbol());
				if (CurrentLetter.GetSymbol() == '*')
					HandleException(DefaultErrors::BadToken, FileName, CurrentLetter, DynamicLine, DynamicColumn);
				else
					Stream.push_back(Token(buffer[0], DynamicLine, TokenColumn));
				CurrentLetter = GetChar(*Iterator);
				++DynamicColumn;
			}
			buffer.clear();
			break;
		case Attributes::DELIMITER:
			buffer.push_back(CurrentLetter.GetSymbol());
			Stream.push_back(Token(buffer[0], DynamicLine, DynamicColumn));
			buffer.clear();
			CurrentLetter = GetChar(*Iterator);
			++DynamicColumn;
			break;
		case Attributes::ERROR:
			HandleException(DefaultErrors::BadToken, FileName, CurrentLetter, DynamicLine, DynamicColumn);
			CurrentLetter = GetChar(*Iterator);
			++DynamicColumn;
			break;
		}
	} while ((Iterator->sgetc() != EOF));
	IFile.close();
	return Stream;
}
