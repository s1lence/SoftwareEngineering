/****
*file:"Code Generator.h"
*synopsis: declarations code generating functions
*author: R. Neshta
*written: 02/06/17
*last modified: 07/06/17
***/

#include<map>
#include<vector>
#include<memory>
#include<stack>
#include<string>

#include"../Syntax Analyser/Syntax Analyser.h"
#include"../Scaner/InformationTables.h"
#include"../Scaner/LexicalAnalyser.h"

#pragma once

#ifndef _CODE_GENERATOR_H_
#define _CODE_GENERATOR_H_

namespace semanticProcessor {

	typedef std::map<std::string, int> Table;
	typedef InformationTables::IdentifierTables id_T;
	typedef InformationTables::Token token_T;
	typedef InformationTables::Symbol symbol_T;
	typedef std::vector<token_T> token_stream;


	const std::map<std::string, std::string> operators{
		{ "RETURN", "ret" }
	};

	namespace processorTables {

		class InformationTables;
		typedef InformationTables _i_Ts;


		class InformationTables {

			Table labels_;
			Table procedures_;

			int lastLabelsIdUsed;
			int lastProceduresIdUsed;

			InformationTables() :lastLabelsIdUsed(0), lastProceduresIdUsed(500) {}
			InformationTables(Table&) = delete;
			InformationTables(InformationTables&) = delete;
			~InformationTables() {}

		public:
			static _i_Ts& getInstance() { static _i_Ts item; return item; }

			int findLabel(std::string);
			std::string findLabel(int);

			int findProcedure(std::string);
			std::string findProcedure(int);

			void addLabel(std::string);
			void addProcedure(std::string);

			bool isLabelExist(std::string);
			bool isProcedureExist(std::string);

			void print(std::ostream&)const;

		};

	}

	namespace grammar {

		static std::stack<std::string> parameters;

		void eraseStack();

		typedef std::shared_ptr<parser::Node> _shd_Node_ptr;
		typedef std::vector<_shd_Node_ptr> vector_Nodes;
		typedef std::ostream out;

		bool isTokenProper(_shd_Node_ptr&, const char*);
		bool isValid(_shd_Node_ptr&);

		void program(out&, std::shared_ptr<parser::Node>&);
		void block(out&, vector_Nodes&);

		void declarations(out&, vector_Nodes&);
		void procedureDeclarations(out&, vector_Nodes&);

		void labelDeclarations(out&, vector_Nodes&);
		void labelsList(out&, vector_Nodes&);
		void labelInAction(out&, vector_Nodes&);

		void procedure(out&, vector_Nodes&);
		void parametersList(out&, vector_Nodes&);
		void identifiersList(out&, vector_Nodes&);

		void statementsList(out&, vector_Nodes&);
		void statement(out&, vector_Nodes&);

		void actualArguments(out&, vector_Nodes&);
		void actualArgumentsList(out&, vector_Nodes&);

		void variableIdentifier(out&, vector_Nodes&);
		void procedureIdentifier(out&, vector_Nodes&, bool);

		void identifierOut(out&, vector_Nodes&);
		void labelOut(out&, vector_Nodes&);

	}

	void generate(std::ostream&, token_stream&, std::shared_ptr<parser::Node>&);

	namespace defaultErrors {

		void report(const char* const, std::string&);

		const char* const invalidParameters = "given list of parameters does not matches any of definitions ";
		const char* const labelRedefinition = "redefinition existing label ";
		const char* const procedureRedefinition = "redefinition existing procedure ";
		const char* const errorTokenMatched = "Unhandled exception: invalid identifier matched:";
		const char* const programAborted = "Program terminated.";

		static token_stream* stream;

	}

}

#endif // _CODE_GENERATOR_H_
