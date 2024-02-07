
bool string_equal(const String& str1, const String& str2)
{
    if(str1.size != str2.size)
    {
        return false;
    }

    for(u32 i = 0; i < str1.size; i++)
    {
        if(str1[i] != str2[i])
        {
            return false;
        }
    }

    return true;    
}

String string_offset(const String& str, u32 offset)
{
    String out;

    out.buf = &str.buf[offset];
    out.size -= offset;

    return out;
}

String string_slice(const String& str, u32 offset, u32 len)
{
    String out;

    out.buf = &str.buf[offset];

    if((str.size - offset) >= len)
    {
        out.size = len;
    }

    else
    {
        out.size = 0;
    }

    return out;
}

s32 string_find(const String& str, const String& search)
{
    if(search.size == 0)
    {
        return -1;
    }

    for(u32 i = 0; i < str.size; i++)
    {
        // initial hit
        if(str[i] == search[0])
        {
            const auto clip = string_slice(str,i,search.size);

            if(clip.size != 0 && string_equal(clip,search))
            {
                return i;
            }
        }
    }

    return -1;
}

bool string_contains(const String& str, const String& search)
{
    return string_find(str,search) != -1;
}


void push_char(ArenaAllocator& allocator, StringBuffer &buffer, char v)
{
    reserve_arena(allocator,buffer,sizeof(v));
    buffer[buffer.size++] = v;
}

void push_string(StringBuffer& buffer, const String &str)
{
    reserve(buffer,str.size);
    memcpy(&buffer.data[buffer.size],str.buf,str.size);

    buffer.size += str.size;
}

void push_string(ArenaAllocator& allocator, StringBuffer& buffer, const String &str)
{
    reserve_arena(allocator,buffer,str.size);
    memcpy(&buffer.data[buffer.size],str.buf,str.size);

    buffer.size += str.size;
}

// NOTE: expects array to have a null term at the end
String make_string(StringBuffer& buffer)
{
    String string;

    string.buf = buffer.data;
    string.size = count(buffer) - 1;

    return string;
}

String make_string(ArenaAllocator& allocator,const char* str, u32 size)
{
    String string;

    char* ptr  = (char*)allocate(allocator,size + 1);
    memcpy(ptr,str,size);

    // null term the string
    ptr[size] = '\0';

    string.buf = ptr;
    string.size = size;

    return string;
}

String make_string(ArenaAllocator& allocator,const char* str)
{
    return make_string(allocator,str,strlen(str));
}

// make a string from a static
String make_static_string(const char* str, u32 size)
{
    String string;

    string.buf = str;
    string.size = size;

    return string;
}




// TODO: is splatting chars across the hash with a random number good enough?
static constexpr u32 HASH_MAGIC = 0x02579275;

u32 hash_string(const String& str, u32 hash)
{
    for(u32 i = 0; i < str.size; i++)
    {
        hash = (hash * HASH_MAGIC) ^ str[i];
    }

    return hash; 
}


u32 hash_slot(u32 size, const String& name)
{
    const u32 hash = hash_string(name,HASH_MAGIC);
    const u32 slot = hash & (size - 1);

    return slot;
}


String copy_string(ArenaAllocator& allocator, const String& in)
{
    char* ptr  = (char*)allocate(allocator,in.size + 1);
    memcpy(ptr,in.buf,in.size);

    ptr[in.size] = '\0';

    String string;

    string.buf = ptr;
    string.size = in.size;

    return string;
}

String cat_string(ArenaAllocator& allocator, const String &v1, const String& v2)
{
    const u32 size = v1.size + v2.size;

    char* ptr = (char*)allocate(allocator,size + 1);
    memcpy(ptr,v1.buf,v1.size);
    memcpy(&ptr[v1.size],v2.buf,v2.size);

    ptr[size] = '\0';

    String string;
    
    string.buf = ptr;
    string.size = size;

    return string;
}


bool contains_ext(const String& str) 
{   
    return strchr(str.buf,'.') != NULL;
}    

// NOTE: this function is in place
String extract_path(const String& filename)
{
    // find last /
    // and clip to it
    const char* ptr = strrchr(filename.buf,'/');

    if(ptr)
    {
        const u32 len = (ptr - filename.buf) + 1;
        return string_slice(filename,0,len);
    }

    // if this fails clip to last '\'
    ptr = strrchr(filename.buf,'\\');

    if(ptr)
    {
        const u32 len = (ptr - filename.buf) + 1;
        return string_slice(filename,0,len);
    }

    // otherwhise just return nothing
    return make_static_string("",0);
}