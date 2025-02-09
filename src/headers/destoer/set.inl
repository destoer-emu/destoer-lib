// NOTE: this only implements test operations
// we need a different repr if we wanted to query magnitude of contents efficiently

static constexpr u32 SET_DEFAULT_SIZE = 32;

template<typename T>
inline Set<T> make_set()
{
    Set<T> set;
    resize(set.buf,SET_DEFAULT_SIZE);

    return set;
}


template<typename T>
inline void destroy_set(Set<T>& set)
{
    for(auto& bucket : set.buf)
    {
        destroy_arr(bucket);
    }    

    set.size = 0;
    destroy_arr(set.buf);
}

template<typename T>
inline b32 contains(Set<T>& set, const T& key)
{
    const u32 slot = hash_slot(count(set.buf),key);

    SetBucket<T>& bucket = set.buf[slot];

    for(const auto& node : bucket)
    {
        if(node == key)
        {
            return true;
        }
    }

    return false;
}


template<typename T>
inline void rehash(Set<T>& set, u32 set_size)
{
    Array<SetBucket<T>> buf_new;
    resize(buf_new,set_size);

    for(auto& bucket : set.buf)
    {
        for(auto& data : bucket)
        {
            const u32 slot = hash_slot(count(buf_new),data);
            push_var(buf_new[slot],data);
        }
    }    

    destroy_set(set);

    set.buf = buf_new;
}
// returns true if modified
template<typename T>
inline b32 add(Set<T>& set, const T& key)
{
    // TODO: this is very slow
    if(set.size == count(set.buf))
    {
        rehash(set,count(set.buf) * 2);
    }

    const u32 slot = hash_slot(count(set.buf),key);

    SetBucket<T>& bucket = set.buf[slot];

    for(auto& node : bucket)
    {
        // allready exists
        if(node == key)
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
inline b32 set_union(Set<T>& out, const Set<T>& other)
{
    b32 modified = false;

    // add all keys to out in other
    for(auto& bucket : other.buf)
    {
        for(auto& node : bucket)
        {
            modified |= add(out,node);
        }
    }

    return modified;   
}