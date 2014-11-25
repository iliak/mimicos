#ifndef ASSERT_H
#define	ASSERT_H



//#pragma interface

#ifdef ASSERT

void AssertionFailure(STRPTR exp, char *file, char *baseFile, LONG line);

#define Assert(exp)  if (exp) ; \
        else AssertionFailure( #exp, __FILE__, __BASE_FILE__, __LINE__ )

#else

#define Assert(exp)

#endif

#endif

