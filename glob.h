#ifndef GLOB_H_
#define GLOB_H_

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  GLOB_UNMATCHED = 0,
  GLOB_MATCHED,
  GLOB_SYNTAX_ERROR,
} Glob_Result;

const char *glob_result_display(Glob_Result result);
char *replaceWord(char *str, char *oldWord, char *newWord);
char *replaceAllClasses(char *pattern);
Glob_Result glob(char *pattern, const char *text);

#endif  // GLOB_H_

#ifdef GLOB_IMPLEMENTATION
const char *glob_result_display(Glob_Result result) {
  switch (result) {
    case GLOB_MATCHED:
      return "\e[0;32mGLOB_MATCHED\e[0m";
    case GLOB_UNMATCHED:
      return "\e[0;31mGLOB_UNMATCHED\e[0m";
    case GLOB_SYNTAX_ERROR:
      return "\e[0;34mGLOB_SYNTAX_ERROR\e[0m";
    default:
      assert(0 && "Unreachable");
  }
}

char *replaceWord(char *str, char *oldWord, char *newWord) {
  char *result;
  int i, count = 0;
  int newWordLen = strlen(newWord);
  int oldWordLen = strlen(oldWord);

  // Counting the number of times old word occur in the string
  for (i = 0; str[i] != '\0'; i++) {
    if (strstr(&str[i], oldWord) == &str[i]) {
      count++;
      // Jumping to index after the old word
      i += oldWordLen - 1;
    }
  }

  // Allocating space for new result string
  result = (char *)malloc(i + count * (newWordLen - oldWordLen) + 1);
  if (!result) {
    printf("Memory allocation failed\n");
    exit(1);
  }

  i = 0;
  while (*str) {
    // Compare the substring with the result
    if (strstr(str, oldWord) == str) {
      strcpy(&result[i], newWord);
      i += newWordLen;
      str += oldWordLen;
    } else {
      result[i++] = *str++;
    }
  }

  result[i] = '\0';
  return result;
}

char *replaceAllClasses(char *pattern) {
  // Alphanumeric [:alnum:] [A-Za-z0-9]
  pattern = replaceWord(pattern, "[:alnum:]", "A-Za-z0-9");
  // Alphabetic   [:alpha:] [A-Za-z]
  pattern = replaceWord(pattern, "[:alpha:]", "A-Za-z");
  // space and tab [:blank:] [ \t]
  pattern = replaceWord(pattern, "[:blank:]", "\t");
  // Control characters [:cntrl:] [\x00-\x1F\x7F]
  pattern = replaceWord(pattern, "[:cntrl:]", "\x00-\x1F");
  // Digit [:digit:] [0-9]
  pattern = replaceWord(pattern, "[:digit:]", "0-9");
  // Visible ch [:graph:] [\x21-\x7E]
  pattern = replaceWord(pattern, "[:graph:]", "\x21-\x7E");
  // Lowercase [:lower:] [a-z]
  pattern = replaceWord(pattern, "[:lower:]", "a-z");
  // visible and space [:print:] [\x20-\x7E]
  pattern = replaceWord(pattern, "[:print:]", "\x20-\x7E");
  // punctuation [:punct:] [][!"#$%&'()*+,./:;<=>?@\^_`{|}~-]
  pattern =
      replaceWord(pattern, "[:punct:]", "][!\"#$%%&'()*+,./:;<=>?@\\^_`{|}~-");
  // whitespace [:space:] [ \t\r\n\v\f]
  pattern = replaceWord(pattern, "[:space:]", " \t\r\n\v\f");
  // upper-case [:upper:] [A-Z]
  pattern = replaceWord(pattern, "[:upper:]", "A-Z");
  // hexadecimal [:xdigit:] [A-Fa-f0-9]
  pattern = replaceWord(pattern, "[:xdigit:]", "A-Fa-f0-9");

  return pattern;
}

Glob_Result glob(char *pattern, const char *text) {
  pattern = replaceAllClasses(pattern);

  //   printf("%s\n", pattern);

  while (*pattern != '\0' && *text != '\0') {
    switch (*pattern) {
      case '?': {
        pattern += 1;
        text += 1;
      } break;

      case '*': {
        Glob_Result result = glob(pattern + 1, text);
        if (result != GLOB_UNMATCHED) return result;
        text += 1;
      } break;

      case '[': {
        pattern += 1;
        bool matched = false;
        bool negated = false;

        if (*pattern == '\0') return GLOB_SYNTAX_ERROR;

        if (*pattern == '!') {
          negated = true;
          pattern += 1;
          if (*pattern == '\0') return GLOB_SYNTAX_ERROR;
        }

        matched |= *pattern == *text;
        char prev = *pattern;
        pattern += 1;

        while (*pattern != ']' && *pattern != '\0') {
          switch (*pattern) {
            case '-': {
              pattern += 1;
              switch (*pattern) {
                case ']': {
                  matched |= '-' == *text;
                } break;
                case '\0':
                  return GLOB_SYNTAX_ERROR;
                default: {
                  matched |= prev <= *text && *text <= *pattern;
                  prev = *pattern;
                  pattern += 1;
                }
              }
            } break;
            default: {
              matched |= *pattern == *text;
              prev = *pattern;
              pattern += 1;
            }
          }
        }

        if (*pattern != ']') return GLOB_SYNTAX_ERROR;

        if (negated) matched = !matched;

        if (!matched) return GLOB_UNMATCHED;

        pattern += 1;
        text += 1;
      } break;
      case '\\': {
        pattern += 1;
        if (*pattern == '\0') return GLOB_SYNTAX_ERROR;
      }  // fall through
      default: {
        if (*pattern == *text) {
          pattern += 1;
          text += 1;
        } else {
          return GLOB_UNMATCHED;
        }
      }
    }
  }

  if (*text == '\0') {
    // "" "*" "**" "***"
    while (*pattern == '*') pattern += 1;
    return *pattern == '\0';
  }

  return GLOB_UNMATCHED;
}

#endif  // GLOB_IMPLEMENTATION