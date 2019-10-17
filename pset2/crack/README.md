# Crack

[Walkthrough Intros - Crack](https://youtu.be/NkUf9vj1iFA)

## The Shadow Knows

On most systems running Linux these days is a file called `/etc/shadow` that contains usernames and passwords. Fortunately, the passwords therein aren’t stored “in the clear” but are instead encrypted using a “one-way hash function.” When a user logs into these systems by typing a username and password, the latter is encrypted with the very same hash function, and the result is compared against the username’s entry in `/etc/shadow`. If the two hashes match, the user is allowed in. If you’ve ever forgotten some password, you might have been told that tech support can’t look up your password but can change it for you. Odds are that’s because tech support can only see, if anything at all, a hash of your password, not your password itself. But they can create a new hash for you.

Even though passwords in `/etc/shadow` are hashed, the hash function is not always that strong. Quite often are adversaries, upon obtaining that file somehow, able to guess (and check) users’ passwords or crack them using brute force (i.e., trying all possible passwords). Below is what `/etc/shadow` might look like, albeit simplified, wherein each line is formatted as `username:hash`.
`brian:51.xJagtPnb6s`
`bjbrown:50GApilQSG3E2`
`emc:502sDZxA/ybHs`
`greg:50C6B0oz0HWzo`
`jana:50WUNAFdX/yjA`
`lloyd:50n0AAUD.pL8g`
`walker:508ny6Rw0aRio`
`zamyla:50cI2vYkF0YU2`

## Safecracker

Your task is to design and implement a program, `crack`, that cracks passwords. We’re not going to give too many hints on this one, but to get started you may want to read up on how the cryptfunction works on Unix/Linux systems, such as this lab environment. To do so, type:
`man crypt`

in the terminal. Take particular note of that program’s mention of “salt”.
In order to declare function crypt for use in your solution, you’ll want to put
`#include <crypt.h>`

near the top of your file.

## Specification

* Your program should accept one and only one command-line argument: a hashed password.
* If your program is executed without any command-line arguments or with more than one command-line argument, your program should print an error (of your choice) and exit immediately, with main returning 1 (thereby signifying an error).
* Otherwise, your program must proceed to crack the given password, ideally as quickly as possible, ultimately printing the password in the clear followed by \n, nothing more, nothing less, with main returning 0.
* Assume that each password has been hashed with C’s DES-based (not MD5-based) `crypt`function.
* Assume that each password is no longer than five (5) characters. Gasp!
* Assume that each password is composed entirely of alphabetical characters (uppercase and/or lowercase).

Below, then, is some example behavior.
`$ ./crack`
`Usage: ./crack hash`

`$ ./crack 50cI2vYkF0YU2`
`LOL`



# exload comment
Fully written in notepad :)
because there is no support for crypt function in online compilers such as crypt.h or -lcrypt
Only last stage was checked in CS50 IDE (like 'make', 'style50')
Programm crack DES-based hash of password crypted with crypt linux command.
Brute force.
Hard coded to crack 5-letters pass(strings).
To change length of cracked pass need to make changes in line 30 and line 39.

for Line 30: dimension of array pass = number of characters + '\0'-symbol

for Line 39: the third parameter of the called function = number of characters - 1
this is because the intended calculation in the function 'brute_hash' starts with 0

This problem is really was hard to implement.
