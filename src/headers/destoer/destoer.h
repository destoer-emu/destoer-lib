#pragma once
#include <filesystem>
#include <optional>
#include <chrono>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>


namespace destoer
{
    
// integer typedefs
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

using b32 = bool;
using b8 = u8;

using f32 = float;
using f64 = double;

enum class [[nodiscard]] dtr_res : u32
{
    ok,
    err
};

inline bool operator! (dtr_res res)
{
    return res == dtr_res::err;
}

inline void operator|= (dtr_res &v1, dtr_res v2)
{
    if(v2 == dtr_res::err)
    {
        v1 = dtr_res::err;
    }
}

#define UNUSED(X) ((void)X)

template<typename T1, typename T2>
struct Pair
{
    Pair(T1 a1, T2 a2) : v1(a1), v2(a2)
    {}

    T1 v1;
    T2 v2;
};

template<typename T1, typename T2>
void tie(const Pair<T1,T2> &pair,T1 &v1, T2 &v2)
{
    v1 = pair.v1;
    v2 = pair.v2;
}


// array
template<typename T>
struct Array
{
    T& operator [] (u32 i) 
    {
        return this->data[i];
    }

    const T& operator [] (u32 i) const
    {
        return this->data[i];
    }

    T* begin() 
    {
        return data;
    }

    T* end()
    {
        return data + (size / sizeof(T));
    }

    const T* begin() const
    {
        return data;
    }

    const T* end() const
    {
        return data + (size / sizeof(T));
    }

    T* data = nullptr;

    // in raw bytes
    u32 size = 0;
    u32 capacity = 0;
};



struct BitSet
{
    u64 count = 0;
    Array<u32> set;
};

BitSet make_bit_set(u32 bits);
bool test_bit_set(const BitSet& set,u32 bit);
void set_bit_set(BitSet& set, u32 bit);
void destroy_bit_set(BitSet& bit_set);

template<typename T, typename E>
struct [[nodiscard]] Result
{
    // Can't nest Options<Result> without this
    Result()
    {

    } 

    Result(const E& error) 
    {
        this->kind = dtr_res::err;
        this->err = error;
    }

    Result(const T& value) 
    {
        this->kind = dtr_res::ok;
        this->data = value;
    }

    const E& error() const
    {
        assert(this->kind == dtr_res::err);
        return this->err;
    }

    const T& value() const 
    {
        assert(this->kind == dtr_res::ok);
        return this->data;
    }

    operator bool() const
    {
        return this->kind != dtr_res::err;
    }

    union
    {
        T data = {};
        E err;
    };

    dtr_res kind;
};

template<typename T, typename E>
inline bool operator!(const Result<T,E>& res)
{
    return res.kind == dtr_res::err;
}

template<typename T, typename E>
inline const T operator*(const Result<T,E>& res)
{
    return res.value();
}

template<typename T, typename E>
inline T operator*(Result<T,E>& res)
{
    return res.value();
}


enum class option
{
    none,
};

template<typename T>
struct [[nodiscard]] Option
{
    Option(const T& value) {
        this->data = value;
        this->res = dtr_res::ok;
    }

    Option(option value) {
        UNUSED(value);
        this->res = dtr_res::err;
    }

    T& value() {
        assert(this->res == dtr_res::ok);
        return this->data;
    }

    const T& value() const {
        assert(this->res == dtr_res::ok);
        return this->data;
    }


    operator bool() const
    {
        return this->res != dtr_res::err;
    }


    T data;
    dtr_res res;
};

template<typename T>
inline bool operator!(const Option<T> opt)
{
    return opt.res == dtr_res::err;
}

template<typename T>
inline const T& operator*(const Option<T>& opt)
{
    return opt.value();
}

template<typename T>
inline T operator*(Option<T>& opt)
{
    return opt.value();
}



template<typename T>
inline Option<T> option_none()
{
    Option<T> opt;
    opt.res = dtr_res::err;

    return opt;
}

template<typename T>
inline Option<T> option_some(const T& value)
{
    Option<T> opt = {value,dtr_res::ok};
    return opt;
}

// dynamic string
using StringBuffer = Array<char>;

// allocator
struct ArenaAllocator;

// string
struct String;
bool string_equal(const String& str1, const String& str2);
u32 hash_string(const String& str, u32 hash);
String make_static_string(const char* str, u32 len);

// NOTE: this is designed as a 'view' string it never owns the memory it points to to
struct String
{
    String() {}

    constexpr String(const char* str) 
    {
        buf = str;

        while(str[size])
        {
            size++;
        }
    }

    char operator[] (u32 idx) const
    {
        return this->buf[idx];
    }

    bool operator== (const String& other) const
    {
        return string_equal(*this,other);
    }

    bool operator!= (const String& other) const 
    {
        return !string_equal(*this,other);
    }

    operator bool() const
    {
        return this->size != 0;
    }


    const char* buf = nullptr;

    // NOTE: there is a extra null term on the buf
    // after the size of C string compat
    u32 size = 0;
};

    
inline bool operator! (const String& str)
{
    return str.size == 0;
}

bool string_equal(const String& str1, const String& str2);

String string_offset(const String& str, u32 offset);
String string_slice(const String& str, u32 offset, u32 len);

s32 string_find(const String& str, const String& search);
bool string_contains(const String& str, const String& search);

void push_char(ArenaAllocator& allocator, StringBuffer &buffer, char v);
void push_string(StringBuffer& buffer, const String &str);
void push_string(ArenaAllocator& allocator, StringBuffer& buffer, const String &str);

String make_string(StringBuffer& buffer);
String make_string(ArenaAllocator& allocator,const char* str, u32 size);
String make_string(ArenaAllocator& allocator,const char* str);
String make_static_string(const char* str, u32 size);

u32 hash_slot(u32 size, const String& name);

String copy_string(ArenaAllocator& allocator, const String& in);
String cat_string(ArenaAllocator& allocator, const String &v1, const String& v2);
bool contains_ext(const String& str);
String extract_path(const String& filename);

Pair<StringBuffer,b32> read_str_buf(const String &filename);

template<typename T>
struct BucketIterator
{
    void skip_empty_buckets()
    {
        while(bucket_idx < count(buckets) && count(buckets[bucket_idx]) == 0)
        {
            bucket_idx++;
        }
    }

    BucketIterator(Array<Array<T>> buckets)
    {
        this->buckets = buckets;
        skip_empty_buckets();
    }

    size_t bucket_idx = 0;
    size_t data_idx = 0;
    size_t iterated = 0;
    Array<Array<T>> buckets;   

    bool operator==(const BucketIterator<T>& it) const 
    {
        return iterated == it.iterated;
    }

    BucketIterator<T>& operator++()
    {
        iterated++;
        data_idx++;

        if(data_idx >= count(buckets[bucket_idx]))
        {
            bucket_idx++;
            data_idx = 0;

            skip_empty_buckets();
        }

        return *this;
    }

    T& operator*()
    {
        auto& bucket = buckets[bucket_idx];
        return bucket[data_idx];
    }

    const T& operator*() const
    {
        auto& bucket = buckets[bucket_idx];
        return bucket[data_idx];
    }
};

template<typename T>
BucketIterator<T> end_iter(const Array<Array<T>> buf, size_t size)
{
    BucketIterator<T> it(buf);
    it.iterated = size;

    return it;
}

// hash table
template<typename Key,typename T>
struct HashNode
{
    Key key = {};
    T v = {};
};


template<typename Key,typename T>
using HashBucket = Array<HashNode<Key,T>>;

template<typename Key,typename T>
struct HashTable
{
    using HashNodeType = HashNode<Key,T>;

    BucketIterator<HashNodeType> begin()
    {
        return BucketIterator<HashNodeType>(buf);
    }

    BucketIterator<HashNodeType> end()
    {
        return end_iter(buf,size);
    }

    const BucketIterator<HashNodeType> begin() const
    {
        return BucketIterator<HashNodeType>(buf);
    }

    const BucketIterator<HashNodeType> end() const
    {
        return end_iter(buf,size);
    }


    u32 size = 0;

    // NOTE: Must be sized at a power of two
    Array<HashBucket<Key,T>> buf;
};

template<typename T>
using SetBucket = Array<T>;

template<typename T>
struct Set
{
    BucketIterator<T> begin()
    {
        return BucketIterator<T>(buf);
    }

    BucketIterator<T> end()
    {
        return end_iter(buf,size);
    }

    const BucketIterator<T> begin() const
    {
        return BucketIterator<T>(buf);
    }

    const BucketIterator<T> end() const
    {
        return end_iter(buf,size);
    }

    u32 size = 0;

    // NOTE: Must be sized at a power of two
    Array<SetBucket<T>> buf;   
};

static constexpr u32 HASH_TABLE_DEFAULT_SIZE = 256;
static constexpr s32 INVALID_HASH_SLOT = -1;

struct Arena
{
    // how much have we used?
    u32 len = 0; 

    // how much do we have total?
    u32 size = 0;

    // underyling memory
    void* buf = nullptr;
};


static constexpr u32 ARENA_ALLOC_SIZE = 32;

// for now just have a single pool
// and dont deal with it getting exhausted
struct ArenaAllocator
{
    Arena arena[ARENA_ALLOC_SIZE];
    u32 size = 0;
};

void* allocate(ArenaAllocator& allocator, u32 size);
Arena& cur_arena(ArenaAllocator& allocator);
void reserve_end(Arena& arena,u32 size);
ArenaAllocator make_allocator(u32 size);
void destroy_allocator(ArenaAllocator &allocator);

void print_line(const String& filename,u32 line);

#include <destoer/bit-ops.inl>
#include <destoer/stl-helpers.inl>

#include <destoer/array.inl>
#include <destoer/hash_table.inl>
#include <destoer/set.inl>
#include <destoer/heap_sort.inl>

// stl helpers
dtr_res read_bin(const std::string& filename, std::vector<u8>& buf);
dtr_res write_bin(const std::string &filename, std::vector<uint8_t> &buf);
std::pair<std::vector<std::string>,b32> read_dir_tree(const std::string &file_path);
std::string name_from_path(const std::string& name);
std::vector<std::string> filter_ext(const std::vector<std::string> &files,const std::string &str);

// is there a nicer way to do this?
size_t get_remaining_ifstream_size(std::ifstream &fp);


std::string remove_ext(const std::string &str);
std::string get_save_file_name(const std::string &filename);


#ifndef _MSC_VER
__attribute__((noreturn))
#endif
void unimplemented(const char *fmt, ...);

#ifndef _MSC_VER
__attribute__((noreturn))
#endif
void crash_and_burn(const char *fmt, ...);
void log(b32 cond,const char *fmt, ...);

}