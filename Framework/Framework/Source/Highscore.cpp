#include "Highscore.h"


Highscore::Highscore()
{
	counter = 0;
}


Highscore::~Highscore()
{
	std::cout << "Called" << std::endl;
	std::ofstream myFile("Data//highscore.txt");
	int i = 0;
	if (myFile.is_open())
	{
		while (i != 5)
		{
			myFile << record[i].getTiming().getMin() << " " << record[i].getTiming().getSec() << record[i].getName() << std::endl;
		    ++i;
		}
	}
}

void Highscore::newScore(CTiming timeringa)
{
	if (counter < 5)
	{
		record[counter].getTiming().setMin(timeringa.getMin());
		record[counter].getTiming().setSec(timeringa.getSec());
		++counter;
	}
}

int Highscore::getCount(void)
{
	return counter;
}

void Highscore::reset()
{
	for (int i = 0; i < counter; ++i)
	{
		record[counter].getTiming().setMin(0);
		record[counter].getTiming().setSec(0);
	}

}

bool Highscore::ReadFromTextFile()
{
	std::string line;
	std::ifstream myfile;
	myfile.open("Data//highscore.txt");


	if (myfile.is_open())
	{
		std::cout << "File opened" << std::endl;
		int i = 0;
		while (i != 5)
		{
			std::getline(myfile, line);
			line >> record[i];
			i += 1;
		}
	}
	else
	{
		std::cout << "File not opened" << std::endl;
		return false;
	}

	myfile.close();

	return true;
}


int Highscore::writehighscore(std::string File, CRecord rec)
{
	std::ofstream myFile(File);
	int i = 0;
	if (myFile.is_open())
	{
		while (i < 5)
		{
			myFile << rec.getTiming().getMin() << ":" << rec.getTiming().getSec() << " " << rec.getName() << std::endl;
		}
	}
	return i;
}

bool Highscore::HighscoreCheck(CRecord recs)
{
	if (recs.getTiming().getMin() == record[4].getTiming().getMin())
	{
		if (recs.getTiming().getSec() < record[4].getTiming().getSec())
		{
			return true;
		}
		else
			return false;
	}
	else if (recs.getTiming().getMin() > record[4].getTiming().getMin())
	{
		return false;
	}
	else
		return true;
}

void Highscore::storeNewRecord(CRecord recs)
{
	for (int i = 4; i >= 0; --i)
	{
		if (recs.getTiming().getMin() == record[i].getTiming().getMin())
		{
			if (recs.getTiming().getSec() >= record[i].getTiming().getSec())
			{
				for (int k = 4; k > i; --k)
				{
					record[k] = record[k - 1];
				}
				record[i] = recs;
				return;
			}
		}
		else if (recs.getTiming().getMin() > record[i].getTiming().getMin())
		{
			for (int k = 4; k > i; --k)
			{
				record[k] = record[k - 1];
			}
			record[i] = recs;
			return;
		}
	}
	for (int k = 4; k > 0; --k)
	{
		record[k] = record[k - 1];
	}

	record[0] = recs;
}