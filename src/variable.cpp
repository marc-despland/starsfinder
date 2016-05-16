#include "variable.h"

#include <stdlib.h>
#include <string.h>

Variable::Variable(string description, bool mandatory) {
	this->assign=false;
	this->mandatory=mandatory;
	this->description=description;
	this->value="";
}


Variable::Variable(string description, bool mandatory, string value) {
	this->assign=false;
	this->mandatory=mandatory;
	this->description=description;
	this->value=value;
}

Variable::~Variable() {}

void Variable::set(string value) {
	this->value=value;
	this->assign=true;
}

string Variable::getDescription() {
	return this->description;
}

int Variable::asInt() {
	return atoi(this->value.c_str());
}


double Variable::asDouble() {
	return atof(this->value.c_str());
}


string Variable::asString() {
	return this->value;
}

char * Variable::asChars() {
	return strdup(this->value.c_str());
}

bool Variable::asBool() {
	if (this->value.compare("1")==0) return true;
	if (this->value.compare("true")==0) return true;
	if (this->value.compare("True")==0) return true;
	if (this->value.compare("TRUE")==0) return true;
	if (this->value.compare("on")==0) return true;
	if (this->value.compare("On")==0) return true;
	if (this->value.compare("ON")==0) return true;
	if (this->value.compare("yes")==0) return true;
	if (this->value.compare("Yes")==0) return true;
	if (this->value.compare("YES")==0) return true;
	return false;
}

bool Variable::isAssign() {
	return this->assign;
}


bool Variable::isMandatory() {
	return this->mandatory;
}

bool Variable::isDefine() {
	return ((this->mandatory && this->assign) || (!this->mandatory));
}



