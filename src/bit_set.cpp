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

void BitSetIterator::skip_empty_bits(BitSetIterator& iter)
{
    b32 done = false;

    while(!done)
    {
        u64 entry = iter.bit_set.set[iter.scanned_bits / BITS_PER_ENTRY];
        const u32 cur_bit = iter.scanned_bits % BITS_PER_ENTRY;

        // unset every bit inclusive of the last one we scanned
        entry = (entry >> cur_bit) << cur_bit;

        iter.scanned_bits += ffs(entry) - cur_bit;

        if(iter.scanned_bits == iter.bit_set.capacity)
        {
            done = true;
        }

        // If we have actually hit a bit then we are done
        else if(test_bit_set(iter.bit_set,iter.scanned_bits))
        {
            // We need to skip this next time.
            iter.scanned_bits += 1;
            done = true;
        }
    }
}

BitSetIterator end_iter(const BitSet& bit_set)
{
    BitSetIterator it(bit_set);
    it.scanned_bits = bit_set.capacity;

    return it;
}
