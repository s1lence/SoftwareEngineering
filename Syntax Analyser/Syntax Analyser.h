/****
*file:"Syntax Analyser.h"
*synopsis: declaration functions for syntax analysing
*author: R. Neshta
*written: 20/05/17
*last modified: 07/06/17
***/

#ifndef _SYNTAX_ANALYSER_H_
#define _SYNTAX_ANALYSER_H_

#include<vector>
#include<memory>

#include"../Scaner/InformationTables.h"


namespace parser{
	
	typedef InformationTables::IdentifierTables id_T;
	typedef InformationTables::Token token_T;
	typedef InformationTables::Symbol symbol_T;
	typedef std::vector<token_T> token_stream;
	
	static token_stream* stream;
	static token_T currentSymbol;
	static int lastReadSymbolNumber;

	struct Node{
		std::string item_;
		std::vector<std::shared_ptr<Node>> nodes_;

		Node() :item_("<empty>"){}
		Node(const char* item) :item_(item){}
		Node(Node& node) = delete;
		Node operator=(Node& node) = delete;
		~Node() = default;
	};

	//function that is entry point of parsing
	void parse(token_stream& stream);
	
	//function that reads next token from stream
	void scan();

	//function that checks EXPECTED tokens & handles results
	void checkToken(int codeRequired, const char* stringRequired, std::vector<std::shared_ptr<Node>>& vectorNodes);

	//inserts token & handles exceptions
	void insertTokenSafe(std::vector<std::shared_ptr<Node>>& vectorNodes);

	namespace tree{

		static std::shared_ptr<Node> root;

		void printTree(std::ostream& stream = std::cout);
		void printNode(std::shared_ptr<Node>&, int,std::ostream&);

	}

	namespace grammar{

		typedef std::vector<std::shared_ptr<Node>> vector_Nodes;

		void program();
		void block(vector_Nodes&);

		void declarations(vector_Nodes&);
		void procedureDeclarations(vector_Nodes&);
		
		void labelDeclarations(vector_Nodes&);
		void labelsList(vector_Nodes&);
		void labelInAction(vector_Nodes&);

		void procedure(vector_Nodes&);
		void parametersList(vector_Nodes&);
		void identifiersList(vector_Nodes&);

		void statementsList(vector_Nodes&);
		void statement(vector_Nodes&);

		void actualArguments(vector_Nodes&);
		void actualArgumentsList(vector_Nodes&);

		void variableIdentifier(vector_Nodes&);
		void procedureIdentifier(vector_Nodes&);

		void identifier(vector_Nodes&);
		void number(vector_Nodes&);

	}

	namespace defaultErrors{

		const char* const missedToken		= "expected a ";
		const char* const missedExpression	= "expected an expression ";
		const char* const endOfStream		= "Program has been terminated.";

		std::string generateErrorReport(const char* reason, std::string& token);

	}

}

#endif /*_SYNTAX_ANALYSER_H_*/
