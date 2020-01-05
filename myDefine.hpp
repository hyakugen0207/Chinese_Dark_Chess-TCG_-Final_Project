

#include <random> // default_random_engine
#define HASH_NODE_NUM 1073741824 //2^32

using ULL = unsigned long long int;

static std::random_device my_rd;    
static std::mt19937_64 my_random_engine(my_rd());
static std::uniform_int_distribution<unsigned long long> num(0, ULLONG_MAX);
