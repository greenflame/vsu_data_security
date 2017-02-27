#include <iostream>

#include <bitset>
#include <vector>

#include <cstdlib>
#include <time.h>

using namespace std;

uint16_t get_block(uint64_t num, int block)
{
    return (uint16_t)(num >> block * 16);
}

uint64_t join(uint16_t b0, uint16_t b1, uint16_t b2, uint16_t b3)
{
    uint64_t bb0 = (uint64_t)b0 << 16 * 0;
    uint64_t bb1 = (uint64_t)b1 << 16 * 1;
    uint64_t bb2 = (uint64_t)b2 << 16 * 2;
    uint64_t bb3 = (uint64_t)b3 << 16 * 3;

    return bb0 | bb1 | bb2 | bb3;
}

uint16_t rotl16(uint16_t n, unsigned int c)
{
    const unsigned int mask = (CHAR_BIT*sizeof(n)-1);
    c &= mask;
    return (n<<c) | (n>>( (-c)&mask ));
}

uint16_t rotr16(uint16_t n, unsigned int c)
{
    const unsigned int mask = (CHAR_BIT*sizeof(n)-1);
    c &= mask;
    return (n>>c) | (n<<( (-c)&mask ));
}

uint32_t f(uint16_t num, uint16_t key)
{
    return rotl16(num, 9) ^ (~(rotr16(key, 11) ^ num));
}

uint64_t encode(uint64_t input, vector<uint16_t> keys)
{
    uint16_t b0 = get_block(input, 0);
    uint16_t b1 = get_block(input, 1);
    uint16_t b2 = get_block(input, 2);
    uint16_t b3 = get_block(input, 3);

    for (int i = 0; i < keys.size(); i++)
    {
        uint16_t n0 = f(b0, keys[i]) ^ b1;
        uint16_t n1 = f(b0, keys[i]) ^ b2;
        uint16_t n2 = f(b0, keys[i]) ^ b3;
        uint16_t n3 = b0;

        b0 = n0;
        b1 = n1;
        b2 = n2;
        b3 = n3;
    }

    return join(b0, b1, b2, b3);
}

uint64_t decode(uint64_t input, vector<uint16_t> keys)
{
    uint16_t b0 = get_block(input, 0);
    uint16_t b1 = get_block(input, 1);
    uint16_t b2 = get_block(input, 2);
    uint16_t b3 = get_block(input, 3);

    for (int i = keys.size() - 1; i >= 0; i--)
    {
        uint16_t n0 = b3;
        uint16_t n1 = f(b3, keys[i]) ^ b0;
        uint16_t n2 = f(b3, keys[i]) ^ b1;
        uint16_t n3 = f(b3, keys[i]) ^ b2;

        b0 = n0;
        b1 = n1;
        b2 = n2;
        b3 = n3;
    }

    return join(b0, b1, b2, b3);
}

vector<uint16_t> generate_keys(int rounds)
{
    srand(time(0));
    vector<uint16_t> result;

    uint16_t key = rand();

    for (int i = 0; i < rounds - 1; i++)
    {
        key = rotr16(key, i * 8);
        result.push_back(key);
    }

    return result;
}

vector<uint64_t> cbc_encode(vector<uint64_t> data, vector<uint16_t> keys, uint64_t init_vec)
{
    vector<uint64_t> res(data.size());

    for (int i = 0; i < data.size(); i++)
    {
        uint64_t init;

        if (i == 0)
        {
            init = init_vec;
        }
        else
        {
            init = res[i - 1];
        }

        res[i] = encode(data[i] ^ init, keys);
    }

    return res;
}

vector<uint64_t> cbc_decode(vector<uint64_t> data, vector<uint16_t> keys, uint64_t init_vec)
{
    vector<uint64_t> res(data.size());

    for (int i = data.size() - 1; i >= 0; i--)
    {
        uint64_t init;

        if (i == 0)
        {
            init = init_vec;
        }
        else
        {
            init = data[i - 1];
        }

        res[i] = decode(data[i], keys) ^ init;
    }

    return res;
}

vector<uint64_t> cfb_encode(vector<uint64_t> data, vector<uint16_t> keys, uint64_t init_vec)
{
    vector<uint64_t> res(data.size());

    for (int i = 0; i < data.size(); i++)
    {
        uint64_t init;

        if (i == 0)
        {
            init = init_vec;
        }
        else
        {
            init = res[i - 1];
        }

        res[i] = encode(init, keys) ^ data[i];
    }

    return res;
}

vector<uint64_t> cfb_decode(vector<uint64_t> data, vector<uint16_t> keys, uint64_t init_vec)
{
    vector<uint64_t> res(data.size());

    for (int i = data.size() - 1; i >= 0; i--)
    {
        uint64_t init;

        if (i == 0)
        {
            init = init_vec;
        }
        else
        {
            init = data[i - 1];
        }

        res[i] = encode(init, keys) ^ data[i];
    }

    return res;
}

void print_daba_blocks(vector<uint64_t> blocks)
{
    for (int i = 0; i < blocks.size(); i++)
    {
        cout << bitset<64>(blocks[i]).to_string() << endl;
    }

    cout << endl;
}

int main()
{
    cout << "hi" << endl;

    vector<uint64_t> data_blocks;
    data_blocks.push_back(1);
    data_blocks.push_back(2);
    data_blocks.push_back(3);

    vector<uint16_t> keys = generate_keys(100);

    uint64_t init_vect = 7136458763548;

    print_daba_blocks(data_blocks);

    data_blocks = cbc_encode(data_blocks, keys, init_vect);
    print_daba_blocks(data_blocks);

    data_blocks = cbc_decode(data_blocks, keys, init_vect);
    print_daba_blocks(data_blocks);

    data_blocks = cfb_encode(data_blocks, keys, init_vect);
    print_daba_blocks(data_blocks);

    data_blocks = cfb_decode(data_blocks, keys, init_vect);
    print_daba_blocks(data_blocks);


    return 0;
}
