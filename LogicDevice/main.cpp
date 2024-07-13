#include <iostream>

#include "LogicDevice.h"

using namespace std;
using namespace LHJ;

int main()
{
	LD::Gauge HP = { 10.f };
	LD::Delay Delay = { 11.f };

	HP.Increase(15.f);
	Delay.Increase(7.f);

	cout << HP.Percent() << endl;
	cout << Delay.Percent() << endl;


	return 0;
}