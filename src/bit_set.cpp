#include <destoer/destoer.h>

const u32 BITS_PER_ENTRY = sizeof(u32) * 8;

BitSet make_bit_set(u32 bits)
{
    BitSet bit_set;
    resize(bit_set.set,(bits / BITS_PER_ENTRY) + 1);
    zero_arr(bit_set.set);

    return bit_set;
}

bool test_bit_set(const BitSet& bit_set,u32 bit)
{
    const u32 idx = bit / BITS_PER_ENTRY;
    const u32 offset = bit & (BITS_PER_ENTRY - 1);

    return is_set(bit_set.set[idx],offset);
}

void set_bit_set(BitSet& bit_set, u32 bit)
{
    const u32 idx = bit / BITS_PER_ENTRY;
    const u32 offset = bit & (BITS_PER_ENTRY - 1);

    bit_set.count += !is_set(bit_set.set[idx],offset);
    bit_set.set[idx] = set_bit(bit_set.set[idx],offset);
}

void destroy_bit_set(BitSet& bit_set)
{
    destroy_arr(bit_set.set);
}
