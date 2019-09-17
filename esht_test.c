#include <errno.h>
#include <stdio.h>
#include "esht.c"

int test_good_create(size_t *caps, size_t len, int *test)
{
    int rc = 0;
    int subtest = 1;
    esht *table;

    table = esht_create();
    if (table != NULL)
    {
        printf("Test %d-%d passed: create\n", *test, subtest);
        subtest++;
        if (table->cap == 1)
            printf("Test %d-%d passed: capacity 1\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: capacity %lu expected 1\n", *test, subtest, table->cap);
        }
        subtest++;
        if (table->len == 0)
            printf("Test %d-%d passed: initial len 0\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: initial len %lu expected 0\n", *test, subtest, table->len);
        }
        subtest++;
        if (table->entries != NULL)
            printf("Test %d-%d passed: entries not null\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: entries null\n", *test, subtest);
        }
    }
    else
    {
        rc = 1;
        printf("Test %d-%d failed: create", *test, subtest);
    }
    (*test)++;
    esht_destroy(table);
    return rc;
}

int test_add_remove_get(int *test)
{
    int rc = 0;
    int subtest = 1;
    void *get;
    size_t len;
    esht *table = esht_create();
    if (table == NULL)
        return 1;

    if (!esht_update(table, "a", "abcde", 6))
        printf("Test %d-%d passed: update success\n", *test, subtest);
    else
    {
        rc = 1;
        printf("Test %d-%d failed: update error\n", *test, subtest);
    }
    subtest++;
    if (table->len == 1)
        printf("Test %d-%d passed: table len 1\n", *test, subtest);
    else
    {
        rc = 1;
        printf("Test %d-%d failed: expected len 1 got %lu\n", *test, subtest, table->len);
    }
    subtest++;
    if (table->cap == 2)
        printf("Test %d-%d passed: table cap 2\n", *test, subtest);
    else
    {
        rc = 1;
        printf("Test %d-%d failed: expected cap 2 got %lu\n", *test, subtest, table->cap);
    }
    subtest++;
    get = esht_get(table, "a", &len);
    if (get != NULL)
    {
        printf("Test %d-%d passed: found key 'a'\n", *test, subtest);
        subtest++;
        if (!strcmp("abcde", (char *)get))
            printf("Test %d-%d passed: key 'a' value matched\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: key 'a' value expected 'abcde' got '%s'\n", *test, subtest, (char *)get);
        }
        subtest++;
        if (len == 6)
            printf("Test %d-%d passed: got key 'a' length 6\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: got key 'a' length %lu expected 6\n", *test, subtest, len);
        }
        free(get);
        (*test)++;
        subtest = 1;
        esht_remove(table, "a");
        get = esht_get(table, "a", &len);
        if (get == NULL)
            printf("Test %d-%d passed: removed entry not found\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: found entry for removed 'a'\n", *test, subtest);
        }
        subtest++;
        if (table->len == 0)
            printf("Test %d-%d passed: table len 0\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: table len %lu expected 0\n", *test, subtest, table->len);
        }
        subtest++;
        if (table->cap == 1)
            printf("Test %d-%d passed: table cap 1\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: table cap %lu expected 1", *test, subtest, table->cap);
        }
    }
    else
    {
        rc = 1;
        printf("Test %d-%d failed: get 'a' NULL return\n", *test, subtest);
    }
    if (get != NULL)
        free(get);

    (*test)++;
    subtest = 1;
    if (!esht_update(table, "a", "a", 2))
        printf("Test %d-%d passed: update 'a'\n", *test, subtest);
    else
    {
        rc = 1;
        printf("Test %d-%d failed: update 'a' error\n", *test, subtest);
    }
    subtest++;
    if (!esht_update(table, "b", "bc", 3))
    {
        printf("Test %d-%d passed: update 'b'\n", *test, subtest);
        subtest++;
        if (table->len == 2)
            printf("Test %d-%d passed: table len 2\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: table len %lu expected 2\n", *test, subtest, table->len);
        }
        subtest++;
        if (table->cap == 4)
            printf("Test %d-%d passed: table cap 4\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: table cap %lu expected 4\n", *test, subtest, table->cap);
        }
        subtest++;
        get = esht_get(table, "a", &len);
        if (get != NULL)
            printf("Test %d-%d passed: got 'a'\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed; got 'a' NULL\n", *test, subtest);
        }
        subtest++;
        if (!strcmp("a", (char *)get))
            printf("Test %d-%d passed: key 'a' value 'a'\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: key 'a' got value '%s' expected 'a'", *test, subtest, (char *)get);
        }
        subtest++;
        if (len == 2)
            printf("Test %d-%d passed: key 'a' len 2\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: key 'a' len %lu expected 2\n", *test, subtest, len);
        }
        subtest++;
        free(get);
        get = esht_get(table, "b", &len);
        if (get != NULL)
            printf("Test %d-%d passed: got 'b'\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: got 'b' NULL\n", *test, subtest);
        }
        subtest++;
        if (!strcmp("bc", (char *)get))
            printf("Test %d-%d passed: key 'b' value 'bc'\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: key 'b' got value '%s' expected 'bc'\n", *test, subtest, (char *)get);
        }
        subtest++;
        if (len == 3)
            printf("Test %d-%d passed: key 'b' len 3\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: key 'b' len %lu expected 3\n", *test, subtest, len);
        }
        subtest++;
        if (table->cap == 4)
            printf("Test %d-%d passed: table cap 4\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: table cap %lu expected 4\n", *test, subtest, table->cap);
        }
        free(get);

        (*test)++;
        subtest = 1;
        if (!esht_update(table, "a", "ab", 3))
            printf("Test %d-%d passed: updated 'a'\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: updated 'a' failed\n", *test, subtest);
        }
        subtest++;
        get = esht_get(table, "a", &len);
        if (get != NULL)
        {
            printf("Test %d-%d passed: got key 'a'\n", *test, subtest);
            subtest++;
            if (!strcmp((char *)get, "ab"))
                printf("Test %d-%d passed: key 'a' value 'ab'\n", *test, subtest);
            else
            {
                rc = 1;
                printf("Test %d-%d failed: key 'a' value '%s' expceted 'ab'\n", *test, subtest, (char *)get);
            }
            subtest++;
            if (len == 3)
                printf("Test %d-%d passed: key 'a' len 3\n", *test, subtest);
            else
            {
                rc = 1;
                printf("Test %d-%d failed: key 'a' len %lu expected 3\n", *test, subtest, len);
            }
            free(get);
        }
        else
        {
            rc = 1;
            printf("Test %d-%d failed; got key 'a' NULL\n", *test, subtest);
        }
        get = esht_get(table, "b", &len);
        if (get != NULL)
        {
            printf("Test %d-%d passed: got 'b'\n", *test, subtest);
            subtest++;
            if (!strcmp("bc", (char *)get))
                printf("Test %d-%d passed: key 'b' value 'bc'\n", *test, subtest);
            else
            {
                rc = 1;
                printf("Test %d-%d failed: key 'b' got value '%s' expected 'bc'\n", *test, subtest, (char *)get);
            }
            subtest++;
            if (len == 3)
                printf("Test %d-%d passed: key 'b' len 3\n", *test, subtest);
            else
            {
                rc = 1;
                printf("Test %d-%d failed: key 'b' len %lu expected 3\n", *test, subtest, len);
            }
            free(get);
        }
        else
        {
            rc = 1;
            printf("Test %d-%d failed: got 'b' NULL\n", *test, subtest);
        }
        subtest++;
        if (table->len == 2)
            printf("Test %d-%d passed: table len 2\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: table len %lu expected 2\n", *test, subtest, table->len);
        }
        subtest++;
        if (table->cap == 4)
            printf("Test %d-%d passed: table cap 4\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: table cap %lu expected 4\n", *test, subtest, table->cap);
        }

        (*test)++;
        subtest = 1;
        esht_remove(table, "a");
        if (table->len == 1)
            printf("Test %d-%d passed: table len 1\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: table len %lu expected 1\n", *test, subtest, table->len);
        }
        subtest++;
        if (table->cap == 4)
            printf("Test %d-%d passed: table cap 4\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: table cap %lu expected 4\n", *test, subtest, table->cap);
        }
        subtest++;
        get = esht_get(table, "a", &len);
        if (get == NULL)
            printf("Test %d-%d passed: key 'a' NULL\n", *test, subtest);
        else
        {
            rc = 1;
            printf("Test %d-%d failed: key 'a' expected NULL got value\n", *test, subtest);
        }
        subtest++;
        get = esht_get(table, "b", &len);
        if (get != NULL)
        {
            printf("Test %d-%d passed: got 'b'\n", *test, subtest);
            subtest++;
            if (!strcmp("bc", (char *)get))
                printf("Test %d-%d passed: key 'b' value 'bc'\n", *test, subtest);
            else
            {
                rc = 1;
                printf("Test %d-%d failed: key 'b' got value '%s' expected 'bc'\n", *test, subtest, (char *)get);
            }
            subtest++;
            if (len == 3)
                printf("Test %d-%d passed: key 'b' len 3\n", *test, subtest);
            else
            {
                rc = 1;
                printf("Test %d-%d failed: key 'b' len %lu expected 3\n", *test, subtest, len);
            }
            free(get);
        }
        else
        {
            rc = 1;
            printf("Test %d-%d failed: got 'b' NULL\n", *test, subtest);
        }
    }
    else
    {
        rc = 1;
        printf("Test %d-%d failed: update 'b' error\n", *test, subtest);
    }

    esht_destroy(table);
    return rc;
}

int main(int argc, char const *argv[])
{
    int rc = 0;
    int test = 1;
    size_t good_caps[] = {1, 4, 9, 16};

    if (test_good_create(good_caps, 4, &test))
        rc = 1;

    if (test_add_remove_get(&test))
        rc = 1;

    return rc;
}
