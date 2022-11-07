#include <iostream>
#include <string>
#include <clocale>
#include <exception>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <vector>
#include "City.h"
#include "People.h"
#include "Wheat.h"

constexpr int ROUNDS_COUNT = 10;
constexpr int MIN_ACRE_COST = 17;
constexpr int MAX_ACRE_COST = 26;
constexpr double WHEAT_FOR_ACRE_SOW = 0.5;
constexpr int WHEAT_FOR_PERSON = 20;
constexpr int INITIAL_PEOPLE_COUNT = 100;
constexpr int INITIAL_WHEAT_COUNT = 2800;
constexpr int INITIAL_CITY_SIZE = 1000;
constexpr int MAX_ACRE_FOR_PERSON = 10;
constexpr int MIN_WHEAT_PER_ACRE = 1;
constexpr int MAX_WHEAT_PER_ACRE = 6;
constexpr int PLAGUE_PROBABIITY = 15;
constexpr double MAX_STARVED_TO_DEATH = 0.45;
const std::string PROGRESS_FILE_NAME("progress.txt");

class UserValuesDTO {
private:
	int acreBuy;
	int acreSell;
	int wheatEat;
	int acreSow;
	double wheatCount;
public:
	UserValuesDTO(int _acreBuy, int _acreSell, int _wheatEat, int _acreSow, double _wheatCount)
		: acreBuy(_acreBuy), acreSell(_acreSell), wheatEat(_wheatEat), acreSow(_acreSow), wheatCount(_wheatCount) {
	}
	int getAcreBuy() {
		return acreBuy;
	}
	int getAcreSell() {
		return acreSell;
	}
	int getWheatEat() {
		return wheatEat;
	}
	int getAcreSow() {
		return acreSow;
	}
	double getWheatCount() {
		return wheatCount;
	}
};

void printCityInformation(City& city) {
	std::cout << "Мой повелитель, соизволь поведать тебе\n"
		<< "в году " << city.getYear() + 1 << " твоего высочайшего правления\n";
	if (city.getPeople().getStarvedToDeath() > 0) {
		std::cout << city.getPeople().getStarvedToDeath() << " человек умерли с голоду, и ";
	}
	if (city.getPeople().getArrived() > 0) {
		std::cout << city.getPeople().getArrived() << " человек прибыли в наш великий город; \n";
	}
	std::cout << (city.wasPlague() ? "Чума уничтожила половину населения;\n" : "")
		<< "Население города сейчас составляет " << city.getPeople().getAll() << " человек;\n"
		<< "Мы собрали " << city.getWheat().getCollected() << " бушелей пшеницы, по " << city.getWheatPerAcre() << " бушеля с акра; \n"
		<< "Крысы истребили " << city.getWheat().getDead() << " бушелей пшеницы, оставив " << city.getWheat().getRest() << " бушеля в амбарах;\n"
		<< "Город сейчас занимает " << city.getSize() << " акров;\n"
		<< "1 акр земли стоит сейчас " << city.getAcreCost() << " бушель.\n";
}

void printProgressToFile(City& city) {
	std::ofstream f;
	f.open(PROGRESS_FILE_NAME);
	f << city.getYear() << "\n"
		<< city.wasPlague() << "\n"
		<< city.getWheatPerAcre() << "\n"
		<< city.getSize() << "\n"
		<< city.getAcreCost() << "\n"
		<< city.getHistory() << "\n"
		<< city.getPeople().getStarvedToDeath() << "\n"
		<< city.getPeople().getArrived() << "\n"
		<< city.getPeople().getAll() << "\n"
		<< city.getWheat().getDead() << "\n"
		<< city.getWheat().getCollected() << "\n"
		<< city.getWheat().getRest() << "\n"
		;
	f.close();
}

void readProgressFromFile(City& city) {
	std::ifstream f;
	f.open(PROGRESS_FILE_NAME);
	city.readProgress(f);
	f.close();
}

UserValuesDTO readUserValues(City& city) {
	int acreBuy;
	int acreSell;
	int wheatEat;
	int acreSow;

	bool conditionsOk = false;
	double wheatCount;
	std::cout << "Что пожелаешь, повелитель?\n";
	while (!conditionsOk) {
		std::cout << "Сколько акров земли повелеваешь купить?\n" << std::flush;
		std::cin >> acreBuy;
		std::cout << "Сколько акров земли повелеваешь продать?\n" << std::flush;
		std::cin >> acreSell;
		std::cout << "Сколько бушелей пшеницы повелеваешь съесть?\n" << std::flush;
		std::cin >> wheatEat;
		std::cout << "Сколько акров земли повелеваешь засеять?\n" << std::flush;
		std::cin >> acreSow;

		wheatCount =
			city.getWheat().getRest()
			+ acreSell * city.getAcreCost()
			- acreBuy * city.getAcreCost()
			- wheatEat
			- WHEAT_FOR_ACRE_SOW * acreSow
			;

		conditionsOk = (acreBuy >= 0) && (acreSell >= 0) && (wheatEat >= 0) && (acreSow >= 0)
			&& (wheatCount >= 0)
			&& (WHEAT_FOR_PERSON * city.getPeople().getAll() >= wheatEat)
			&& (acreSell <= city.getSize())
			&& (acreSow <= city.getPeople().getAll() * MAX_ACRE_FOR_PERSON)
			;

		if (!conditionsOk) {
			std::cout << "О, повелитель, пощади нас! У нас только "
				<< city.getPeople().getAll() << " человек, "
				<< city.getWheat().getRest() << " бушелей пшеницы и "
				<< city.getSize() << " акров земли!\n";
		}
	}
	return UserValuesDTO(acreBuy, acreSell, wheatEat, acreSow, wheatCount);

}

int getRandIntInRange(int _min, int _max) {
	int randNum = rand() % (_max - _min + 1) + _min;
	return randNum;
}

int calcArrived(int starvedToDeath, int wheatPerAcre, double allWheat) {
	double res = starvedToDeath / 2.0 + (5 - wheatPerAcre) * allWheat / 600 + 1;
	res = std::max(res, 0.0);
	res = std::min(res, 50.0);
	return (int)std::round(res);
}

int calcDeadWheat(double allWheat) {
	double param = 0.07;
	int res = getRandIntInRange(0, (int)std::round(param * allWheat));
	return res;
}

std::vector<std::string> split(std::string str, char del) {
	std::vector<std::string > result;
	std::string temp = "";

	for (int i = 0; i < (int)str.size(); i++) {
		if (str[i] != del) {
			temp += str[i];
		}
		else {
			result.push_back(temp);
			temp = "";
		}
	}
	if (!temp.empty()) {
		result.push_back(temp);
	}
	return result;
}

double calcStarvedToDeathStats(City& city) {
	std::vector<std::string > history = split(city.getHistory(), ';');
	double _sum = 0;
	for (std::string& element : history) {
		_sum += std::stod(element);
	}
	return _sum /= history.size();
}

void update(City& city, UserValuesDTO& userValues) {
	city.setSize(city.getSize() + userValues.getAcreBuy() - userValues.getAcreSell());
	city.setSowedAcres(userValues.getAcreSow());

	int fedPeople = userValues.getWheatEat() / WHEAT_FOR_PERSON;
	int starvedToDeath = city.getPeople().getAll() - fedPeople;
	if (starvedToDeath / (double)city.getPeople().getAll() > MAX_STARVED_TO_DEATH) {
		throw std::runtime_error("More than " + std::to_string(MAX_STARVED_TO_DEATH) + " of people starved to death.");
	}
	city.getPeople().setStarvedToDeath(starvedToDeath);
	city.getPeople().removeDead(starvedToDeath);
	city.addToHistory(std::to_string(starvedToDeath / (double)city.getPeople().getAll()));
	city.getWheat().setRest(userValues.getWheatCount());

	city.setPlague(getRandIntInRange(1, 100) <= PLAGUE_PROBABIITY);
	if (city.wasPlague()) {
		city.getPeople().removeDead((int)std::ceil(city.getPeople().getAll() / 2.0));
	}

	city.getPeople().addArrived(calcArrived(starvedToDeath, city.getWheatPerAcre(), city.getWheat().getRest()));

	int collectedWheat = city.getWheatPerAcre() * std::min(city.getSowedAcres(), city.getPeople().getAll() * MAX_ACRE_FOR_PERSON);
	city.getWheat().addCollected(collectedWheat);

	city.getWheat().removeDead(calcDeadWheat(city.getWheat().getRest()));
}

void printVerdict(double L, double P) {
	std::string message;
	if (P > 33 && L < 7) {
		message = "Плохо.\nИз - за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города.\nТеперь вы вынуждены влачить жалкое существование в изгнании\n";
	}
	else if (P > 10 && L < 9) {
		message = "Удовлетворительно.\nВы правили железной рукой, подобно Нерону и Ивану Грозному.\nНарод вздохнул с облегчением, и никто больше не желает видеть вас правителем\n";
	}
	else if (P > 3 && L < 10) {
		message = "Хорошо.\nВы справились вполне неплохо, у вас, конечно, есть недоброжелатели, но многие хотели бы увидеть вас во главе города снова\n";
	}
	else {
		message = "Отлично.\nФантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше\n";
	}
	std::cout << message;
}

int main()
{
	std::system("chcp 1251");
	srand((unsigned)time(NULL));
	People people(INITIAL_PEOPLE_COUNT);
	Wheat wheat(INITIAL_WHEAT_COUNT);
	City city(people, wheat, INITIAL_CITY_SIZE);
	std::string command;
	try {
		if (std::filesystem::exists(PROGRESS_FILE_NAME)) {
			std::cout << "Восстановить сохраненное состояние игры? (Да/Нет)\n" << std::flush;
			std::cin >> command;
			if ("Да" == command) {
				readProgressFromFile(city);
			}
			else if ("Нет" != command) {
				throw std::runtime_error("Unexpected command: " + command);
			}
		};

		for (; city.getYear() < ROUNDS_COUNT; city.setYear(city.getYear() + 1)) {
			std::cout << "Закончить игру? (Да/Нет)\n" << std::flush;
			std::cin >> command;
			if ("Да" == command) {
				printProgressToFile(city);
				return 0;
			}
			else if ("Нет" != command) {
				throw std::runtime_error("Unexpected command: " + command);
			}

			printCityInformation(city);
			std::cout << std::endl;
			int allPeople = city.getPeople().getAll();

			city.setAcreCost(getRandIntInRange(MIN_ACRE_COST, MAX_ACRE_COST));
			city.setWheatPerAcre(getRandIntInRange(MIN_WHEAT_PER_ACRE, MAX_WHEAT_PER_ACRE));

			UserValuesDTO userValues = readUserValues(city);
			update(city, userValues);
			if (city.getPeople().getAll() == 0) {
				throw std::runtime_error("All people died.\n");
			}
		}
		double L = city.getSize() / (double)city.getPeople().getAll() * 100;
		double P = calcStarvedToDeathStats(city);
		printVerdict(L, P);
	}
	catch (const std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}
}
