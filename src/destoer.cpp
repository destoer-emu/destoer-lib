#include "destoer.h"



namespace destoer
{

constexpr char path_separator = std::filesystem::path::preferred_separator;


template<typename access_type>
access_type handle_read(const void *buf)
{
    access_type v;
    memcpy(&v,buf,sizeof(access_type));
    return v;
}


void print_line(const String& filename,u32 line)
{
    // this is slow, but we are about to terminate anyways
    // when this is used
    FILE *fp = fopen(filename.buf,"r");

    if(!fp)
    {
        printf("could not open file %s for error printing\n",filename.buf);
    }

    char buf[512] = {0};
    for(u32 i = 0; i < line; i++)
    {   
        if(!fgets(buf,sizeof(buf) - 2,fp))
        {
            break;
        }
    }

    fclose(fp);

    printf("%s\n",buf);    
}


template<typename access_type>
void handle_write(void *buf, access_type v)
{
    memcpy(buf,&v,sizeof(access_type));
}

template<typename T>
bool in_range(T v, T min, T max)
{
    return v >= min && v <= max;
}

u32 set_bit(u32 v, u32 bit)
{
    return v | (1 << bit);
}

u32 deset_bit(u32 v, u32 bit)
{
    return v & ~(1 << bit);
}

bool is_set(u32 v, u32 bit)
{
    return (v >> bit) & 1;
}

u32 popcount(u32 v)
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

//https://stackoverflow.com/questions/42534749/signed-extension-from-24-bit-to-32-bit-in-c
template<typename T>
T sign_extend(T x, int b)
{
	T res = 1;
	res <<= b - 1;
	return (x ^ res) - res;
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

u32 max(u32 v1, u32 v2)
{
    return v1 > v2? v1 : v2;
}

constexpr u32 bit_ceil(u32 v)
{
    u32 ans = 1;
    while(ans < v)
    {
        ans *= 2;
    }

    return ans;
}


#ifndef _MSC_VER
__attribute__((noreturn))
#endif
void unimplemented(const char *fmt, ...)
{
    printf("unimplemented: ");
    va_list args; 
    va_start(args, fmt);
    vprintf(fmt,args);
    putchar('\n');
    va_end(args);
    exit(1);
}

#ifndef _MSC_VER
__attribute__((noreturn))
#endif
void crash_and_burn(const char *fmt, ...)
{
    printf("panic: ");
    va_list args; 
    va_start(args, fmt);
    vprintf(fmt,args);
    va_end(args);
    putchar('\n');
    exit(1);     
}


void log(b32 cond,const char *fmt, ...)
{
    if(cond)
    {
        va_list args; 
        va_start(args, fmt);
        vprintf(fmt,args);
        va_end(args);
    }
}


#include "stl-helpers.cpp"
#include "alloc.cpp"
#include "array.cpp"
#include "string.cpp"
#include "hashtable.cpp"
#include "heap_sort.cpp"

// read entire file into a string
Array<char> read_file(const String &filename)
{
    FILE* fp = fopen(filename.buf,"rb");

    Array<char> buf;

    // file is invalid dont bother
    if(!fp)
    {
        return buf;
    }

    // get the file len
    fseek(fp, 0, SEEK_END);
    const u32 len = ftell(fp); 
    fseek(fp, 0, SEEK_SET); 

    // allocate an appriopately sized buffer
    // and read the whole file out
    resize(buf,len + 1);
    u32 read = fread(buf.data,len,1,fp);
    // silence nag
    UNUSED(read);

    buf[len] = '\0';

    fclose(fp);
    return buf;
}

}
