#include "random.h"
#include <stdlib.h>
#include <time.h>

Random* Random::unique = 0;

Random* Random::instance()
{
    if(!unique) {
        unique = new Random();
    }
    return unique;
}

int Random::rand(int less_than_num)
{
    if(less_than_num) {
        return ::rand() % less_than_num;
    }
    return ::rand();
}

Random::Random()
{
    srand(time(NULL));
}

Random::~Random()
{

}
