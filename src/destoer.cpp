#include <destoer/destoer.h>



namespace destoer
{



void print_line(const String& filename,u32 line)
{
    // this is slow, but we are about to terminate anyways
    // when this is used
    FILE *fp = fopen(filename.buf,"r");

    if(!fp)
    {
        printf("could not open file %s for error printing\n",filename.buf);
        return;
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





#include "stl-helpers.cpp"
#include "alloc.cpp"
#include "string.cpp"

// read entire file into string buf
Pair<StringBuffer,b32> read_str_buf(const String &filename)
{
    FILE* fp = fopen(filename.buf,"rb");

    StringBuffer buf;

    // file is invalid dont bother
    if(!fp)
    {
        return Pair{buf,true};
    }

    // get the file len
    fseek(fp, 0, SEEK_END);
    const u32 len = ftell(fp); 
    fseek(fp, 0, SEEK_SET); 

    // allocate an appriopately sized buffer
    // and read the whole file out
    resize(buf,len + 1);
    size_t rc = fread(buf.data,len,1,fp);
    UNUSED(rc);

    buf[len] = '\0';

    fclose(fp);
    return Pair{buf,false};
}

}
