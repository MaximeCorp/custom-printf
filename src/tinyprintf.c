#include "tinyprintf.h"

static int s_len(const char *s)
{
    int len = 0;

    while (*s)
    {
        s++;
        len++;
    }

    return len;
}

static void revert_s(char *s)
{
    int len = s_len(s);

    for (int i = 0; i < len - i - 1; i++)
    {
        char temp = s[i];
        s[i] = s[len - i - 1];
        s[len - i - 1] = temp;
    }
}

static void insert_c(char **s, char c)
{
    int len = s_len(*s);
    *s = realloc(*s, (len + 2) * sizeof(char));
    for (int i = len + 1; i > 0; i--)
    {
        s[0][i] = s[0][i - 1];
    }

    s[0][0] = c;
}

static int insert_s(char **s1, char *s2)
{
    while (*s2)
    {
        insert_c(s1, *s2);
        s2++;
    }

    int len = s_len(*s1);

    return len;
}

static void stoa(char **input, char *s)
{
    *input = realloc(*input, sizeof(char));
    input[0][0] = '\0';

    if (!s)
    {
        insert_s(input, "(null)");
    }
    else
    {
        insert_s(input, s);
    }

    revert_s(*input);
}

static void ctoa(char **input, char c)
{
    *input = realloc(*input, sizeof(char));
    input[0][0] = '\0';

    insert_c(input, c);
}

static void ntoa(char **input, unsigned n, char *base)
{
    *input = realloc(*input, sizeof(char));
    *input[0] = '\0';

    int b = s_len(base);

    if (n == 0)
    {
        insert_c(input, base[0]);
        return;
    }

    while (n > 0)
    {
        insert_c(input, base[n % b]);
        n /= b;
    }
}

static void dtoa(char **input, int n)
{
    *input = realloc(*input, sizeof(char));
    *input[0] = '\0';

    int cp = n;

    if (n < 0)
    {
        cp *= -1;
    }

    if (n == 0)
    {
        insert_c(input, '0');
        return;
    }

    while (cp > 0)
    {
        insert_c(input, cp % 10 + '0');
        cp /= 10;
    }

    if (n < 0)
    {
        insert_c(input, '-');
    }
}

static void utoa(char **input, unsigned n)
{
    ntoa(input, n, "0123456789");
}

static void otoa(char **input, unsigned n)
{
    char base[9] = "01234567";

    ntoa(input, n, base);
}

static void xtoa(char **input, unsigned n)
{
    char base[17] = "0123456789abcdef";
    ntoa(input, n, base);
}

static int syntax_char(char c)
{
    char types[6] = { 'd', 'u', 's', 'c', 'o', 'x' };

    for (int i = 0; i < 6; i++)
    {
        if (c == types[i])
        {
            return 1;
        }
    }

    return 0;
}

static int add_type(const char format, char ***inputs, int *i_len, char c)
{
    if (format == c)
    {
        inputs[0][*i_len] = malloc(2 * sizeof(char));
        inputs[0][*i_len][0] = c;
        inputs[0][*i_len][1] = '\0';
        *i_len += 1;
        return 1;
    }

    return 0;
}

static int check_types(const char **format, char ***inputs)
{
    int f_len = s_len(*format);
    int i_len = 0;

    for (int i = 0; i < f_len - 1; i++)
    {
        if (format[0][i] == '%')
        {
            char types[6] = { 'd', 'u', 's', 'c', 'o', 'x' };
            for (int j = 0; j < 6; j++)
            {
                if (add_type(format[0][i + 1], inputs, &i_len, types[j]))
                {
                    break;
                }
            }
        }
    }

    return i_len;
}

static void edit_input(va_list ap, char **input)
{
    char c = *input[0];

    if (c == 'd')
    {
        dtoa(input, va_arg(ap, int));
    }
    if (c == 'u')
    {
        utoa(input, va_arg(ap, unsigned));
    }
    if (c == 's')
    {
        stoa(input, va_arg(ap, char *));
    }
    if (c == 'c')
    {
        ctoa(input, va_arg(ap, int));
    }
    if (c == 'o')
    {
        otoa(input, va_arg(ap, int));
    }
    if (c == 'x')
    {
        xtoa(input, va_arg(ap, int));
    }
}

static int make_str(char **final, const char *format, char ***inputs)
{
    int nb_i = 0;
    int o_len = 0;

    *final = calloc(1, sizeof(char));

    for (int i = 0; format[i]; i++)
    {
        char c = format[i];

        if (c == '%')
        {
            if (syntax_char(format[i + 1]))
            {
                o_len = insert_s(final, inputs[0][nb_i++]);
                i++;
            }
            else if (format[i + 1] == '%')
            {
                insert_c(final, '%');
                o_len++;
                i++;
            }
            else
            {
                insert_c(final, '%');
                o_len++;
            }
        }
        else
        {
            insert_c(final, c);
            o_len++;
        }
    }

    return o_len;
}

static void free_inputs(char **inputs, size_t i_len)
{
    for (size_t i = 0; i < i_len; i++)
    {
        free(inputs[i]);
    }

    free(inputs);
}

int tinyprintf(const char *format, ...)
{
    if (format == NULL)
    {
        return 0;
    }

    int f_len = s_len(format);

    char **inputs = calloc(f_len / 2, sizeof(char *));

    char *final = NULL;

    int i_len = check_types(&format, &inputs);

    va_list ap;

    va_start(ap, format);

    for (int i = 0; i < i_len; i++)
    {
        edit_input(ap, inputs + i);
    }

    va_end(ap);

    int o_len = make_str(&final, format, &inputs);

    for (int i = o_len - 1; i >= 0; i--)
    {
        putchar(final[i]);
    }

    free(final);

    free_inputs(inputs, i_len);

    return o_len;
}
