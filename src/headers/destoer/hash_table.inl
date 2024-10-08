
inline u32 hash_slot(u32 size, u32 v)
{
    // TODO: impl a better integer hash func
    const u32 hash = v;
    const u32 slot = hash & (size - 1); 

    return slot;
}

template<typename Key,typename T>
inline HashTable<Key,T> make_table()
{
    HashTable<Key,T> table;
    resize(table.buf,HASH_TABLE_DEFAULT_SIZE);

    return table;
}


template<typename Key,typename T>
inline void destroy_table(HashTable<Key,T> &table)
{
    for(u32 i = 0; i < count(table.buf); i++)
    {
        HashBucket<Key,T>& bucket = table.buf[i];

        for(u32 j = 0; j < count(bucket); j++)
        {
            destroy_arr(bucket);
        }
    }    

    destroy_arr(table.buf);
}


template<typename Key,typename T>
inline T* lookup(HashTable<Key,T> &table, const Key& key)
{
    const u32 slot = hash_slot(count(table.buf),key);

    HashBucket<Key,T>& bucket = table.buf[slot];


    for(u32 i = 0; i < count(bucket); i++)
    {
        if(bucket[i].key == key)
        {
            return &bucket[i].v;
        }
    }

    return nullptr;
}


template<typename Key,typename T>
inline bool contains(HashTable<Key,T> &table, const Key& key)
{
    return lookup(table,key) != nullptr;
}

template<typename Key,typename T>
inline void rehash(HashTable<Key,T> &table, u32 table_size)
{
    Array<HashBucket<Key,T>> buf_new;
    resize(buf_new,table_size);

    for(u32 i = 0; i < count(table.buf); i++)
    {
        const HashBucket<Key,T>& bucket = table.buf[i];

        for(u32 j = 0; j < count(bucket); j++)
        {
            const auto &node = bucket[j];

            const u32 slot = hash_slot(count(buf_new),node.key);
            push_var(buf_new[slot],node);
        }
    }    

    destroy_table(table);

    table.buf = buf_new;
}

template<typename Key,typename T>
inline T* add(HashTable<Key,T> &table, const Key& key, T v)
{
    // TODO: this is very slow
    if(table.size == count(table.buf))
    {
        rehash(table,count(table.buf) * 2);
    }


    const u32 slot = hash_slot(count(table.buf),key);

    HashBucket<Key,T>& bucket = table.buf[slot];

    for(u32 i = 0; i < count(bucket); i++)
    {
        // allready exists
        if(bucket[i].key == key)
        {
            bucket[i].v = v;
            return &bucket[i].v;
        }
    }


    // did not exist in bucket add it
    HashNode<Key,T> node = {key,v};
    push_var(bucket,node);
    table.size++;

    // return the new inserted entry
    return &bucket[count(bucket) - 1].v;
}

template<typename Key,typename T>
inline void remove(HashTable<Key,T> &table, const Key& key)
{
    const u32 slot = hash_slot(count(table.buf),key);

    HashBucket<Key,T>& bucket = table.buf[slot];

    for(u32 i = 0; i < count(bucket); i++)
    {
        // found key
        if(bucket[i].key == key)
        {
            swap(bucket[i],bucket[count(bucket)]);
            pop(bucket);
        }
    }    
}


// for constant internal chained hashtables, see gen_table.cpp
template<typename T>
inline s32 lookup_internal_hashtable(const HashNode<String,T> table[],u32 size,const String& name)
{
    u32 slot = hash_slot(size,name);

    while(table[slot].key.size)
    {
        if(table[slot].key == name)
        {
            return slot;
        }


        slot = (slot + 1) & (size - 1);
    }

    return -1;    
}