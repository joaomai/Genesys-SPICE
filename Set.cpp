/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Set.cpp
 * Author: rlcancian
 * 
 * Created on 03 de Junho de 2019, 15:11
 */

#include "Set.h"

Set::Set(ElementManager* elems) : ModelElement(Util::TypeOf<Set>()) {
    _elems = elems;
}

Set::Set(ElementManager* elems, std::string name) : ModelElement(Util::TypeOf<Set>()) {
    _name = name;
    _elems = elems;
}


std::string Set::show() {
    return ModelElement::show() +
	    "";
}

void Set::setSetOfType(std::string _setOfType) {
    this->_setOfType = _setOfType;
}

std::string Set::getSetOfType() const {
    return _setOfType;
}

List<ModelElement*>* Set::getElementSet() const {
    return _elementSet;
}

PluginInformation* Set::GetPluginInformation() {
    PluginInformation* info = new PluginInformation(Util::TypeOf<Set>(), &Set::LoadInstance); 
    return info;
}

ModelElement* Set::LoadInstance(ElementManager* elems, std::map<std::string, std::string>* fields) {
    Set* newElement = new Set(elems);
    try {
	newElement->_loadInstance(fields);
    } catch (const std::exception& e) {

    }
    return newElement;
}

bool Set::_loadInstance(std::map<std::string, std::string>* fields) {
    bool res = ModelElement::_loadInstance(fields);
    if (res) {
	try {
	    //this->_attributeName = (*fields->find("attributeName")).second;
	    //this->_orderRule = static_cast<OrderRule> (std::stoi((*fields->find("orderRule")).second));
	} catch (...) {
	}
    }
    return res;
}

std::map<std::string, std::string>* Set::_saveInstance() {
    std::map<std::string, std::string>* fields = ModelElement::_saveInstance(); //Util::TypeOf<Set>());
    //fields->emplace("orderRule", std::to_string(static_cast<int> (this->_orderRule)));
    //fields->emplace("attributeName", this->_attributeName);
    return fields;
}

bool Set::_check(std::string* errorMessage) {
    bool resultAll = true;
    // resultAll |= ...
    return resultAll;
}

ParserChangesInformation* Set::_getParserChangesInformation() {
    ParserChangesInformation* changes = new ParserChangesInformation();
    //changes->getProductionToAdd()->insert(...);
    //changes->getTokensToAdd()->insert(...);
    return changes;
}

