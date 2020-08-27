sls
===

Suckless su and suedit

Dependencies
------------

* C99 compiler (build time)
* POSIX make (build time)
* POSIX libc

Installation
------------

```sh
make
make PREFIX=/usr install
```

Usage
-----

```sh
# change group to group of user (id -gn)
chown root:<group> path/to/sls
chmod gu+s path/to/sls
```
