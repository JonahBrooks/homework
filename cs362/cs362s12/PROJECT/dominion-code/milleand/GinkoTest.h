#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>

#ifndef GINKOTEST_H
#define GINKOTEST_H

/*
*  GinkoTest is a very simple test framework for C.  The thing
*		that sets GinkoTest apart from other frameworks is
*		heirarchically organized assertions to allow for a
*		'fail-and-go' testing system.  See the README for more
*		information on how to make the most of GinkoTest
*
*  Author: Tyler McClung
*
*  Credits:
*  	Substantial portions of the below structs have been
*		cannibalized from CuTest code.  All credit for the
*		Tests and Suites base functionality goes to the authors
*		of CuTest.
*/

#define ROOT_ID			0
#define ANONYMOUS_ID	-1
#define MAX_TEST_CASES	1024
#define GINKO_ALLOC(TYPE)		((TYPE*) malloc(sizeof(TYPE)))

/*********************** A string class, courtesy of CuTest ***********************/
#define HUGE_STRING_LEN	8192
#define STRING_MAX		256
#define STRING_INC		256

typedef struct
{
	int length;
	int size;
	char* buffer;
} GinkoString;

void ginkoStringInit(GinkoString* str);
GinkoString* newGinkoString(void);
void ginkoStringRead(GinkoString* str, const char* path);
void ginkoStringAppend(GinkoString* str, const char* text);
void ginkoStringAppendChar(GinkoString* str, char ch);
void ginkoStringAppendFormat(GinkoString* str, const char* format, ...);
void ginkoStringInsert(GinkoString* str, const char* text, int pos);
void ginkoStringResize(GinkoString* str, int newSize);

/*********************** Assertion results to be stored in GinkoTests ***********************/
typedef struct
{
	int id;
	int parent_id;
	char message[STRING_MAX];
} GinkoAssertion;

/*********************** GinkoTests hold a test and its results ***********************/
typedef struct GinkoTest GinkoTest;

typedef void (*TestFunction)(GinkoTest *);
struct GinkoTest
{
	int id;
	int parent_id;
	const char* name;
	TestFunction function;
	int failed;
	int ran;
	jmp_buf *jumpBuf;
	int numberOfAssertions;
	GinkoAssertion failedAssertions[MAX_TEST_CASES];
};

GinkoTest* _newGinkoTest(const int id, const int parent_id, const char* name, TestFunction function); // Child test

// Several constructors, depending on which features you want to take advantage of
#define newGinkoTest(name, function)	_newGinkoTest(ANONYMOUS_ID, ROOT_ID, (name), (function)
#define newGinkoTestParent(id, name, function)	_newGinkoTest((id), ROOT_ID, (name), (function)
#define newGinkoTestChild(id, parent_id, name, function)	_newGinkoTest((id), (parent_id), (name), (function)

int parentAssertionPassed(GinkoTest* t, const int parent_id);
void runGinkoTest(GinkoTest* t);

// Interal functions
void _GinkoFail_Line(GinkoTest* t, const char* file, int line, const int id, const int parent_id, const char* message2, const char* message, int isBreaking);
int _GinkoAssert_Line(GinkoTest* t, const char* file, int line, const int id, const int parent_id, const char* message, int condition, int isBreaking);
int _GinkoAssertIntEquals_Line(GinkoTest* t, const char* file, int line, const int id, const int parent_id, const char* message, int expected, int actual, int isBreaking);
int _GinkoAssertDblEquals_Line(GinkoTest* t, const char* file, int line, const int id, const int parent_id, const char* message, double expected, double actual, double delta, int isBreaking);
int _GinkoAssertStrEquals_Line(GinkoTest* t, const char* file, int line, const int id, const int parent_id, const char* message, char* expected, char* actual, int isBreaking);
int _TestContains(GinkoTest* t, int id);

/*********************** Several asserts, depending on which features you want to take advantage of ***********************/
// Fail command, breaking
#define ginkoFail(tc, ms)                        		_GinkoFail_Line(  (tc), __FILE__, __LINE__, ANONYMOUS_ID, ROOT_ID, NULL, (ms), 1)
// Anonymous assert, traditional functionality (breaking)
#define ginkoAssert(tc, ms, cond)                		_GinkoAssert_Line((tc), __FILE__, __LINE__, ANONYMOUS_ID, ROOT_ID, (ms), (cond), 1)
// Parent-less assert, breaking
#define ginkoAssertParent(tc, id, ms, cond)					_GinkoAssert_Line((tc), __FILE__, __LINE__, (id), ROOT_ID, (ms), (cond), 1)
// Child assert, breaking
#define ginkoAssertChild(tc, id, parent_id, ms, cond)		_GinkoAssert_Line((tc), __FILE__, __LINE__, (id), (parent_id), (ms), (cond), 1)
// Parent-less assert, non-breaking
#define	nbGinkoAssertParent(tc, id, ms, cond)					_GinkoAssert_Line((tc), __FILE__, __LINE__, (id), ROOT_ID, (ms), (cond), 0)
// Child assert, non-breaking
#define nbGinkoAssertChild(tc, id, parent_id, ms, cond)		_GinkoAssert_Line((tc), __FILE__, __LINE__, (id), (parent_id), (ms), (cond), 0)

#define ginkoAssertIntEquals(tc, ms, expected, actual)                	_GinkoAssertIntEquals_Line((tc), __FILE__, __LINE__, ANONYMOUS_ID, ROOT_ID, (ms), (expected), (actual), 1)
#define ginkoAssertIntEqualsParent(tc, id, ms, expected, actual)				_GinkoAssertIntEquals_Line((tc), __FILE__, __LINE__, (id), ROOT_ID, (ms), (expected), (actual), 1)
#define ginkoAssertIntEqualsChild(tc, id, parent_id, ms, expected, actual)	_GinkoAssertIntEquals_Line((tc), __FILE__, __LINE__, (id), (parent_id), (ms), (expected), (actual), 1)
#define	nbGinkoAssertIntEqualsParent(tc, id, ms, expected, actual)				_GinkoAssertIntEquals_Line((tc), __FILE__, __LINE__, (id), ROOT_ID, (ms), (expected), (actual), 0)
#define nbGinkoAssertIntEqualsChild(tc, id, parent_id, ms, expected, actual)	_GinkoAssertIntEquals_Line((tc), __FILE__, __LINE__, (id), (parent_id), (ms), (expected), (actual), 0)

#define ginkoAssertDblEquals(tc, ms, expected, actual, delta)                	_GinkoAssertDblEquals_Line((tc), __FILE__, __LINE__, ANONYMOUS_ID, ROOT_ID, (ms), (expected), (actual), 1)
#define ginkoAssertDblEqualsParent(tc, id, ms, expected, actual, delta)				_GinkoAssertDblEquals_Line((tc), __FILE__, __LINE__, (id), ROOT_ID, (ms), (expected), (actual), (delta), 1)
#define ginkoAssertDblEqualsChild(tc, id, parent_id, ms, expected, actual, delta)	_GinkoAssertDblEquals_Line((tc), __FILE__, __LINE__, (id), (parent_id), (ms), (expected), (actual), (delta), 1)
#define	nbGinkoAssertDblEqualsParent(tc, id, ms, expected, actual, delta)				_GinkoAssertDblEquals_Line((tc), __FILE__, __LINE__, (id), ROOT_ID, (ms), (expected), (actual), (delta), 0)
#define nbGinkoAssertDblEqualsChild(tc, id, parent_id, ms, expected, actual, delta)	_GinkoAssertDblEquals_Line((tc), __FILE__, __LINE__, (id), (parent_id), (ms), (expected), (actual), (delta), 0)

#define ginkoAssertStrEquals(tc, ms, expected, actual)                	_GinkoAssertStrEquals_Line((tc), __FILE__, __LINE__, ANONYMOUS_ID, ROOT_ID, (ms), (expected), (actual), 1)
#define ginkoAssertStrEqualsParent(tc, id, ms, expected, actual)				_GinkoAssertStrEquals_Line((tc), __FILE__, __LINE__, (id), ROOT_ID, (ms), (expected), (actual), 1)
#define ginkoAssertStrEqualsChild(tc, id, parent_id, ms, expected, actual)	_GinkoAssertStrEquals_Line((tc), __FILE__, __LINE__, (id), (parent_id), (ms), (expected), (actual), 1)
#define	nbGinkoAssertStrEqualsParent(tc, id, ms, expected, actual)				_GinkoAssertStrEquals_Line((tc), __FILE__, __LINE__, (id), ROOT_ID, (ms), (expected), (actual), 0)
#define nbGinkoAssertStrEqualsChild(tc, id, parent_id, ms, expected, actual)	_GinkoAssertStrEquals_Line((tc), __FILE__, __LINE__, (id), (parent_id), (ms), (expected), (actual), 0)

/*********************** GinkoSuites hold tests and their results ***********************/
#define SUITE_ADD_TEST(SUITE,TEST)	ginkoSuiteAdd(SUITE, _newGinkoTest(ANONYMOUS_ID, ROOT_ID, #TEST, TEST))
#define SUITE_ADD_TEST_PARENT(SUITE, ID, TEST)	ginkoSuiteAdd(SUITE, _newGinkoTest(ID, ROOT_ID, #TEST, TEST))
#define SUITE_ADD_TEST_CHILD(SUITE, ID, PARENT_ID, TEST)	ginkoSuiteAdd(SUITE, _newGinkoTest(ID, PARENT_ID, #TEST, TEST))


typedef struct
{
	int count;
	GinkoTest* list[MAX_TEST_CASES];
	int failCount;
} GinkoSuite;


void ginkoSuiteInit(GinkoSuite* testSuite);
GinkoSuite* newGinkoSuite(void);
void ginkoSuiteAdd(GinkoSuite* testSuite, GinkoTest *testCase);
void ginkoSuiteAddSuite(GinkoSuite* testSuite, GinkoSuite* testSuite2);
void ginkoSuiteRun(GinkoSuite* testSuite);
void ginkoSuiteSummary(GinkoSuite* testSuite, GinkoString* summary);
void ginkoSuiteDetails(GinkoSuite* testSuite, GinkoString* details);
int parentTestPassed(GinkoSuite* suite, const int parent_id);

#endif
