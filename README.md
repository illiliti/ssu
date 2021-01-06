# sls

Extremely simple su utility

## Dependencies

* C99 compiler (build time)
* POSIX make (build time)
* POSIX libc & initgroups

## Configuration

`sls` authenticates based on groups. It can be configured at compile-time to use
the desired group for authentication by editing `config.h`. By default, `sls`
allows the users in the `wheel` group.

## Installation

```sh
make
make PREFIX=/usr install
```

## Note

Unlike musl-based and other strict POSIX-compliant standard
libraries, glibc-based distributions have different non-POSIX behaviour
regarding `getopt(3)`[1]. In order to correctly use commands options without
annoying 'invalid option' error, you must explicitly guard them by using '--'.

### Example

```sh
sls    ls -la /root # doesn't work
sls -- la -la /root # works
```

[1] https://wiki.musl-libc.org/functional-differences-from-glibc.html
