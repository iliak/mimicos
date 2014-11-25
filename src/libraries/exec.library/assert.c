
/* Assert.cc for GNU C/C++ */

//#pragma implementation

#ifdef ASSERT

void AssertionFailure(char *exp, char *file, char *baseFile, int line)
{
    if (!strcmp(file, baseFile)) {
       fprintf(stderr, 
           "Assert(%s) failed in file %s, line %d\n", exp, file, line);
    } else {
       fprintf(stderr, 
           "Assert(%s) failed in file %s (included from %s), line %d\n", 
           exp, file, baseFile, line);
    }
}

#endif

