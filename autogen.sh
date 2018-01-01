#!/bin/sh
autoreconf -v --force --install && {
  [ -n "$NOCONFIGURE" ] || ./configure "$@"
}
