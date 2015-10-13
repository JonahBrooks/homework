#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "GinkoTest.h"

/*********************** GinkoString ***********************/

char* ginkoStrAlloc(int size)
{
	char* newStr = (char*) malloc( sizeof(char) * (size) );
	return newStr;
}

char* ginkoStrCopy(const char* old)
{
	int len = strlen(old);
	char* newStr = ginkoStrAlloc(len + 1);
	strcpy(newStr, old);
	return newStr;
}

void ginkoStringInit(GinkoString* str)
{
	str->length = 0;
	str->size = STRING_MAX;
	str->buffer = (char*) malloc(sizeof(char) * str->size);
	str->buffer[0] = '\0';
}

GinkoString* newGinkoString(void)
{
	GinkoString* str = GINKO_ALLOC(GinkoString);
	str->length = 0;
	str->size = STRING_MAX;
	str->buffer = (char*) malloc(sizeof(char) * str->size);
	str->buffer[0] = '\0';
	return str;
}

void ginkoStringResize(GinkoString* str, int newSize)
{
	str->buffer = (char*) realloc(str->buffer, sizeof(char) * newSize);
	str->size = newSize;
}

void ginkoStringAppend(GinkoString* str, const char* text)
{
	int length;

	if (text == NULL) {
		text = "NULL";
	}

	length = strlen(text);
	if (str->length + length + 1 >= str->size)
	ginkoStringResize(str, str->length + length + 1 + STRING_INC);
	str->length += length;
	strcat(str->buffer, text);
}

void ginkoStringAppendChar(GinkoString* str, char ch)
{
	char text[2];
	text[0] = ch;
	text[1] = '\0';
	ginkoStringAppend(str, text);
}

void ginkoStringAppendFormat(GinkoString* str, const char* format, ...)
{
	va_list argp;
	char buf[HUGE_STRING_LEN];
	va_start(argp, format);
	vsprintf(buf, format, argp);
	va_end(argp);
	ginkoStringAppend(str, buf);
}

void ginkoStringInsert(GinkoString* str, const char* text, int pos)
{
	int length = strlen(text);
	if (pos > str->length)
	pos = str->length;
	if (str->length + length + 1 >= str->size)
	ginkoStringResize(str, str->length + length + 1 + STRING_INC);
	memmove(str->buffer + pos + length, str->buffer + pos, (str->length - pos) + 1);
	str->length += length;
	memcpy(str->buffer + pos, text, length);
}

/*********************** GinkoTest ***********************/
GinkoTest* _newGinkoTest(const int id, const int parent_id, const char* name, TestFunction function)
{
	GinkoTest* t = GINKO_ALLOC(GinkoTest);
	t->id = id;
	t->parent_id = parent_id;
	t->name = ginkoStrCopy(name);
	t->failed = 0;
	t->ran = 0;
	t->numberOfAssertions = 0;
	t->function = function;
	t->jumpBuf = NULL;
	return t;
}

char* ginkoTestResults(GinkoTest* t)
{
	GinkoString string;
	int i;

	ginkoStringInit(&string);
	ginkoStringAppend(&string, "\n");
	
	for (i = 0; i < t->numberOfAssertions; i++)
	{
		if (parentAssertionPassed(t, t->failedAssertions[i].parent_id))
		{
			ginkoStringAppend(&string, "  -");
			ginkoStringAppend(&string, t->failedAssertions[i].message);
			ginkoStringAppend(&string, "\n");
		}
	}
	return string.buffer;
}

int parentAssertionPassed(GinkoTest* t, const int parent_id)
{
	int i;
	if (parent_id == ROOT_ID)
	{
		return 1;
	}
	for (i = 0; i < t->numberOfAssertions; i++)
	{
		if (t->failedAssertions[i].id == parent_id)
		{
			return 0;
		}
	}
	return 1;
}

void runGinkoTest(GinkoTest* t)
{
	jmp_buf buf;
	t->jumpBuf = &buf;
	if (setjmp(buf) == 0)
	{
		t->ran = 1;
		(t->function)(t);
	}
	t->jumpBuf = 0;
}

int parentTestPassed(GinkoSuite* suite, const int parent_id)
{
	int i;
	if (parent_id == ROOT_ID)
	{
		return 1;
	}
	for (i = 0; i < suite->count; i++)
	{
		if (suite->list[i]->id == parent_id && suite->list[i]->failed == 1)
		{
			return 0;
		}
	}
	return 1;
}

/*********************** GinkoAssert ***********************/
int _TestContains(GinkoTest* t, int id)
{
	int i;
	for (i = 0; i < t->numberOfAssertions; i++)
	{
		if (t->failedAssertions[i].id == id)
		{
			return 1;
		}
	}
	return 0;
}

static void _GinkoFailInternal(GinkoTest* t, const char* file, int line, const int id, const int parent_id, GinkoString* string, int isBreaking)
{
	GinkoAssertion g;
	char buf[HUGE_STRING_LEN];

	sprintf(buf, "%s:%d: ", file, line);
	ginkoStringInsert(string, buf, 0);
	t->failed = 1;
	if ((id != ANONYMOUS_ID && _TestContains(t, id)) || id == ROOT_ID)
	{
		if (id == ROOT_ID)
		{
			printf("Invalid id for assertion\n");
		}
		return;
	}
	
	strcpy(g.message, string->buffer);
	g.id = id;
	g.parent_id = parent_id;
	t->failedAssertions[t->numberOfAssertions++] = g;
	if (isBreaking && t->jumpBuf != 0)
	{
		longjmp(*(t->jumpBuf), 0);
	}
}

void _GinkoFail_Line(GinkoTest* t, const char* file, int line, const int id, const int parent_id, const char* message2, const char* message, int isBreaking)
{
	GinkoString string;

	ginkoStringInit(&string);
	if (message2 != NULL)
	{
		ginkoStringAppend(&string, message2);
		ginkoStringAppend(&string, ": ");
	}
	ginkoStringAppend(&string, message);
	_GinkoFailInternal(t, file, line, id, parent_id, &string, isBreaking);
}

int _GinkoAssert_Line(GinkoTest* t, const char* file, int line, const int id, const int parent_id, const char* message, int condition, int isBreaking)
{
	if (condition)
	{
		return 1;
	}
	_GinkoFail_Line(t, file, line, id, parent_id, NULL, message, isBreaking);
	return 0;
}

int _GinkoAssertIntEquals_Line(GinkoTest* t, const char* file, int line, const int id, const int parent_id, const char* message, int expected, int actual, int isBreaking)
{
	char buf[STRING_MAX];
	if (expected == actual)
	{
		return 1;
	}
	sprintf(buf, "expected <%d> but was <%d>", expected, actual);
	_GinkoFail_Line(t, file, line, id, parent_id, message, buf, isBreaking);
	return 0;
}

int _GinkoAssertDblEquals_Line(GinkoTest* t, const char* file, int line, const int id, const int parent_id, const char* message, double expected, double actual, double delta, int isBreaking)
{
	char buf[STRING_MAX];
	if (fabs(expected - actual) <= delta)
	{
		return 1;
	}
	sprintf(buf, "expected <%lf> but was <%lf>", expected, actual);
	_GinkoFail_Line(t, file, line, id, parent_id, message, buf, isBreaking);
	return 0;
}

int _GinkoAssertStrEquals_Line(GinkoTest* t, const char* file, int line, const int id, const int parent_id, const char* message, char* expected, char* actual, int isBreaking)
{
	GinkoString string;
	if ((expected == NULL && actual == NULL) ||
			(expected != NULL && actual != NULL &&
				strcmp(expected, actual) == 0))
	{
		return 1;
	}

	ginkoStringInit(&string);
	if (message != NULL)
	{
		ginkoStringAppend(&string, message);
		ginkoStringAppend(&string, ": ");
	}
	ginkoStringAppend(&string, "expected <");
	ginkoStringAppend(&string, expected);
	ginkoStringAppend(&string, "> but was <");
	ginkoStringAppend(&string, actual);
	ginkoStringAppend(&string, ">");
	_GinkoFailInternal(t, file, line, id, parent_id, &string, isBreaking);
	return 0;
}

/*********************** GinkoSuite ***********************/

void ginkoSuiteInit(GinkoSuite* testSuite)
{
	testSuite->count = 0;
	testSuite->failCount = 0;
}

GinkoSuite* newGinkoSuite(void)
{
	GinkoSuite* testSuite = GINKO_ALLOC(GinkoSuite);
	ginkoSuiteInit(testSuite);
	return testSuite;
}

void ginkoSuiteAdd(GinkoSuite* testSuite, GinkoTest *testCase)
{
	assert(testSuite->count < MAX_TEST_CASES);
	testSuite->list[testSuite->count] = testCase;
	testSuite->count++;
}

void ginkoSuiteAddSuite(GinkoSuite* testSuite, GinkoSuite* testSuite2)
{
	int i;
	for (i = 0 ; i < testSuite2->count ; ++i)
	{
		GinkoTest* testCase = testSuite2->list[i];
		ginkoSuiteAdd(testSuite, testCase);
	}
}

void ginkoSuiteRun(GinkoSuite* testSuite)
{
	int i;
	for (i = 0 ; i < testSuite->count ; ++i)
	{
		GinkoTest* testCase = testSuite->list[i];
		runGinkoTest(testCase);
		if (testCase->failed)
		{
			testSuite->failCount += 1;
		}
	}
}

void ginkoSuiteSummary(GinkoSuite* testSuite, GinkoString* summary)
{
	int i;
	for (i = 0 ; i < testSuite->count ; ++i)
	{
		GinkoTest* testCase = testSuite->list[i];
		ginkoStringAppend(summary, testCase->failed ? "F" : ".");
	}
	ginkoStringAppend(summary, "\n\n");
}

void ginkoSuiteDetails(GinkoSuite* testSuite, GinkoString* details)
{
	int i;
	int failCount = 0;

	if (testSuite->failCount == 0)
	{
		int passCount = testSuite->count - testSuite->failCount;
		const char* testWord = passCount == 1 ? "test" : "tests";
		ginkoStringAppendFormat(details, "OK (%d %s)\n", passCount, testWord);
	}
	else
	{
		if (testSuite->failCount == 1)
		ginkoStringAppend(details, "There was 1 failure:\n");
		else
		ginkoStringAppendFormat(details, "There were %d failures:\n", testSuite->failCount);

		for (i = 0 ; i < testSuite->count ; ++i)
		{
			GinkoTest* testCase = testSuite->list[i];
			if (testCase->failed)
			{
				failCount++;
				if (parentTestPassed(testSuite, testCase->parent_id))
				{
					ginkoStringAppendFormat(details, "%d) %s: %s\n", failCount, testCase->name, ginkoTestResults(testCase));
				}
			}
		}
		ginkoStringAppend(details, "\n!!!FAILURES!!!\n");

		ginkoStringAppendFormat(details, "Runs: %d ",   testSuite->count);
		ginkoStringAppendFormat(details, "Passes: %d ", testSuite->count - testSuite->failCount);
		ginkoStringAppendFormat(details, "Fails: %d\n",  testSuite->failCount);
	}
}

