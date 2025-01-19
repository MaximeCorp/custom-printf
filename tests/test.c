#include <criterion/assert.h>
#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../src/tinyprintf.h"

Test(tinyprintf_test1, s, .init = cr_redirect_stdout)
{
    char test[] = "caca";
    int expected = 4;

    int res = tinyprintf(test);

    cr_expect(expected == res);

    fflush(stdout);

    cr_assert_stdout_eq_str(test);
}

Test(tinyprintf_test2, null, .init = cr_redirect_stdout)
{
    char *test = NULL;
    int expected = 0;

    int res = tinyprintf(test);

    cr_expect(expected == res);

    fflush(stdout);

    cr_assert_stdout_eq_str("");
}

Test(tinyprintf_test3, s, .init = cr_redirect_stdout)
{
    char test[] = "%d";
    int expected = 1;

    int res = tinyprintf(test, 1);

    cr_expect(expected == res);

    fflush(stdout);

    cr_assert_stdout_eq_str("1");
}

Test(tinyprintf_test4, s, .init = cr_redirect_stdout)
{
    char test[] = "caca";
    int expected = 4;

    int res = tinyprintf(test);

    cr_expect(expected == res);

    fflush(stdout);

    cr_assert_stdout_eq_str(test);
}

Test(tinyprintf_test4, all_types, .init = cr_redirect_stdout)
{
    char test[] = "%d%u%s%c%o%x%%%";
    int expected = 8;

    int res = tinyprintf(test, 1, 1, "1", '1', 1, 1);

    cr_expect(expected == res);

    fflush(stdout);

    cr_assert_stdout_eq_str("111111%%");
}

Test(tinyprintf_test5, null_s, .init = cr_redirect_stdout)
{
    char test[] = "%s";
    int expected = 6;

    int res = tinyprintf(test, NULL);

    cr_expect(expected == res);

    fflush(stdout);

    cr_assert_stdout_eq_str("(null)");
}

Test(tinyprintf_test6, neg_int, .init = cr_redirect_stdout)
{
    char test[] = "%d";
    int expected = 2;

    int res = tinyprintf(test, -1);

    cr_expect(expected == res);

    fflush(stdout);

    cr_assert_stdout_eq_str("-1");
}

Test(tinyprintf_test7, zero, .init = cr_redirect_stdout)
{
    char test[] = "%d";
    int expected = 1;

    int res = tinyprintf(test, 0);

    cr_expect(expected == res);

    fflush(stdout);

    cr_assert_stdout_eq_str("0");
}

Test(tinyprintf_test8, zero_u, .init = cr_redirect_stdout)
{
    char test[] = "%u";
    int expected = 1;

    int res = tinyprintf(test, 0);

    cr_expect(expected == res);

    fflush(stdout);

    cr_assert_stdout_eq_str("0");
}
Test(tinyprintf_test9, sn, .init = cr_redirect_stdout)
{
    char test[] = "%s";
    int expected = 6;

    int res = tinyprintf(test, NULL);

    cr_expect(expected == res);

    fflush(stdout);

    cr_assert_stdout_eq_str("(null)");
}
