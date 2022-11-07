#pragma once

#include "People.h"
#include "Wheat.h"
#include <string>
#include <fstream>

class City {
public:
	People& people;
	Wheat& wheat;
	int wheatPerAcre = 0;
	int size = 0;
	bool plague = false;
	int acreCost = 0;
	int sowedAcres = 0;
	int year = 0;
	std::string history;

public:
	City(People& _people, Wheat& _wheat, int _size) :people(_people), wheat(_wheat), size(_size) {

	}

	People& getPeople() {
		return people;
	}
	Wheat& getWheat() {
		return wheat;
	}
	int getWheatPerAcre() {
		return wheatPerAcre;
	}
	bool wasPlague() {
		return plague;
	}
	int getSize() {
		return size;
	}
	int getAcreCost() {
		return acreCost;
	}
	int getSowedAcres() {
		return sowedAcres;
	}
	int getYear() {
		return year;
	}
	std::string getHistory() {
		return history;
	}
	void setWheatPerAcre(int count) {
		wheatPerAcre = count;
	}
	void setAcreCost(int cost) {
		acreCost = cost;
	}
	void setSize(int _size) {
		size = _size;
	}
	void setSowedAcres(int _sowedAcres) {
		sowedAcres = _sowedAcres;
	}
	void setPlague(bool _plague) {
		plague = _plague;
	}
	void readProgress(std::ifstream& f) {
		f >> year >> plague >> wheatPerAcre >> size >> acreCost >> history;
		people.readProgress(f);
		wheat.readProgress(f);
	}
	void setYear(int _year) {
		year = _year;
	}

	void addToHistory(std::string s) {
		history += s + ";";
	}
};

