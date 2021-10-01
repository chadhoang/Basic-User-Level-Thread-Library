#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>
#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}

/* Destroy */
void test_queue_destroy(void) 
{
	fprintf(stderr, "*** TEST destroy ***\n");

	queue_t q;
	q = queue_create();
	TEST_ASSERT(queue_destroy(q) == 0);
}

/* Destroy Error Unempty */
void test_queue_destroy_error_unempty(void) 
{
	fprintf(stderr, "*** TEST destroy error unempty ***\n");

	queue_t q;
	int data = 1;
	q = queue_create();
	queue_enqueue(q, &data);
	TEST_ASSERT(queue_destroy(q) == -1);
}

/* Destroy Error Queue Null */
void test_queue_destroy_error_queue_null(void) 
{
	fprintf(stderr, "*** TEST destroy error queue null ***\n");

	queue_t q = NULL;
	TEST_ASSERT(queue_destroy(q) == -1);
}

/* Enqueue Error Queue Null */
void test_queue_enqueue_error_queue_null(void)
{
	queue_t q = NULL;
	int data = 3;
	fprintf(stderr, "*** TEST enqueue error queue null ***\n");

	TEST_ASSERT(queue_enqueue(q, &data) == -1);
}

/* Enqueue Error Data Null */
void test_queue_enqueue_error_data_null(void)
{
	queue_t q;
	q = queue_create();
	fprintf(stderr, "*** TEST enqueue error data null ***\n");

	TEST_ASSERT(queue_enqueue(q, NULL) == -1);
}

/* Dequeue Error Queue Null */
void test_queue_dequeue_error_queue_null(void)
{
	queue_t q = NULL;
	int *ptr;
	fprintf(stderr, "*** TEST dequeue error queue null ***\n");

	TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == -1);
}

/* Dequeue Error Data Null */
void test_queue_dequeue_error_data_null(void)
{
	queue_t q;
	int data = 3;
	q = queue_create();
	fprintf(stderr, "*** TEST dequeue error data null ***\n");

	queue_enqueue(q, &data);
	TEST_ASSERT(queue_dequeue(q, (void**)NULL) == -1);
}

/* Dequeue Error Queue Empty */
void test_queue_dequeue_error_queue_empty(void)
{
	queue_t q;
	int data = 3;
	int *ptr;
	q = queue_create();
	fprintf(stderr, "*** TEST dequeue error data null ***\n");

	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == -1);
}

/* Delete Error Queue Null */
void test_queue_delete_error_queue_null(void)
{
	queue_t q = NULL;
	int data = 3;
	fprintf(stderr, "*** TEST delete error queue null ***\n");

	TEST_ASSERT(queue_delete(q, &data) == -1);
}

/* Delete Error Data Null */
void test_queue_delete_error_data_null(void)
{
	queue_t q;
	int data = 3;
	fprintf(stderr, "*** TEST delete error data null ***\n");
	
	q = queue_create();
	queue_enqueue(q, &data);
	TEST_ASSERT(queue_delete(q, NULL) == -1);
}

/* Delete Error Data Unfound */
void test_queue_delete_error_data_unfound(void)
{
	queue_t q;
	int data1 = 3;
	int data2 = 1;
	fprintf(stderr, "*** TEST delete error data unfound ***\n");
	
	q = queue_create();
	queue_enqueue(q, &data1);
	TEST_ASSERT(queue_delete(q, &data2) == -1);
}

/* Simple Delete Middle */
void test_queue_delete_middle(void) 
{
	queue_t q;
	int data1 = 1;
	int data2 = 2;
	int data3 = 3;
	fprintf(stderr, "*** TEST simple delete middle ***\n");

	q = queue_create();
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);
	TEST_ASSERT(queue_delete(q, &data2) == 0);
	TEST_ASSERT(queue_length(q) == 2);
	TEST_ASSERT(queue_delete(q, &data2) == -1);
}

/* Simple Delete Tail */
void test_queue_delete_tail(void) 
{
	queue_t q;
	int data1 = 1;
	int data2 = 2;
	int data3 = 3;
	fprintf(stderr, "*** TEST simple delete tail ***\n");

	q = queue_create();
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);
	TEST_ASSERT(queue_delete(q, &data3) == 0);
	TEST_ASSERT(queue_length(q) == 2);
	TEST_ASSERT(queue_delete(q, &data3) == -1);
}

/* Simple Delete Head */
void test_queue_delete_head(void) 
{
	queue_t q;
	int data1 = 1;
	int data2 = 2;
	int data3 = 3;
	fprintf(stderr, "*** TEST simple delete head ***\n");

	q = queue_create();
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);
	TEST_ASSERT(queue_delete(q, &data1) == 0);
	TEST_ASSERT(queue_length(q) == 2);
	TEST_ASSERT(queue_delete(q, &data1) == -1);
}

/* Length Error Queue Null */
void test_queue_length_error_queue_null(void) 
{
	queue_t q = NULL;
	fprintf(stderr, "*** TEST length error queue null ***\n");
	
	TEST_ASSERT(queue_length(q) == -1);
}

/* Simple Length */
void test_queue_length(void) 
{
	queue_t q;
	int data1 = 1;
	int data2 = 2;
	int data3 = 3;
	fprintf(stderr, "*** TEST simple length ***\n");

	q = queue_create();
	TEST_ASSERT(queue_length(q) == 0);
	queue_enqueue(q, &data1);
	TEST_ASSERT(queue_length(q) == 1);
	queue_enqueue(q, &data2);
	TEST_ASSERT(queue_length(q) == 2);
	queue_enqueue(q, &data3);
	TEST_ASSERT(queue_length(q) == 3);
	queue_delete(q, &data1);
	TEST_ASSERT(queue_length(q) == 2);
	queue_delete(q, &data2);
	TEST_ASSERT(queue_length(q) == 1);
	queue_delete(q, &data3);
	TEST_ASSERT(queue_length(q) == 0);
}

/* Dequeue and Delete */
void test_queue_dequeue_delete(void) 
{
	queue_t q;
	int data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int *ptr;
	fprintf(stderr, "*** TEST dequeue delete ***\n");

	q = queue_create();
	for (int i = 0; i < 10; i++)
        queue_enqueue(q, &data[i]);
	for (int i = 0; i < 5; i++) {
		queue_dequeue(q, (void**)&ptr);
		TEST_ASSERT(*ptr == i);
	}
	for (int i = 9; i > 5; i--) 
		queue_delete(q, &i);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(*ptr == 5);
}

// Callback function that increments integer items by a certain value (or delete
// item if item is value 42) 
static int inc_item(queue_t q, void *data, void *arg)
{
    int *a = (int*)data;
    int inc = (int)(long)arg;

    if (*a == 42)
        queue_delete(q, data);
    else
        *a += inc;

    return 0;
}

/* Callback function that finds a certain item according to its value */
static int find_item(queue_t q, void *data, void *arg)
{
    int *a = (int*)data;
    int match = (int)(long)arg;
    (void)q; //unused

    if (*a == match)
        return 1;

    return 0;
}

/* Iterate */
void test_iterator(void)
{
    queue_t q = NULL;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
    size_t i;
    int *ptr;
	fprintf(stderr, "*** TEST iterator ***\n");

	// Test @queue is NULL returns -1
	TEST_ASSERT(queue_iterate(q, inc_item, (void*)1, NULL) == -1);

    /* Initialize the queue and enqueue items */
    q = queue_create();
	// Test @func is NULL returns -1
	TEST_ASSERT(queue_iterate(q, NULL, (void*)1, NULL) == -1);
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Add value '1' to every item of the queue, delete item '42' */
    queue_iterate(q, inc_item, (void*)1, NULL);
    TEST_ASSERT(data[0] == 2);
    TEST_ASSERT(queue_length(q) == 9);

    /* Find and get the item which is equal to value '5' */
    ptr = NULL;     // result pointer *must* be reset first
    queue_iterate(q, find_item, (void*)5, (void**)&ptr);
    TEST_ASSERT(ptr != NULL);
    TEST_ASSERT(*ptr == 5);
    TEST_ASSERT(ptr == &data[3]);
}


int main(void)
{
	test_create();
	test_queue_simple();
	test_queue_destroy();
	test_queue_destroy_error_unempty();
	test_queue_destroy_error_queue_null();
	test_queue_enqueue_error_queue_null();
	test_queue_enqueue_error_data_null();
	test_queue_dequeue_error_queue_null();
	test_queue_dequeue_error_data_null();
	test_queue_dequeue_error_queue_empty();
	test_queue_delete_error_queue_null();
	test_queue_delete_error_data_null();
	test_queue_delete_error_data_unfound();
	test_queue_delete_middle();
	test_queue_delete_tail();
	test_queue_delete_head();
	test_queue_length_error_queue_null();
	test_queue_length();
	test_queue_dequeue_delete();
	test_iterator();

	return 0;
}
