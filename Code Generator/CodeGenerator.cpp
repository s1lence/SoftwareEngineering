/****
*file:"Code Generator.cpp"
*synopsis: definitions code generating functions
*author: R. Neshta
*written: 02/06/17
*last modified: 07/06/17
***/

#include<sstream>

#include"CodeGenerator.h"

using namespace semanticProcessor;

typedef semanticProcessor::processorTables::InformationTables _i_Tbls;

std::string _i_Tbls::findLabel(int code) {

	for (auto &i : _i_Ts::getInstance().labels_)

	if (code == i.second)
		return i.first;

	throw std::out_of_range("haven't found");

}

int _i_Tbls::findLabel(std::string string) {

	for (auto &i : _i_Ts::getInstance().labels_)

	if (string == i.first)
		return i.second;

	throw std::out_of_range("haven't found");

}

std::string _i_Tbls::findProcedure(int code) {

	for (auto &i : _i_Ts::getInstance().procedures_)

	if (code == i.second)
		return i.first;

	throw std::out_of_range("haven't found");
}

int _i_Tbls::findProcedure(std::string string) {

	for (auto &i : _i_Ts::getInstance().labels_)

	if (string == i.first)
		return i.second;

	throw std::out_of_range("haven't found");
}

void _i_Tbls::addLabel(std::string name) {

	_i_Ts::getInstance().labels_.insert(std::make_pair(name, lastLabelsIdUsed++));

}

void _i_Tbls::addProcedure(std::string name) {

	_i_Ts::getInstance().procedures_.insert(std::make_pair(name, lastProceduresIdUsed++));

}

bool _i_Tbls::isLabelExist(std::string name) {

	if (_i_Ts::getInstance().labels_.find(name) == _i_Ts::getInstance().labels_.end())
		return false;

	return true;

}

bool _i_Tbls::isProcedureExist(std::string name) {

	if (_i_Ts::getInstance().procedures_.find(name) == _i_Ts::getInstance().procedures_.end())
		return false;

	return true;

}

void _i_Tbls::print(std::ostream& stream) const{

	stream << "Procedure tables:" << std::endl;

	for (auto i : _i_Tbls::getInstance().procedures_)
		stream << i.first << " " << i.second << std::endl;

	stream << std::endl << "Label tables:" << std::endl;

	for (auto i : _i_Tbls::getInstance().labels_)
		stream << i.first << " " << i.second << std::endl;

}

void grammar::eraseStack() {

	std::stack<std::string>().swap(parameters);

}

bool grammar::isTokenProper(_shd_Node_ptr& node, const char* predicted) {

	if (node->item_ != "<error>" && node->item_ == predicted)
		return true;

	return false;
}

bool grammar::isValid(_shd_Node_ptr& node) {

	using namespace defaultErrors;

	if (node->item_ != "<error>")
		return true;

	report(errorTokenMatched, node->item_);

	return false;

}

void grammar::program(out& stream, std::shared_ptr<parser::Node>& root) {

	if (root == nullptr){

		throw std::string("Code can't be generated.");

	}

	if (isTokenProper(root->nodes_.front()->nodes_.at(2), "<parameters-list>"))
		parametersList(stream, root->nodes_.front()->nodes_.at(2)->nodes_);

	if (isValid(root->nodes_.front()->nodes_.at(1)))
		procedureIdentifier(stream, root->nodes_.front()->nodes_[1]->nodes_, false);

	int size = root->nodes_.front()->nodes_.size();

	block(stream, root->nodes_.front()->nodes_.at(size - 2)->nodes_);

}

void grammar::block(out& stream, vector_Nodes& myNodes) {

	int statementPositionPredictedAt = 1;

	if (isTokenProper(myNodes.front(), "<declarations>")) {

		declarations(stream, myNodes.front()->nodes_);
		++statementPositionPredictedAt;

	}

	if (isTokenProper(myNodes.at(statementPositionPredictedAt - 1), "BEGIN"))
		//begin label as the block qualifier
		stream << "\nBEGIN:" << std::endl;

	if (!isTokenProper(myNodes.at(statementPositionPredictedAt), "END"))
		statementsList(stream, myNodes.at(statementPositionPredictedAt)->nodes_);

	if (isTokenProper(myNodes.at(statementPositionPredictedAt + 1), "END"))
		stream << "end BEGIN" << std::endl;	//block closed now

}

void grammar::declarations(out& stream, vector_Nodes& myNodes) {

	int offset = 0;

	if (!isTokenProper(myNodes.front()->nodes_.front(), "PROCEDURE")) {

		labelDeclarations(stream, myNodes.front()->nodes_);

		++offset;

	}

	if (!isTokenProper(myNodes.front()->nodes_.at(offset), "BEGIN"))
		procedureDeclarations(stream, myNodes.at(offset)->nodes_);

}

void grammar::procedureDeclarations(out& stream, vector_Nodes& myNodes) {

	for (auto &i : myNodes)
		procedure(stream, i->nodes_);

}

void grammar::labelDeclarations(out& stream, vector_Nodes& myNodes) {

	std::ostringstream identifier;

	labelOut(identifier, myNodes.at(1)->nodes_);

	if (processorTables::_i_Ts::getInstance().isLabelExist(myNodes.at(1)->nodes_.front()->item_))
		defaultErrors::report(defaultErrors::labelRedefinition, myNodes.at(1)->nodes_.front()->item_);

	else
		processorTables::_i_Ts::getInstance().addLabel(identifier.str());

	stream << identifier.str() << ":" << std::endl;

	if (!isTokenProper(myNodes.at(2), ";"))
		labelsList(stream, myNodes.at(2)->nodes_);

}

void grammar::labelsList(out& stream, vector_Nodes& myNodes) {

	for (unsigned i = 1; i < myNodes.size(); i += 2){

		std::ostringstream identifier;

		labelOut(identifier, myNodes.at(i)->nodes_);

		if (processorTables::_i_Ts::getInstance().isLabelExist(identifier.str()))
			defaultErrors::report(defaultErrors::labelRedefinition, identifier.str());

		else
			processorTables::_i_Ts::getInstance().addLabel(identifier.str());

		stream << identifier.str() << ":" << std::endl;

	}

}

void grammar::labelInAction(out& stream, vector_Nodes& myNodes){

	std::ostringstream identifier;

	labelOut(identifier,myNodes.front()->nodes_);

	if (_i_Tbls::getInstance().isLabelExist(identifier.str()))

		stream << identifier.str() << ":";

	else

		defaultErrors::report(defaultErrors::errorTokenMatched, identifier.str());

}

void grammar::procedure(out& stream, vector_Nodes& myNodes) {

	if (isTokenProper(myNodes.at(2), "<parameters-list>"))
		parametersList(stream, myNodes.at(2)->nodes_);

	if (isValid(myNodes.at(1)))
		procedureIdentifier(stream, myNodes.at(1)->nodes_, false);

}

void grammar::parametersList(out& stream, vector_Nodes& nodes) {

	if (!parameters.empty())
		eraseStack();

	variableIdentifier(stream, nodes[1]->nodes_);
	identifiersList(stream, nodes[2]->nodes_);

}

void grammar::identifiersList(out& stream, vector_Nodes& nodes) {

	for (auto &i : nodes){

		if (!isTokenProper(i, ","))
			parameters.push(i->nodes_.front()->item_);

	}

}

void grammar::statementsList(out& stream, vector_Nodes& myNodes) {

	for (auto &i : myNodes)
		statement(stream, i->nodes_);

}

void grammar::statement(out& stream, vector_Nodes& myNodes) {

	int statementPredictedAt = 0;

	if (isTokenProper(myNodes.front(), "<label-using>")){

		labelInAction(stream, myNodes.front()->nodes_);
		
		++statementPredictedAt;

	}
	
	if (isTokenProper(myNodes.at(statementPredictedAt), "<procedure-identifier>")) {

		eraseStack();

		if (isTokenProper(myNodes.at(statementPredictedAt + 1), "<actual-arguments>"))
			actualArguments(stream, myNodes.at(statementPredictedAt + 1)->nodes_);

		procedureIdentifier(stream, myNodes.at(statementPredictedAt)->nodes_, true);

	}
	else {

		stream << "\t" << operators.find(myNodes.at(statementPredictedAt)->item_)->second << std::endl << std::endl;

	}

}

void grammar::actualArguments(out& stream, vector_Nodes& myNodes) {

	parameters.push(myNodes.at(1)->nodes_.front()->item_);

	actualArgumentsList(stream, myNodes.at(2)->nodes_);

}

void grammar::actualArgumentsList(out& stream, vector_Nodes& myNodes) {

	for (unsigned i = 1; i < myNodes.size(); i += 2)
		parameters.push(myNodes.at(i)->nodes_.front()->item_);

}

void grammar::variableIdentifier(out& stream, vector_Nodes& nodes) {

	parameters.push(nodes.front()->nodes_.front()->item_);

}

void grammar::procedureIdentifier(out& stream, vector_Nodes& node, bool isCalled) {

	std::ostringstream identifier;

	if (!isValid(node.front()))
		throw defaultErrors::programAborted;

	identifierOut(identifier, node.front()->nodes_);

	if (!isCalled && semanticProcessor::processorTables::_i_Ts::getInstance().isProcedureExist(identifier.str()))
		defaultErrors::report(defaultErrors::procedureRedefinition, identifier.str());

	else{

		if (isCalled && !semanticProcessor::processorTables::_i_Ts::getInstance().isProcedureExist(identifier.str()))
			defaultErrors::report(defaultErrors::invalidParameters, identifier.str());

		else
			semanticProcessor::processorTables::_i_Ts::getInstance().addProcedure(identifier.str());

	}


	while (isCalled && !parameters.empty()) {

		stream << "\t" << "push " << parameters.top() << std::endl;
		parameters.pop();

	}

	stream << "\t";

	if (isCalled)
		stream << "call " << identifier.str() << std::endl << std::endl;

	else
		stream << identifier.str() << " proc" << std::endl;

	eraseStack();

}

void grammar::identifierOut(out& stream, vector_Nodes& nodes) {

	stream << "_" << nodes.front()->item_;

	if (parameters.size() != 0)

		stream << "@" << parameters.size()*sizeof(int);

}

void grammar::labelOut(out& stream, vector_Nodes& nodes) {

	stream << "@" << nodes.front()->item_;

}

void semanticProcessor::generate(std::ostream& where, token_stream& tokens, std::shared_ptr<parser::Node>& root) {

	defaultErrors::stream = &tokens;

	try{

		grammar::program(where, root);

	}

	catch (const std::out_of_range& e){

		std::cerr << e.what() << std::endl;

	}
	catch (const char* const s) {

		std::cerr << s << std::endl;

	}

}

void defaultErrors::report(const char* const message, std::string& token) {

	using namespace defaultErrors;

	std::string report(message);

	if (token == "<error>") {

		std::cerr << report << std::endl;
		return;

	}

	if (message == invalidParameters) {

		report += "of function " + token;

	}
	else {

		report += token;

	}

	std::cerr << "\n\nUnhandled exception matched: " << report << "(at current line):" << std::endl;

}
