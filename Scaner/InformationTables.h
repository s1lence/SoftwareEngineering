/****
*file:"InformationTables.h"
*synopsis: definition struct for token stream
*author: R. Neshta
*written: 06/03/17
*last modified: 05/06/17
***/

#ifndef _INFORMATIONTABLES_H_
#define _INFORMATIONTABLES_H_

#define LAST_NON_CONSTANT_ID (500)
#define LAST_NON_IDENTIFIER_ID (1000)
#define TABULATION_EQUIVALENT (4)

#include<map>
#include<set>
#include<iostream>

namespace InformationTables {

	namespace DefaultErrors {
		const char* const BadComment = "unexpected end of file found in comment ";
		const char* const BadItem = "unexpected symbol matched: ";
		const char* const BadToken = "unsuppoted symbol: ";
		const char* const UndefinedToken = "undefined item matched: ";
		const char* const Syntax = "syntax error: ";
	}

	struct Token {
		int Code;
		int Line;
		int Column;
		Token() :Code(0), Line(0), Column(0) {}
		Token(int _Code, int _Line, int _Column) :Code(_Code), Line(_Line), Column(_Column) {}
		Token(Token& Item) :Code(Item.Code), Line(Item.Line), Column(Item.Column) {}
		~Token() {}
		void Print(std::ostream &Stream) const;
	};

	enum class Attributes {
		WHITESPACE = 0, NUMBER = 1, LETTER = 2, COMMENT = 3, DELIMITER = 4, ERROR = 5
	};

	class IdentifierTables {
		std::string fileName;
		int LastIdentifierUsedId, LastConstantUsedId;
		std::map<std::string, int> Identifiers, Constants;
		IdentifierTables() :LastConstantUsedId(LAST_NON_CONSTANT_ID), LastIdentifierUsedId(LAST_NON_IDENTIFIER_ID) {}
		IdentifierTables(int) = delete;
		IdentifierTables(IdentifierTables&) = delete;
		~IdentifierTables() {}
		IdentifierTables& operator=(IdentifierTables&) = delete;
	public:
		static IdentifierTables& Instance() { static IdentifierTables _Table; return _Table; }
		void saveTranslatedFileName(const char* name){ fileName = name; }
		const std::string getTranslatedFileName()const{ return fileName; }
		int Add(std::string &NewItem, const Attributes &WhereToPut);
		bool Exist(std::string &Item, const Attributes &WhereToLook)const;
		void Print(std::ostream &Stream = std::cout, Attributes What = Attributes::WHITESPACE);
		std::map<std::string, int>::iterator Find(std::string &Key, const Attributes &WhereToLook);
		const std::pair<std::string, int> FindByCode(int &Key);
	};

	class Symbol {
		char __Symbol;
		Attributes __Attribute;
	public:
		Symbol() = default;
		Symbol(char _Symbol) :__Symbol(_Symbol), __Attribute(Attributes::WHITESPACE) {}
		Symbol(char _Symbol, Attributes _Attribute) :__Symbol(_Symbol), __Attribute(_Attribute) {}
		Symbol(Symbol &Object) :__Symbol(Object.__Symbol), __Attribute(Object.__Attribute) {}
		~Symbol() = default;
		void SetSymbolValue(const char &Symbol) { __Symbol = Symbol; }
		void SetSymbolAttribute(const Attributes &Attribute) { __Attribute = Attribute; }
		char GetSymbol(void)const { return __Symbol; }
		Attributes GetAttribute(void)const { return __Attribute; }
	};

	const std::set<char> Whitespace{
		static_cast<char>(9),
		static_cast<char>(10),
		static_cast<char>(11),
		static_cast<char>(12),
		static_cast<char>(13),
		static_cast<char>(32)
	};

	const std::set<char> Numbers{
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
	};

	const std::set<char> Letters{
		'A', 'B', 'C', 'D', 'E',
		'F', 'G', 'H', 'I', 'J',
		'K', 'L', 'M', 'N', 'O',
		'P', 'Q', 'R', 'S', 'T',
		'U', 'V', 'W', 'X', 'Y',
		'Z'
	};

	const std::map<std::string, int> KeyWords{
		{ "PROCEDURE", 301 },
		{ "BEGIN", 302 },
		{ "END", 303 },
		{ "RETURN", 304 },
		{"LABEL",305}
	};

	const std::map<char, int> Delimiters{
		{ ';', 59 }, { ',', 44 }, { '.', 46 }, { ':',58 }
	};

	const std::map<char, int> Comment{
		{ '(', 40 }, { ')', 41 }, { '*', 42 }
	};

}

#endif //_INFORMATIONTABLES_H_
