#ifndef CRECORD_H
#define CRECORD_H
#include <string>
#include "Timing.h"
#include <sstream>
class CRecord
{
public:
	CRecord(void);
	~CRecord(void);

	CTiming getTiming();
	void setTiming(CTiming);

	std::string getName();
	void setName(std::string);

	friend std::string& operator>>(std::string& line, CRecord& record)
	{
		std::string temp;
		CTiming timeTemp;

		std::istringstream iss(line);

		iss >> temp;
		timeTemp.setMin(std::stoi(temp));
		record.setTiming(timeTemp);
		temp.clear();

		iss >> temp;
		timeTemp.setSec(std::stod(temp));
		record.setTiming(timeTemp);
		temp.clear();

		std::getline(iss, temp);
		record.setName(temp);

		return line;
	};
	void update(double dt);
	void reset();
private:
	CTiming timing;
	std::string playername;

};

#endif