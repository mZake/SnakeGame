#include <random>

class Random
{
public:
    static int Get(int min, int max);

private:
    static std::random_device mDevice;
    static std::mt19937 mGenerator;
};
