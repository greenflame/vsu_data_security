#include <iostream>

#include <bitset>
#include <vector>

#include <cstdlib>
#include <time.h>

using namespace std;

uint32_t split_left(uint64_t num)
{
    uint64_t mask = (1ll << 32) - 1;
    return (uint32_t)((num & (~mask)) >> 32);
}

uint32_t split_right(uint64_t num)
{
    uint64_t mask = (1ll << 32) - 1;
    return (uint32_t)(num & mask);
}

uint64_t join(uint32_t left, uint32_t right)
{
    return ((uint64_t)left << 32) + right;
}

uint32_t rotl32 (uint32_t n, unsigned int c)
{
  const unsigned int mask = (CHAR_BIT*sizeof(n)-1);
  c &= mask;
  return (n<<c) | (n>>( (-c)&mask ));
}

uint32_t rotr32 (uint32_t n, unsigned int c)
{
  const unsigned int mask = (CHAR_BIT*sizeof(n)-1);
  c &= mask;
  return (n>>c) | (n<<( (-c)&mask ));
}

uint32_t f(uint32_t num, uint32_t key)
{
    return rotl32(num, 9) ^ (~(rotr32(key, 11) ^ num));
}

uint64_t encode(uint64_t input, vector<uint32_t> keys)
{
    uint32_t left = split_left(input);
    uint32_t right = split_right(input);

    for (int i = 0; i < keys.size(); i++)
    {
        uint32_t newLeft = right ^ f(left, keys[i]);
        uint32_t newRight = left;

        left = newLeft;
        right = newRight;
    }

    return join(left, right);
}

uint64_t decode(uint64_t input, vector<uint32_t> keys)
{
    uint32_t left = split_left(input);
    uint32_t right = split_right(input);

    for (int i = keys.size() - 1; i >= 0; i--)
    {
        uint32_t newLeft = right;
        uint32_t newRight = left ^ f(right, keys[i]);

        left = newLeft;
        right = newRight;
    }

    return join(left, right);
}

vector<uint32_t> generate_keys(int rounds)
{
    srand(time(0));
    vector<uint32_t> result;

    uint32_t key = rand();

    for (int i = 0; i < rounds - 1; i++)
    {
        key = rotr32(key, i * 8);
        result.push_back(key);
    }

    return result;
}

int main()
{
    cout << "hi" << endl;

    vector<uint32_t> keys = generate_keys(100);

    uint64_t a = (1ll << 32) + (1ll << 1);
    cout << bitset<64>(a).to_string() << endl;
    a = encode(a, keys);
    cout << bitset<64>(a).to_string() << endl;
    a = decode(a, keys);
    cout << bitset<64>(a).to_string() << endl;


    return 0;
}
