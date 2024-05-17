#include <stdio.h>

#define GLOB_IMPLEMENTATION
#include "glob.h"

#define check_glob(p, t, e) check_glob_log(__FILE__, __LINE__, p, t, e)

void check_glob_log(const char *file, int line, char *pattern, const char *text,
                    Glob_Result expected) {
  Glob_Result actual = glob(pattern, text);

  printf("%12s <=> %-12s ---> %s\n", pattern, text,
         glob_result_display(actual));

  if (actual != expected) {
    printf(
        "%s:%d:\n\t"
        "\e[0;32mERROR\e[0;m: FAILURE! Expected %7s\n",
        file, line, glob_result_display(expected));
    exit(1);
  }
}

int main(void) {
  check_glob("main.?", "main.c", GLOB_MATCHED);
  check_glob("index.?", "main.c", GLOB_UNMATCHED);
  printf("\n");
  check_glob("?at", "Cat", GLOB_MATCHED);
  check_glob("?at", "cat", GLOB_MATCHED);
  check_glob("?at", "Bat", GLOB_MATCHED);
  check_glob("?at", "bat", GLOB_MATCHED);
  check_glob("?at", "at", GLOB_UNMATCHED);
  printf("\n");
  check_glob("*", "main.c", GLOB_MATCHED);
  check_glob("*.c", "main.c", GLOB_MATCHED);
  check_glob("*.c", "index.c", GLOB_MATCHED);
  check_glob("*.c", "test.c", GLOB_MATCHED);
  check_glob("*.js", "main.c", GLOB_UNMATCHED);
  printf("\n");
  check_glob("Law*", "Law", GLOB_MATCHED);
  check_glob("Law*", "Laws", GLOB_MATCHED);
  check_glob("Law*", "Lawyer", GLOB_MATCHED);
  check_glob("Law*", "GrokLaw", GLOB_UNMATCHED);
  check_glob("Law*", "La", GLOB_UNMATCHED);
  check_glob("Law*", "aw", GLOB_UNMATCHED);
  printf("\n");
  check_glob("*Law*", "Law", GLOB_MATCHED);
  check_glob("*Law*", "GrokLaw", GLOB_MATCHED);
  check_glob("*Law*", "Lawyer", GLOB_MATCHED);
  check_glob("*Law*", "La", GLOB_UNMATCHED);
  check_glob("*Law*", "aw", GLOB_UNMATCHED);
  printf("\n");
  check_glob("*.[abc]", "main.a", GLOB_MATCHED);
  check_glob("*.[abc]", "main.b", GLOB_MATCHED);
  check_glob("*.[abc]", "main.c", GLOB_MATCHED);
  check_glob("*.[abc]", "main.d", GLOB_UNMATCHED);
  check_glob("*.[abc", "main.a", GLOB_SYNTAX_ERROR);
  printf("\n");
  check_glob("[CB]at", "Cat", GLOB_MATCHED);
  check_glob("[CB]at", "Bat", GLOB_MATCHED);
  check_glob("[CB]at", "cat", GLOB_UNMATCHED);
  check_glob("[CB]at", "bat", GLOB_UNMATCHED);
  check_glob("[CB]at", "CBat", GLOB_UNMATCHED);
  printf("\n");
  check_glob("[][!]", "]", GLOB_MATCHED);
  check_glob("[][!]", "[", GLOB_MATCHED);
  check_glob("[][!]", "!", GLOB_MATCHED);
  check_glob("[][!]", "a", GLOB_UNMATCHED);
  check_glob("[][!", "]", GLOB_SYNTAX_ERROR);
  printf("\n");
  check_glob("Letter[0-9]", "Letter0", GLOB_MATCHED);
  check_glob("Letter[0-9]", "Letter1", GLOB_MATCHED);
  check_glob("Letter[0-9]", "Letter2", GLOB_MATCHED);
  check_glob("Letter[0-9]", "Letter9", GLOB_MATCHED);
  printf("\n");
  check_glob("Letter[0-9]", "Letters", GLOB_UNMATCHED);
  check_glob("Letter[0-9]", "Letter", GLOB_UNMATCHED);
  check_glob("Letter[0-9]", "Letter10", GLOB_UNMATCHED);
  printf("\n");
  check_glob("[A-Fa-f0-9]", "A", GLOB_MATCHED);
  check_glob("[A-Fa-f0-9]", "a", GLOB_MATCHED);
  check_glob("[A-Fa-f0-9]", "B", GLOB_MATCHED);
  check_glob("[A-Fa-f0-9]", "b", GLOB_MATCHED);
  check_glob("[A-Fa-f0-9]", "0", GLOB_MATCHED);
  check_glob("[A-Fa-f0-9]", "-", GLOB_UNMATCHED);
  printf("\n");
  check_glob("[]-]", "]", GLOB_MATCHED);
  check_glob("[]-]", "-", GLOB_MATCHED);
  check_glob("[]-]", "a", GLOB_UNMATCHED);
  printf("\n");
  check_glob("[--0]", "-", GLOB_MATCHED);
  check_glob("[--0]", ".", GLOB_MATCHED);
  check_glob("[--0]", "0", GLOB_MATCHED);
  check_glob("[--0]", "/", GLOB_MATCHED);
  check_glob("[--0]", "a", GLOB_UNMATCHED);
  printf("\n");
  check_glob("[!]a-]", "b", GLOB_MATCHED);
  check_glob("[!]a-]", "]", GLOB_UNMATCHED);
  check_glob("[!]a-]", "a", GLOB_UNMATCHED);
  check_glob("[!]a-]", "-", GLOB_UNMATCHED);
  printf("\n");
  check_glob("[[?*\\]", "[", GLOB_MATCHED);
  check_glob("[[?*\\]", "?", GLOB_MATCHED);
  check_glob("[[?*\\]", "*", GLOB_MATCHED);
  check_glob("[[?*\\]", "\\", GLOB_MATCHED);
  check_glob("[[?*\\]", "a", GLOB_UNMATCHED);
  printf("\n");
  check_glob("\\*", "*", GLOB_MATCHED);
  check_glob("\\*", "?", GLOB_UNMATCHED);
  check_glob("\\?", "?", GLOB_MATCHED);
  check_glob("\\?", "a", GLOB_UNMATCHED);
  check_glob("\\*", "?", GLOB_UNMATCHED);
  printf("\n");
  check_glob("[", "aaaaa", GLOB_SYNTAX_ERROR);
  check_glob("[!", "aaaaa", GLOB_SYNTAX_ERROR);
  check_glob("[--", "aaaaa", GLOB_SYNTAX_ERROR);
  check_glob("\\", "aaaaa", GLOB_SYNTAX_ERROR);
  printf("\n");
  check_glob("[[:alpha:]]", "a", GLOB_MATCHED);
  check_glob("[[:alpha:]]", "A", GLOB_MATCHED);
  check_glob("[[:alpha:]]", "z", GLOB_MATCHED);
  check_glob("[[:alpha:]]", "Z", GLOB_MATCHED);
  check_glob("[[:alpha:]]", "3", GLOB_UNMATCHED);
  check_glob("[[:alpha:]", "A", GLOB_SYNTAX_ERROR);
  printf("\n");
  check_glob("[[:space:][:alpha:]]", "a", GLOB_MATCHED);
  check_glob("[[:space:][:alpha:]]", "\n", GLOB_MATCHED);
  check_glob("[[:space:][:alpha:]]", "\t", GLOB_MATCHED);

  return 0;
}
