# ssu

Extremely simple su utility

## Dependencies

* C99 compiler (build time)
* POSIX make (build time)
* POSIX libc & GNU/BSD initgroups

## Installation

```sh
make
make PREFIX=/usr install
```

## Usage

As you know, most privilege elevation utilities uses config-based approach for
authentication mechanism. Instead of that, ssu relies on file metadata which
gives a simple, config-less way to authenticate.

By default, only users in `wheel` group can use ssu. Changing that behavior is
fairly simple.

```sh
# run 'id -g' to see your current group.
# change 'me' to that group.
chgrp me /path/to/ssu

# extra(4) - setuid
# owner(7) - read, write, exec
# group(5) - read, exec
# other(4) - read
chmod 4754 /path/to/ssu
```

## Note

While using ssu you may encounter weird errors like 'invalid option'. In order
to prevent such errors from pop up, you must set `POSIXLY_CORRECT` variable, or
guard command-line arguments by using '--'.

```sh
POSIXLY_CORRECT=1 ssu ls -la /root
ssu -- ls -la /root
```

### References

https://wiki.musl-libc.org/functional-differences-from-glibc.html
