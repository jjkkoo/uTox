#!/bin/sh
set -eux

. ./extra/gitlab/env.sh

cmake . -DCMAKE_INCLUDE_PATH=$CACHE_DIR/usr/lib -DENABLE_TESTS=OFF -DENABLE_WERROR=OFF
make VERBOSE=1
