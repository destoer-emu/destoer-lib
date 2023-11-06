// NOTE: this only implements test operations
// we need a different repr if we wanted to query magnitude of contents efficiently

static constexpr u32 SET_DEFAULT_SIZE = 32;

template<typename T>
Set<T> make_set()
{
    Set<T> set;
    resize(set.buf,SET_DEFAULT_SIZE);

    return set;
}


template<typename T>
void destroy_set(Set<T>& set)
{
    for(u32 i = 0; i < count(set.buf); i++)
    {
        SetBucket<T>& bucket = set.buf[i];

        for(u32 j = 0; j < count(bucket); j++)
        {
            destroy_arr(bucket);
        }
    }    

    destroy_arr(set.buf);
}

template<typename T>
b32 contains(Set<T>& set, const T& key)
{
    const u32 slot = hash_slot(count(set.buf),key);

    SetBucket<T>& bucket = set.buf[slot];

    for(u32 i = 0; i < count(bucket); i++)
    {
        if(bucket[i] == key)
        {
            return true;
        }
    }

    return false;
}


template<typename T>
void rehash(Set<T>& set, u32 set_size)
{
    Array<SetBucket<T>> buf_new;
    resize(buf_new,set_size);

    for(u32 i = 0; i < count(set.buf); i++)
    {
        const SetBucket<T>& bucket = set.buf[i];

        for(u32 j = 0; j < count(bucket); j++)
        {
            const auto &data = bucket[j];

            const u32 slot = hash_slot(count(buf_new),data);
            push_var(buf_new[slot],data);
        }
    }    

    destroy_set(set);

    set.buf = buf_new;
}
// returns true if modified
template<typename T>
b32 add(Set<T>& set, const T& key)
{
    // TODO: this is very slow
    if(set.size == count(set.buf))
    {
        rehash(set,count(set.buf) * 2);
    }

    const u32 slot = hash_slot(count(set.buf),key);

    SetBucket<T>& bucket = set.buf[slot];

    for(u32 i = 0; i < count(bucket); i++)
    {
        // allready exists
        if(bucket[i] == key)
        {
            return false;
        }
    }

    // did not exist in bucket add it
    push_var(bucket,key);
    set.size++;

    return true;
}

// returns true if modified
template<typename T>
b32 set_union(Set<T>& out, const Set<T>& other)
{
    b32 modified = false;

    // add all keys to out in other
    for(u32 i = 0; i < count(other.buf); i++)
    {
        const SetBucket<T>& bucket = other.buf[i];

        for(u32 j = 0; j < count(bucket); j++)
        {
            modified |= add(out,bucket[j]);
        }
    }

    return modified;   
}