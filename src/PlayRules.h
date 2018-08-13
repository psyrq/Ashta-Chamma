#pragma once
#include <string>
using namespace std;

class PlayRules
{
public:
	PlayRules();
	~PlayRules();
	int RandomAshtaChamma();
	int MoveWhichToken();
	int RandomChoice(int numChoice);
private:
	int result;
};

