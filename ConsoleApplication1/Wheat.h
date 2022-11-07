#pragma once

#include <fstream>

class Wheat {
private:
	int collected = 0;
	int dead = 0;
	double rest = 0;
public:
	Wheat(int _rest) : rest(_rest) {

	}
	int getCollected() {
		return collected;
	}
	int getDead() {
		return dead;
	}
	double getRest() {
		return rest;
	}
	void setRest(double _rest) {
		rest = _rest;
	}
	void removeDead(int count) {
		dead = count;
		rest -= count;
	}
	void addCollected(int count) {
		collected = count;
		rest += count;
	}

	void readProgress(std::ifstream& f) {
		f >> dead >> collected >> rest;
	}
};
