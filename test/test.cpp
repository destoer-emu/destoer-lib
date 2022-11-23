#include <destoer.cpp>

using namespace destoer;

int main()
{
    // TODO: expand this if need be, this is basically just a simple compile check atm
    Array<String> buf;

    String str = "Hey";

    push_var(buf,str);

    printf("'%s' : %d\n",buf[0].buf,buf[0] == "Hey");
}