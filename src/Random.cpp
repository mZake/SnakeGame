#include "Random.hpp"

std::random_device Random::mDevice;
std::mt19937 Random::mGenerator(Random::mDevice());

int Random::Get(int min, int max)
{
    std::uniform_int_distribution<int> distributor(min, max);
    return distributor(mGenerator);
}
