# ssu

Extremely simple su utility

## Dependencies

* C99 compiler (gcc, clang, tcc, cproc, ...) (build time)
* POSIX make (gmake, bmake, smake, pdpmake, ...) (build time)
* POSIX libc + initgroups(3) (musl libc, glibc, *BSD libc, ...)

## Installation

```sh
# pass CC=cc macro if c99 is not available
make

# the default group with ability to elevate privileges is wheel.
# to change that, pass BINGRP=grp macro, where 'grp' is a desirable group.
#
# you must have appropriate privileges to run this command.
make PREFIX=/usr install
```

## Usage

Instead of using a config file, ssu offloads authentication mechanism to the
kernel by using unix permissions, which makes it simple and secure. However
simplicity comes with a cost and such mechanism is not flexible at all; it is
completely limited to the unix permissions. Therefore, ssu is only good for
those who don't need complicated setups.

Here is an example how to setup ssu for a specific group(you must be root to
run these commands):

```sh
# run 'id -gn' to see your current group.
# change 'me' to that group.
chown root:me /path/to/ssu

# allow your group to execute suid ssu and thus elevate privileges.
#
# extra(4) - setuid
# owner(7) - read, write, exec
# group(5) - read, exec
# other(4) - read
chmod 4754 /path/to/ssu
```

## Note

While using ssu, you may encounter weird errors like 'invalid option'. In order
to fix them, set `POSIXLY_CORRECT` environment variable or guard command-line
arguments with '--'.

Example with ls:

```sh
POSIXLY_CORRECT=1 ssu ls -la /root
ssu -- ls -la /root
```

See also: https://wiki.musl-libc.org/functional-differences-from-glibc.html
