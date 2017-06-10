/****
*file:"Syntax Analyser.h"
*synopsis: definitions functions for syntax analysing
*author: R. Neshta
*written: 21/05/17
*last modified: 07/06/17
***/

#include<string>

#include"Syntax Analyser.h"
#include"../Scaner/LexicalAnalyser.h"

#define ERROR_GENERAL(MESSAGE) Scaner::HandleException(::generateErrorReport(MESSAGE,std::string()).c_str(), id_T::Instance().getTranslatedFileName().c_str(),symbol_T(), currentSymbol.Line, currentSymbol.Column,"Syntax analyser")
#define ERROR_MISSED_TOKEN(WHAT_MISSED) Scaner::HandleException(::generateErrorReport(defaultErrors::missedToken,std::string(WHAT_MISSED)).c_str(), id_T::Instance().getTranslatedFileName().c_str(),symbol_T(), currentSymbol.Line, currentSymbol.Column,"Syntax analyser")
#define CREATE_NEW_NODE(STRING) std::make_shared<Node>(STRING)
#define GET_ITEM_BY_CODE(CODE) id_T::Instance().FindByCode(CODE).first.c_str()

using namespace parser;
using namespace defaultErrors;

void parser::parse(token_stream& stream){
	lastReadSymbolNumber = 0;
	parser::stream = &stream;
	try{
		scan();
		grammar::program();
	}
	catch (const char* error){
		std::cerr << error << std::endl;
	}
	catch (std::out_of_range&){
		ERROR_GENERAL(defaultErrors::missedExpression);
		std::cerr << defaultErrors::endOfStream << std::endl;
	}
}

void parser::scan(){
	currentSymbol = stream->at(lastReadSymbolNumber++);
}

void parser::checkToken(int codeRequired, const char* stringRequired, std::vector<std::shared_ptr<Node>>& vectorNodes){
	if (currentSymbol.Code != codeRequired){
		ERROR_MISSED_TOKEN(stringRequired);
		vectorNodes.push_back(CREATE_NEW_NODE("<error>"));
	}
	else
		vectorNodes.push_back(CREATE_NEW_NODE(stringRequired));
}

void parser::insertTokenSafe(std::vector<std::shared_ptr<Node>>& pushHere){
	try{
		pushHere.push_back(CREATE_NEW_NODE(GET_ITEM_BY_CODE(currentSymbol.Code)));
	}
	catch (const char*){
		pushHere.push_back(CREATE_NEW_NODE("<error>"));
		ERROR_GENERAL(defaultErrors::missedExpression);
	}
	catch (std::logic_error& error){
		pushHere.push_back(CREATE_NEW_NODE("<error>"));
		ERROR_GENERAL(error.what());
	}
}

void tree::printTree(std::ostream& stream){

	int currentDepth = 0;
	
	if (tree::root == nullptr)
		stream << "Tree does not exist." << std::endl;

	else{

		stream << "Parse tree:" << std::endl;

		printNode(tree::root, currentDepth, stream);

	}

}

void tree::printNode(std::shared_ptr<Node>& node, int depth, std::ostream& stream){
	for (int i = 0; i < depth; ++i)
		stream << "---|";
	stream << node->item_ << std::endl;
	for (auto &i : node->nodes_)
		printNode(i, depth + 1, stream);
}

void grammar::program(){
	tree::root = CREATE_NEW_NODE("<signal-program>");
	tree::root->nodes_.push_back(CREATE_NEW_NODE("<program>"));
	checkToken(301, "PROCEDURE", tree::root->nodes_.back()->nodes_);
	scan();
	grammar::procedureIdentifier(tree::root->nodes_.back()->nodes_);
	scan();
	if (currentSymbol.Code != 59){
		grammar::parametersList(tree::root->nodes_.back()->nodes_);
		scan();
	}
	checkToken(59, ";", tree::root->nodes_.back()->nodes_);
	scan();
	grammar::block(tree::root->nodes_.back()->nodes_);
	scan();
	checkToken(46, ".", tree::root->nodes_.back()->nodes_);
}

void grammar::block(vector_Nodes& pushHere){
	pushHere.push_back(CREATE_NEW_NODE("<block>"));
	if (currentSymbol.Code != 302)
		grammar::declarations(pushHere.back()->nodes_);
	checkToken(302, "BEGIN", pushHere.back()->nodes_);
	scan();
	if (currentSymbol.Code != 303)
		grammar::statementsList(pushHere.back()->nodes_);
	checkToken(303, "END", pushHere.back()->nodes_);
}

void grammar::declarations(vector_Nodes& pushHere){
	pushHere.push_back(CREATE_NEW_NODE("<declarations>"));
	if(currentSymbol.Code == 305){
		labelDeclarations(pushHere.back()->nodes_);
		scan();
	}
	procedureDeclarations(pushHere.back()->nodes_);
}

void grammar::procedureDeclarations(vector_Nodes& pushHere){
	pushHere.push_back(CREATE_NEW_NODE("<procedure-declarations>"));
	while (currentSymbol.Code == 301){
		procedure(pushHere.back()->nodes_);
		scan();
	}
}

void grammar::labelDeclarations(vector_Nodes& pushHere) {
	pushHere.push_back(CREATE_NEW_NODE("<label-declarations>"));
	checkToken(305,"LABEL",pushHere.back()->nodes_);
	scan();
	number(pushHere.back()->nodes_);
	scan();
	if(currentSymbol.Code == 44)
		labelsList(pushHere.back()->nodes_);
	checkToken(59,";",pushHere.back()->nodes_);
}

void grammar::labelsList(vector_Nodes& pushHere) {
	pushHere.push_back(CREATE_NEW_NODE("<labels-list>"));
	while (currentSymbol.Code == 44) {
		checkToken(44, ",", pushHere.back()->nodes_);
		scan();
		number(pushHere.back()->nodes_);
		scan();
	}
}

void grammar::labelInAction(vector_Nodes& pushHere){
	
	pushHere.push_back(CREATE_NEW_NODE("<label-using>"));

	number(pushHere.back()->nodes_);
	
	scan();

	checkToken(58, ":", pushHere.back()->nodes_);

}

void grammar::procedure(vector_Nodes& pushHere){
	pushHere.push_back(CREATE_NEW_NODE("<procedure>"));
	checkToken(301, "PROCEDURE", pushHere.back()->nodes_);
	scan();
	procedureIdentifier(pushHere.back()->nodes_);
	scan();
	if (currentSymbol.Code == 40){
		parametersList(pushHere.back()->nodes_);
		scan();
	}
	checkToken(59, ";", pushHere.back()->nodes_);
}

void grammar::parametersList(vector_Nodes& pushHere){
	pushHere.push_back(CREATE_NEW_NODE("<parameters-list>"));;
	checkToken(40, "(", pushHere.back()->nodes_);
	scan();
	variableIdentifier(pushHere.back()->nodes_);
	scan();
	if (currentSymbol.Code == 44)
		identifiersList(pushHere.back()->nodes_);
	checkToken(41, ")", pushHere.back()->nodes_);
}

void grammar::identifiersList(vector_Nodes& pushHere){
	pushHere.push_back(CREATE_NEW_NODE("<identifiers-list>"));
	while (currentSymbol.Code == 44){
		checkToken(44, ",", pushHere.back()->nodes_);
		scan();
		variableIdentifier(pushHere.back()->nodes_);
		scan();
	}
}

void grammar::statementsList(vector_Nodes& pushHere){
	pushHere.push_back(CREATE_NEW_NODE("<statements-list>"));
	statement(pushHere.back()->nodes_);
	scan();
	while (currentSymbol.Code != 303){
		statement(pushHere.back()->nodes_);
		scan();
	}
}

void grammar::statement(vector_Nodes& pushHere){

	pushHere.push_back(CREATE_NEW_NODE("<statement>"));

	if (currentSymbol.Code < 1001){

		labelInAction(pushHere.back()->nodes_);

		scan();

	}

	if (currentSymbol.Code == 304){
		pushHere.back()->nodes_.push_back(CREATE_NEW_NODE("RETURN"));
		scan();
	}
	else{

		procedureIdentifier(pushHere.back()->nodes_);
		scan();
		if (currentSymbol.Code != 59){
			actualArguments(pushHere.back()->nodes_);
			scan();
		}
	}
	checkToken(59, ";", pushHere.back()->nodes_);
}

void grammar::actualArguments(vector_Nodes& pushHere){
	pushHere.push_back(CREATE_NEW_NODE("<actual-arguments>"));
	checkToken(40, "(", pushHere.back()->nodes_);
	scan();
	number(pushHere.back()->nodes_);
	scan();
	if (currentSymbol.Code != 41)
		actualArgumentsList(pushHere.back()->nodes_);
	checkToken(41, ")", pushHere.back()->nodes_);
}

void grammar::actualArgumentsList(vector_Nodes& pushHere){
	pushHere.push_back(CREATE_NEW_NODE("<actual-arguments-list>"));
	while (currentSymbol.Code == 44){
		checkToken(44, ",", pushHere.back()->nodes_);
		scan();
		number(pushHere.back()->nodes_);
		scan();
	}
}

void grammar::variableIdentifier(vector_Nodes& pushHere){
	pushHere.push_back(CREATE_NEW_NODE("<variable-identifier>"));
	identifier(pushHere.back()->nodes_);
}

void grammar::procedureIdentifier(vector_Nodes& pushHere){
	pushHere.push_back(CREATE_NEW_NODE("<procedure-identifier>"));
	identifier(pushHere.back()->nodes_);
}

void grammar::identifier(vector_Nodes& pushHere){
	if (currentSymbol.Code > 1000) {
		pushHere.push_back(CREATE_NEW_NODE("<identifier>"));
		insertTokenSafe(pushHere.back()->nodes_);
	}
	else {
		pushHere.push_back(CREATE_NEW_NODE("<error>"));
		ERROR_GENERAL(defaultErrors::missedExpression);
	}
}

void grammar::number(vector_Nodes& pushHere){
	if(currentSymbol.Code<1001){
		pushHere.push_back(CREATE_NEW_NODE("<unsigned-integer>"));
		insertTokenSafe(pushHere.back()->nodes_);
	}
	else {
		pushHere.push_back(CREATE_NEW_NODE("<error>"));
		ERROR_GENERAL(defaultErrors::missedExpression);
	}
}

std::string defaultErrors::generateErrorReport(const char* reason, std::string& token){
	std::string message(InformationTables::DefaultErrors::Syntax);
	if (reason == missedToken)
		message += reason + token + " ";
	else
		message += reason;
	return message;
}
