#pragma once

#include <fstream>

class People {
private:
	int dead = 0;
	int arrived = 0;
	int all;
public:
	People(int _all) : all(_all) {

	}
	int getAll() {
		return all;
	}
	int getArrived() {
		return arrived;
	}
	int getDead() {
		return dead;
	}
	void setAll(int count) {
		all = count;
	}

	void addArrived(int count) {
		arrived = count;
		all += count;
	}
	void removeDead(int count) {
		dead = count;
		all -= count;
	}
	void readProgress(std::ifstream& f) {
		f >> dead >> arrived >> all;
	}
};
