# sls

Extremely simple su utility

## Dependencies

* C99 compiler (build time)
* POSIX make (build time)
* POSIX libc & initgroups

## Installation

```sh
make
make PREFIX=/usr install
```

## Usage

sls authentication system is based on file metadata. To authenticate you need
to change the group of sls to a group you are in. Pretty nifty, huh?

### Example

```sh
# change <grp> to group of user (see: id -gn)
chown root:<grp> path/to/sls
chmod gu+s       path/to/sls

# or make the owning group wheel, so the users in wheel can authenticate
chown root:wheel path/to/sls
chmod gu+s       path/to/sls
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
