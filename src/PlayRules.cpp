#include <string>
#include "PlayRules.h"
#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <vector>
#include "windows.h"

using namespace std;

#define MAX 6
#define MIN 1

PlayRules::PlayRules()
{
}


PlayRules::~PlayRules()
{
}


int PlayRules::RandomAshtaChamma()
{
	//struct timeb timeSeed;
	//ftime(&timeSeed);
	//srand(timeSeed.time * 1000 + timeSeed.millitm);

	unsigned int tseed = time(NULL);
	srand(tseed);
	Sleep(1000);
	return result = (rand() % MAX) + MIN;
	//return 6;
}

int PlayRules::MoveWhichToken()
{
	return result = (rand() % 3) + 1;
}

int PlayRules::RandomChoice(int numChoice)
{
	return result = (rand() % numChoice);
}

void testRandomResult()
{
	PlayRules m_playRule;

	vector<int> one, two, three, four, five, six;

	one.clear();
	two.clear();
	three.clear();
	four.clear();
	five.clear();
	six.clear();

	for (int j = 0; j < 10; j++) {

		one.push_back(0);
		two.push_back(0);
		three.push_back(0);
		four.push_back(0);
		five.push_back(0);
		six.push_back(0);

		for (int i = 0; i < 300; i++) {

			int result = m_playRule.RandomAshtaChamma();
			if (result == 1) one[j] += 1;
			else if (result == 2) two[j] += 1;
			else if (result == 3) three[j] += 1;
			else if (result == 4) four[j] += 1;
			else if (result == 5) five[j] += 1;
			else if (result == 6) six[j] += 1;

			printf("cycle %d testing %d result = %d\n", j, i, result);
		}
	}

	for (int j = 0; j < one.size(); j++) {
		printf("\n\nin cycle %d\n", j);
		printf("one appear: %d times\n", one[j]);
		printf("two appear: %d times\n", two[j]);
		printf("three appear: %d times\n", three[j]);
		printf("four appear: %d times\n", four[j]);
		printf("five appear: %d times\n", five[j]);
		printf("six appear: %d times\n", six[j]);
	}

	getchar();
}
