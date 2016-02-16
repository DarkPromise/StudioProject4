#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include "Timing.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include "Record.h"


class Highscore
{
public:
	Highscore();
	~Highscore();
	void newScore(CTiming);
	int getCount(void);

	void reset();

	bool ReadFromTextFile();
	CRecord record[5];
	int writehighscore(std::string, CRecord);
	bool HighscoreCheck(CRecord recs);
	void storeNewRecord(CRecord recs);
private:
	int counter;
	std::string m_sd;
};

#endif