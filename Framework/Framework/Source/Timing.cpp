#include "Timing.h"


CTiming::CTiming(void)
{
	min = 0;
	sec = 0;
}


CTiming::~CTiming(void)
{
}

CTiming::CTiming(int min, int sec)
{
	this->min = min;
	this->sec = sec;

	Normalising();
}


void CTiming::Normalising()
{
	if (sec >= 60)
	{
		sec -= 60;
		++min;
	}
}


void CTiming::setMin(int m)
{
	this->min = m;
}

int CTiming::getMin()
{
	return min;
}

void CTiming::setSec(double s)
{
	this->sec = s;
}

double CTiming::getSec()
{
	return sec;
}

CTiming CTiming::operator+(const CTiming& t)
{
	CTiming temp;

	temp.min = this->min + t.min;
	temp.sec = this->sec + t.sec;

	temp.Normalising();

	return temp;
}

bool CTiming::operator>(const CTiming& t)
{
	if (this->min > t.min)
	{
		return true;
	}
	else if (this->min == t.min)
	{
		if (this->sec > t.sec)
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

void CTiming::Reset()
{
	min = 0;
	sec = 0;
}
