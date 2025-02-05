/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Queue.cpp
 * Author: rafael.luiz.cancian
 *
 * Created on 21 de Agosto de 2018, 17:12
 */

#include "Queue.h"
#include "../../kernel/simulator/Model.h"
#include "../../kernel/simulator/Attribute.h"

#ifdef PLUGINCONNECT_DYNAMIC

extern "C" StaticGetPluginInformation GetPluginInformation() {
	return &Queue::GetPluginInformation;
}
#endif

ModelDataDefinition* Queue::NewInstance(Model* model, std::string name) {
	return new Queue(model, name);
}

Queue::Queue(Model* model, std::string name) : ModelDataDefinition(model, Util::TypeOf<Queue>(), name) {
	//controls
	_parentModel->getControls()->insert(new SimulationControlInt(
					 std::bind(&Queue::getOrderRuleInt, this),
					 std::bind(&Queue::setOrderRuleInt, this, std::placeholders::_1),
					 Util::TypeOf<Queue>(), getName(), "OrderRule"));
	_parentModel->getControls()->insert(new SimulationControlString(
					 std::bind(&Queue::getAttributeName, this),
					 std::bind(&Queue::setAttributeName, this, std::placeholders::_1),
					 Util::TypeOf<Queue>(), getName(), "AttributeName"));
}

Queue::~Queue() {
	//_parentModel->elements()->remove(Util::TypeOf<StatisticsCollector>(), _cstatNumberInQueue);
	//_parentModel->elements()->remove(Util::TypeOf<StatisticsCollector>(), _cstatTimeInQueue);
}

std::string Queue::show() {
	return ModelDataDefinition::show() +
			",waiting=" + this->_list->show();
}

void Queue::insertElement(Waiting* modeldatum) {
	if (_reportStatistics) {
		double tnow = _parentModel->getSimulation()->getSimulatedTime();
		double duration = tnow - _lastTimeNumberInQueueChanged;
		this->_cstatNumberInQueue->getStatistics()->getCollector()->addValue(_list->size(), duration); // save the OLD quantity and for how long it was there
		_lastTimeNumberInQueueChanged = tnow;
	}
	_list->insert(modeldatum);
}

void Queue::removeElement(Waiting* modeldatum) {
	if (_reportStatistics) {
		double tnow = _parentModel->getSimulation()->getSimulatedTime();
		double duration = tnow - _lastTimeNumberInQueueChanged;
		this->_cstatNumberInQueue->getStatistics()->getCollector()->addValue(_list->size(), duration); // save the OLD quantity and for how long it was there
		_lastTimeNumberInQueueChanged = tnow;
		double timeInQueue = tnow - modeldatum->getTimeStartedWaiting();
		this->_cstatTimeInQueue->getStatistics()->getCollector()->addValue(timeInQueue);
	}
	_list->remove(modeldatum);
}

void Queue::_initBetweenReplications() {
	this->_list->clear();
	_lastTimeNumberInQueueChanged = 0.0;
}

unsigned int Queue::size() {
	return _list->size();
}

Waiting* Queue::first() {
	return _list->front();
}

Waiting* Queue::getAtRank(unsigned int rank) {
	return _list->getAtRank(rank);
}

void Queue::setAttributeName(std::string _attributeName) {
	this->_attributeName = _attributeName;
}

std::string Queue::getAttributeName() const {
	return _attributeName;
}

void Queue::setOrderRule(OrderRule _orderRule) {
	this->_orderRule = _orderRule;
	//@TODO: SORT THE QUEUE BASED ON QUE RULE. Create comparators
}

Queue::OrderRule Queue::getOrderRule() const {
	return _orderRule;
}

void Queue::setOrderRuleInt(int orderRule){
	setOrderRule(static_cast<Queue::OrderRule>(orderRule));
}
int Queue::getOrderRuleInt() const{
	return static_cast<int>(_orderRule);
}


double Queue::sumAttributesFromWaiting(Util::identification attributeID) {
	double sum = 0.0;
	for (Waiting* waiting : *_list->list()) {
		sum += waiting->getEntity()->getAttributeValue(attributeID);
	}
	return sum;
}

double Queue::getAttributeFromWaitingRank(unsigned int rank, Util::identification attributeID) {
	Waiting* wait = _list->getAtRank(rank);
	if (wait != nullptr) {
		return wait->getEntity()->getAttributeValue(attributeID);
	}
	return 0.0;
}

PluginInformation* Queue::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Queue>(), &Queue::LoadInstance, &Queue::NewInstance);
	return info;
}

ModelDataDefinition* Queue::LoadInstance(Model* model, PersistenceRecord *fields) {
	Queue* newElement = new Queue(model);
	try {
		newElement->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newElement;
}

bool Queue::_loadInstance(PersistenceRecord *fields) {
	bool res = ModelDataDefinition::_loadInstance(fields);
	if (res) {
		try {
			this->_attributeName = fields->loadField("attributeName", DEFAULT.attributeName);
			this->_orderRule = static_cast<OrderRule> (fields->loadField("orderRule", static_cast<int> (DEFAULT.orderRule)));
		} catch (...) {
		}
	}
	return res;
}

void Queue::_saveInstance(PersistenceRecord *fields, bool saveDefaultValues) {
	ModelDataDefinition::_saveInstance(fields, saveDefaultValues);
	fields->saveField("orderRule", static_cast<int> (this->_orderRule), static_cast<int> (DEFAULT.orderRule), saveDefaultValues);
	fields->saveField("attributeName", this->_attributeName, DEFAULT.attributeName, saveDefaultValues);
}

bool Queue::_check(std::string* errorMessage) {
	return _parentModel->getDataManager()->check(Util::TypeOf<Attribute>(), _attributeName, "AttributeName", false, errorMessage);
}

void Queue::_createInternalAndAttachedData() {
	if (_reportStatistics) {
		if (_cstatNumberInQueue == nullptr) {
			_cstatNumberInQueue = new StatisticsCollector(_parentModel, getName() + "." + "NumberInQueue", this);
			_cstatTimeInQueue = new StatisticsCollector(_parentModel, getName() + "." + "TimeInQueue", this);
			_internalDataInsert("NumberInQueue", _cstatNumberInQueue);
			_internalDataInsert("TimeInQueue", _cstatTimeInQueue);
		}
	} else if (_cstatNumberInQueue != nullptr) {
		_internalDataClear();
	}
}

ParserChangesInformation * Queue::_getParserChangesInformation() {
	ParserChangesInformation* changes = new ParserChangesInformation();
	//changes->getProductionToAdd()->insert(...);
	//changes->getTokensToAdd()->insert(...);
	return changes;
}
