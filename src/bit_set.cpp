#include <destoer/destoer.h>

void grow_bit_set(BitSet& bit_set, u32 bits)
{
    resize(bit_set.set,(bits / BITS_PER_ENTRY) + 1);
    bit_set.capacity = count(bit_set.set) * BITS_PER_ENTRY;
    bit_set.bits = bits;
}

BitSet make_bit_set(u32 bits)
{
    BitSet bit_set;
    grow_bit_set(bit_set,bits);

    return bit_set;
}

void clear_bit_set(BitSet& bit_set)
{
    zero_arr(bit_set.set);
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

bool set_bit_set_grow(BitSet& bit_set, u32 bit)
{
    const u32 idx = bit / BITS_PER_ENTRY;
    if(idx >= count(bit_set.set))
    {
        resize(bit_set.set,idx);
    }

    return set_bit_set(bit_set,bit);
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

s32 bit_set_last(const BitSet& bit_set)
{
    if(bit_set.count == 0)
    {
        return -1;
    }

    for(s32 i = count(bit_set.set) - 1; i >= 0; i--)
    {
        const auto bit = fls(bit_set.set[i]);
        if(bit != FLS_EMPTY)
        {
            return (i * BITS_PER_ENTRY) + bit;
        }
    }

    return -1;
}

s32 bit_set_first(const BitSet& bit_set)
{
    if(bit_set.count == 0)
    {
        return -1;
    }

    for(u32 i = 0; i < count(bit_set.set); i++)
    {
        const auto bit = ffs(bit_set.set[i]);
        if(bit != FFS_EMPTY)
        {
            return (i * BITS_PER_ENTRY) + bit;
        }
    }

    return -1;
}


bool test_bit_set_intersection(const BitSet& v1, const BitSet& v2)
{
    for(u32 i = 0; i < count(v1.set); i++)
    {
        if(v1.set[i] & v2.set[i])
        {
            return true;
        }
    }

    return false;
}

BitSetIterator end_iter(const BitSet& bit_set)
{
    BitSetIterator it(bit_set);
    it.scanned_bits = bit_set.capacity;

    return it;
}
