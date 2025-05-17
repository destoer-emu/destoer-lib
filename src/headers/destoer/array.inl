// insert at end as a var as a raw set of bytes into the Array
template<typename T, typename Y>
inline void push_raw_var(Array<T> &arr,const Y& v)
{
    const u32 size = sizeof(v);

    reserve(arr,size);

    // actually write in the data
    handle_write(&arr.data[count(arr)],v);
    arr.size += size;
}

// Push var on end
template<typename T>
inline void push_var(Array<T> &arr,const T& v)
{
    push_raw_var(arr,v);
}

template<typename T>
inline Array<T> clip_array(Array<T> arr, u32 size)
{
    auto out = arr;
    out.size = size * sizeof(T);

    return out;
}


template<typename T>
inline u32 count(Array<T> arr)
{
    return arr.size / sizeof(T);
}

template<typename T>
inline T pop(Array<T> &arr)
{
    const T v = arr.data[count(arr) - 1];
    arr.size -= sizeof(v);

    return v;
}

// Remove an array item by swapping it off the end.
template<typename T>
inline T remove_out_of_place(Array<T>& arr, u32 idx)
{
    const auto tmp = arr[idx];
    arr[idx] = arr[count(arr) - 1];
    arr[count(arr) - 1] = tmp;

    return pop(arr);
}

template<typename T>
inline void move_into_array(Array<T>& dst, Array<T>& src, u32 idx)
{
    push_var(dst,remove_out_of_place(src,idx));
}

template<typename T>
inline Array<T> copy_array(const Array<T>& src)
{
    Array<T> out;
    push_mem(out,src.data,src.size);
    return out;
}

template<typename T>
inline void remove_unordered_key(Array<T>& arr, const T& key)
{
    // scan for key
    for(u32 i = 0; i < count(arr); i++)
    {
        // swap to end and pop
        if(arr[i] == key)
        {
            remove_out_of_place(arr,i);
            return;
        }
    }
}


template<typename T>
inline void reserve_mem(Array<T> &arr, u32 size)
{
    arr.capacity = size;
    arr.data = (T*)realloc(arr.data,size);       
}

template<typename T>
inline void resize(Array<T> &arr, u32 len)
{
    const u32 bytes = len * sizeof(T);

    const u32 old_len = count(arr);

    reserve_mem(arr,bytes);
    arr.size = bytes;


    // default initialize the new elements
    for(u32 i = old_len; i < count(arr); i++)
    {
        arr.data[i] = {};
    }
}


// make sure there is enough left for the allocation we are doing
template<typename T>
inline void reserve(Array<T> &arr, u32 size)
{
    const u32 free_size = arr.capacity - arr.size;

    // we have room to just dump this in
    if(free_size >= size)
    {
        return;
    }

    else
    {   
        const u32 new_capacity = (arr.capacity + size) * 2;
        reserve_mem(arr,new_capacity);
    }
}


template<typename T, typename Y>
inline void write_mem(Array<Y> &arr, u32 idx, T v)
{
    u8* addr = (u8*)arr.data + idx;
    memcpy(addr,&v,sizeof(v));
}

template<typename T, typename Y>
inline T read_mem(const Array<Y> &arr, u32 idx)
{
    T v;

    const u8* addr = (u8*)arr.data + idx;
    memcpy(&v,addr,sizeof(v));

    return v;
}

// insert raw memory block into the array
template<typename T>
inline u32 push_mem(Array<T>& arr, const void* data, u32 size)
{
    reserve(arr,size);

    u8* addr = (u8*)arr.data + arr.size;
    memcpy(addr,data,size);

    const u32 offset = arr.size;

    arr.size += size;

    return offset;
}

template<typename T>
inline u32 push_mem(Array<T>& arr, const String& str)
{
    return push_mem(arr,str.buf,str.size);
}

template<typename T,typename Y>
inline u32 push_mem(Array<T>& arr, const Array<Y>& buf)
{
    return push_mem(arr,buf.data,buf.size);
}

template<typename T>
inline void destroy_arr(Array<T> &arr)
{
    if(arr.data)
    {
        free(arr.data);
        arr.data = nullptr;
    }
    arr.size = 0;
    arr.capacity = 0;
}

template<typename T>
inline void clear_arr(Array<T>& arr)
{
    arr.size = 0;
}

template<typename T>
inline void reserve_arena(ArenaAllocator& allocator,Array<T>& arr, u32 size)
{
    const u32 free_size = arr.capacity - arr.size;

    // aquire more memory from the arena
    if(free_size < size)
    {
        Arena& arena = cur_arena(allocator);

        u8* arena_ptr = (u8*)arena.buf;

        const u32 reserve = arena.size - arena.len;

        // we have enough memory left on the end of the arena to just give it more memory
        if((T*)&arena_ptr[arena.len] == &arr[arr.capacity] && size <= reserve)
        {
            reserve_end(arena,size);
            arr.capacity += size;
        }

        // get back a new chunk that is large enough and move it
        else
        {
            // allocate a new buffer large enough
            const u32 new_capacity = (arr.capacity + size) * 2;
            T* new_buf = (T*)allocate(allocator,new_capacity);

            // copy over the old data
            memcpy(new_buf,arr.data,arr.capacity);
            arr.capacity = new_capacity;

            arr.data = new_buf;
        }   
    }    
}