#pragma once

#include <fstream>

class People {
private:
	int starvedToDeath = 0;
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
	int getStarvedToDeath() {
		return starvedToDeath;
	}
	void setAll(int count) {
		all = count;
	}

	void addArrived(int count) {
		arrived = count;
		all += count;
	}
	void removeDead(int count) {
		all -= count;
	}
	void setStarvedToDeath(int count) {
		starvedToDeath = count;
	}
	void readProgress(std::ifstream& f) {
		f >> starvedToDeath >> arrived >> all;
	}
};
