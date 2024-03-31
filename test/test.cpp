#include <destoer.cpp>

using namespace destoer;


b32 test_arr_str()
{
    // TODO: expand this if need be, this is basically just a simple compile check atm
    Array<String> buf;

    String str = "Hey";

    push_var(buf,str);

    return buf[0] == "Hey";
}

b32 test_bit_minip()
{
    u32 v = 1;

    const b32 not_set = !is_set(v,1);

    v = set_bit(v,1);

    const b32 set = is_set(v,1);

    v = deset_bit(v,1);

    const b32 post_set = !is_set(v,1);

    return not_set && set && post_set;
}

using TEST_FUNCTION = b32 (*)();
struct Test
{
    const char* name;
    TEST_FUNCTION func;
};

const Test TESTS[] = 
{
    {"arr str",&test_arr_str},
    {"bit minip",&test_bit_minip},
};

static constexpr u32 TEST_SIZE = sizeof(TESTS) / sizeof(Test);



int main()
{
    for(u32 i = 0; i < TEST_SIZE; i++)
    {
        const auto& test = TESTS[i];
        
        const b32 pass = test.func();

        if(pass)
        {
            printf("pass: %s\n",test.name);
        }

        else
        {
            printf("fail: %s\n",test.name);
            return -1;
        }
    }   
}