/****
*file:"InformationTables.cpp"
*synopsis: functions definitions for IdentifierTables class
*author: R. Neshta
*written: 08/03/17
*last modified: 05/06/17
***/

#include"InformationTables.h"

#include<iostream>
#include<iomanip>
#include<string>

using namespace InformationTables;

int IdentifierTables::Add(std::string &NewItem,const Attributes &WhereToPut){
	switch (WhereToPut){
	case Attributes::LETTER:
		Identifiers.insert(std::pair<std::string, int>(NewItem, ++LastIdentifierUsedId));
		return LastIdentifierUsedId;
	default:
		Constants.insert(std::pair<std::string, int>(NewItem, ++LastConstantUsedId));
		return LastConstantUsedId;
	}
}

bool IdentifierTables::Exist(std::string &Item, const Attributes &WhereToLook)const{
	switch (WhereToLook){
	case Attributes::LETTER:
		return Identifiers.find(Item) != Identifiers.end();
	default:
		return Constants.find(Item) != Constants.end();
	}
}

void IdentifierTables::Print(std::ostream &Stream,Attributes What){
	if (What == Attributes::LETTER || What == Attributes::WHITESPACE){
		Stream << "Identifiers table:\n";
		for (auto &i : Identifiers)
			Stream << '\t' << i.first << " " << i.second << std::endl;
	}
	if (What == Attributes::NUMBER || What == Attributes::WHITESPACE){
		Stream << "Constants table:\n";
		for (auto &i : Constants)
			Stream << '\t' << i.first << " " << i.second << std::endl;
	}
}

std::map<std::string, int>::iterator IdentifierTables::Find(std::string &Key,const Attributes &WhereToLook){
	switch (WhereToLook){
	case Attributes::LETTER:
		return Identifiers.find(Key);
	default:
		return Constants.find(Key);
	}
}

const std::pair<std::string, int> IdentifierTables::FindByCode(int &Key){
	if (Key > LAST_NON_IDENTIFIER_ID){
		for (auto &i : Identifiers)
			if (i.second == Key)
				return i;
	}	
	else{
		for (auto &i : Constants)
			if (i.second == Key)
				return i;
	}
	throw std::logic_error("unrecognized token matched");
}

void Token::Print(std::ostream &Stream) const{
	Stream << std::setw(5) << this->Code << " " << std::setw(5) << this->Line << " " << std::setw(5) << this->Column << std::endl;
}