#include "tests/test_framework.h"
#include "modules/database/params.h"

int main()
{
    TEST_START();

    TEST_START_SECTION("Params Read/Write");
    {
        db_params_t params = {0};
        params_get(&params);

        float stub = params.mainParachuteHeight;
        params.mainParachuteHeight = 999;

        TEST_ASSERT(params_save(&params));
        TEST_ASSERT(params_get(&params));

        TEST_ASSERT(params.mainParachuteHeight == 999);

        params.mainParachuteHeight = stub;

        TEST_ASSERT(params_save(&params));
    }
    TEST_END_SECTION();

    TEST_END();
}