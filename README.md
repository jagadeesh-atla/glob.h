# Simple Wildcard Matching Implementation

### Testing

```console
$ ./test.sh
```

## Wildcards

| Wildcard | Description                                                                  | Example        | Matches                                                            | Does not match                                |
| -------- | ---------------------------------------------------------------------------- | -------------- | ------------------------------------------------------------------ | --------------------------------------------- |
| `*`      | matches any number of any characters including none                          | `Law*`         | `Law`, `Laws`, or `Lawyer`                                         | `GrokLaw`, `La`, or `aw`                      |
| `*Law*`  | `Law`, `GrokLaw`, or `Lawyer`.                                               | `La`, or `aw`  |                                                                    |                                               |
| `?`      | matches any single character                                                 | `?at`          | `Cat`, `cat`, `Bat` or `bat`                                       | `at`                                          |
| `[abc]`  | matches one character given in the bracket                                   | `[CB]at`       | `Cat` or `Bat`                                                     | `cat`, `bat` or `CBat`                        |
| `[a-z]`  | matches one character from the (locale-dependent) range given in the bracket | `Letter[0-9]`  | `Letter0`, `Letter1`, `Letter2` up to `Letter9`                    | `Letters`, `Letter` or `Letter10`             |
| `[!abc]` | matches one character that is not given in the bracket                       | `[!C]at`       | `Bat`, `bat`, or `cat`                                             | `Cat`                                         |
| `[!a-z]` | matches one character that is not from the range given in the bracket        | `Letter[!3-5]` | `Letter1`, `Letter2`, `Letter6` up to `Letter9` and `Letterx` etc. | `Letter3`, `Letter4`, `Letter5` or `Letterxx` |

### Character classes

To use inside `[[:alpha:]]`.

| Description          | POSIX       | ASCII         |
| -------------------- | ----------- | ------------- |
| Alphanumeric         | `[:alnum:]` | `[A-Za-z0-9]` |
| Alphabetic character | `[:alpha:]` | `[A-Za-z]`    |
| Space and tab        | `[:blank:]` | `[ \t]`       |

---
