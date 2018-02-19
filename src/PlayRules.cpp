#include <string>
#include "PlayRules.h"
#include <time.h>
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
	//return result = (rand() % MAX) + MIN;
	return 6;
}
