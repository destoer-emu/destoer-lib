#include <destoer/destoer.h>

BitSet make_bit_set(u32 bits)
{
    BitSet bit_set;
    resize(bit_set.set,(bits / BITS_PER_ENTRY) + 1);
    zero_arr(bit_set.set);
    bit_set.capacity = count(bit_set.set) * BITS_PER_ENTRY;
    bit_set.bits = bits;

    return bit_set;
}

bool test_bit_set(const BitSet& bit_set,u32 bit)
{
    const u32 idx = bit / BITS_PER_ENTRY;
    const u32 offset = bit % BITS_PER_ENTRY;

    return is_set(bit_set.set[idx],offset);
}

bool set_bit_set(BitSet& bit_set, u32 bit)
{
    const u32 idx = bit / BITS_PER_ENTRY;
    const u32 offset = bit % BITS_PER_ENTRY;

    const bool modifed = !is_set(bit_set.set[idx],offset);
    bit_set.count += modifed;
    bit_set.set[idx] = set_bit(bit_set.set[idx],offset);

    return modifed;
}

bool deset_bit_set(BitSet& bit_set, u32 bit)
{
    const u32 idx = bit / BITS_PER_ENTRY;
    const u32 offset = bit % BITS_PER_ENTRY;

    const bool modifed = is_set(bit_set.set[idx],offset);
    bit_set.count += modifed;
    bit_set.set[idx] = deset_bit(bit_set.set[idx],offset);

    return modifed;
}

void destroy_bit_set(BitSet& bit_set)
{
    destroy_arr(bit_set.set);
}

bool bit_set_union(BitSet& v1, const BitSet& v2)
{
    const auto start = v1.count;

    for(u32 i = 0; i < count(v2.set); i++)
    {
        const auto old = v1.set[i];
        v1.set[i] |= v2.set[i];
        v1.count += old ^ v1.set[i];
    }

    return v1.count != start; 
}

bool bit_set_difference(BitSet& v1, const BitSet& v2, const BitSet& v3)
{
    const auto start = v1.count;

    for(u32 i = 0; i < count(v1.set); i++)
    {
        const auto old = v1.set[i];
        v1.set[i] |= v2.set[i] & ~v3.set[i];
        v1.count += old ^ v1.set[i];
    }

    return v1.count != start;    
}