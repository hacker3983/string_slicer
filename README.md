# string_slicer
String Slicer is a C command-line tool that splits a string into substrings using a delimiter, inspired by Python’s split()

# Installation and Compilation
`git clone https://github.com/hacker3983/string_slicer`
`cd string_slicer && gcc stringslicer.c -o stringslicer && ./stringslicer`

# Usage example
Usage help:
`./stringslicer`

* Output:
```
Usage: ./stringslicer [STRINGTOSPLIT]:[DELIMITERSTRING]...
String Slicer is a command-line tool that splits a string into substrings using a delimiter, inspired by Python’s split()
```

* Example of splitting a sequence of strings by delimiter:
`./stringslicer Hello:e "How are you doing: are" test: word: "john doe:ohn " jdoe:o"

* Output of splitting a sequence of strings by delimter:
```
[*] Perform split sequence for argument 1 of 6 -> "Hello:e":
["String to split": "Hello", "String to split by": "e"]
["H", "llo"]
[*] Perform split sequence for argument 2 of 6 -> "How are you doing: are":
["String to split": "How are you doing", "String to split by": " are"]
["How", " you doing"]
[*] Perform split sequence for argument 3 of 6 -> "test:":
No delimiter specified for argument string "test". Therefore cannot split
[*] Perform split sequence for argument 4 of 6 -> "word:":
No delimiter specified for argument string "word". Therefore cannot split
[*] Perform split sequence for argument 5 of 6 -> "john doe:ohn ":
["String to split": "john doe", "String to split by": "ohn "]
["j", "doe"]
[*] Perform split sequence for argument 6 of 6 -> "jdoe:o":
["String to split": "jdoe", "String to split by": "o"]
["jd", "e"]
```
