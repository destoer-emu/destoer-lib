#include <destoer.cpp>

using namespace destoer;


void test_arr_str()
{
    Array<String> buf;

    String str = "Hey";

    push_var(buf,str);

    assert(buf[0] == "Hey");

    destroy_arr(buf);

    assert(count(buf) == 0);
}

void test_bit_minip()
{
    u32 v = 0;

    assert(!is_set(v,1));

    v = set_bit(v,1);
    
    assert(is_set(v,1));

    v = deset_bit(v,1);

    assert(!is_set(v,1));
}

void test_bit_set_minip()
{
    assert(popcount(0xffff) == 16);

    assert(destoer::ffs(0) == FFS_EMPTY);

    assert(destoer::ffs(8) == 3);
}



int main()
{
    test_arr_str();

    test_bit_minip();

    test_bit_set_minip();

    puts("passed all tests");
}