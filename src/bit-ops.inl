
// bit operations (make sure these are inlined in all build types)
template<typename T>
inline b32 in_range(T v, T min, T max)
{
    return v >= min && v <= max;
}

inline u32 set_bit(u32 v, u32 bit)
{
    return v | (1 << bit);
}

inline u32 deset_bit(u32 v, u32 bit)
{
    return v & ~(1 << bit);
}

inline b32 is_set(u32 v, u32 bit)
{
    return (v >> bit) & 1;
}

inline b32 set_if_set(b32 b, b32 v, u32 bit)
{
    if(is_set(v,bit))
    {
        return true;
    }

    return b;
}

inline b32 deset_if_set(b32 b, b32 v, u32 bit)
{
    if(is_set(v,bit))
    {
        return false;
    }

    return b;
}

inline u32 set_bitset_if_set(u32 set, u32 v, u32 b1, u32 b2)
{
	if(is_set(v,b1))
	{
		set = set_bit(set,b2);
	}

	return set;
}

inline u32 deset_bitset_if_set(u32 set, u32 v, u32 b1, u32 b2)
{
	if(is_set(v,b1))
	{
		set = deset_bit(set,b2);
	}

	return set;
}


inline u32 popcount(u32 v)
{
    u32 count = 0;
    for(u32 i = 0; i < sizeof(v) * 8; i++)
    {
        count += is_set(v,i);
    }

    return count;
}

// TODO: make this use compilier builtins where avaible
template<typename T>
inline T bswap(T x)
{
	unsigned char *buf = reinterpret_cast<unsigned char *>(&x);
	for(size_t i = 0; i < sizeof(x) / 2; i++)
	{
		std::swap(buf[i],buf[sizeof(x)-i-1]);
	}
	memcpy(&x,buf,sizeof(x));
	return x;
}

// std::rotr and std::rotl in c++20 probs should be used
// for now https://stackoverflow.com/questions/776508/best-practices-for-circular-shift-rotate-operations-in-c

inline uint32_t rotl(uint32_t n,u32 c)
{
    const u32 mask = (CHAR_BIT*sizeof(n) - 1);  
    c &= mask;
    return (n<<c) | (n>>( (-c)&mask ));
}

inline uint32_t rotr(uint32_t n, u32 c)
{
    u32 mask = (CHAR_BIT*sizeof(n) - 1);
    c &= mask;
    return (n>>c) | (n<<( (-c)&mask ));
}


// this checks if the msb (sign) changes to something it shouldunt
// during arithmetic
template <typename T,typename U, typename X>
inline b32 did_overflow(T v1, U v2, X ans)
{
    return  is_set((v1 ^ ans) & (v2 ^ ans),(sizeof(T)*8)-1); 
}



/*
thanks yaed for suggesting use of compilier builtins
*/

template<typename T>
inline b32 sadd_overflow(T v1, T v2)
{
    using signed_type_in = typename std::make_signed<T>::type;
    auto sv1 = static_cast<signed_type_in>(v1);
    auto sv2 = static_cast<signed_type_in>(v2);

#ifdef _MSC_VER
	const auto ans = sv1 + sv2;
	return did_overflow(sv1, sv2, ans);  
#else
    return __builtin_add_overflow(sv1,sv2,&sv1);
#endif  
}


template<typename T>
inline b32 uadd_overflow(T v1, T v2)
{
    using unsigned_type_in = typename std::make_unsigned<T>::type;
    auto uv1 = static_cast<unsigned_type_in>(v1);
    auto uv2 = static_cast<unsigned_type_in>(v2);
    
#ifdef _MSC_VER
	const auto ans = uv1 + uv2;
	return ans < uv1;
#else
    return __builtin_add_overflow(uv1,uv2,&uv1);
#endif  
}


template<typename T>
inline b32 ssub_overflow(T v1,T v2)
{
    using signed_type_in = typename std::make_signed<T>::type;
    auto sv1 = static_cast<signed_type_in>(v1);
    auto sv2 = static_cast<signed_type_in>(v2);

#ifdef _MSC_VER
    const auto ans = sv1 - sv2;
    // negate 2nd operand so we can pretend
    // this is like an additon
    return did_overflow(sv1,~sv2, ans);
#else
    return __builtin_sub_overflow(sv1,sv2,&sv1);
#endif    
}


template<typename T>
inline b32 usub_overflow(T v1,T v2)
{
    using unsigned_type_in = typename std::make_unsigned<T>::type;
    auto uv1 = static_cast<unsigned_type_in>(v1);
    auto uv2 = static_cast<unsigned_type_in>(v2);
#ifdef _MSC_VER
    return uv1 < uv2;
#else
	// TODO: this needs to be inverted if we compile for ARM
    return __builtin_sub_overflow(uv1,uv2,&uv1);
#endif  
}


//https://stackoverflow.com/questions/42534749/signed-extension-from-24-bit-to-32-bit-in-c
template<typename T>
inline T sign_extend(T x, int b)
{
	T res = 1;
	res <<= b - 1;
	return (x ^ res) - res;
}

template<typename access_type>
inline void handle_write(void *buf, access_type v)
{
    memcpy(buf,&v,sizeof(access_type));
}

template<typename access_type>
inline access_type handle_read(const void *buf)
{
    access_type v;
    memcpy(&v,buf,sizeof(access_type));
    return v;
}

// sign extend types 
// NOTE: works even if input types are unsigned
template<typename OUT,typename IN>
inline OUT sign_extend_type(IN x)
{
    using signed_type_in = typename std::make_signed<IN>::type;
    const auto v = static_cast<signed_type_in>(x);

    using signed_type_out = typename std::make_signed<OUT>::type;
    return static_cast<OUT>(static_cast<signed_type_out>(v));
}

inline u32 max(u32 v1, u32 v2)
{
    return v1 > v2? v1 : v2;
}

inline constexpr u32 bit_ceil(u32 v)
{
    u32 ans = 1;
    while(ans < v)
    {
        ans *= 2;
    }

    return ans;
}