#!/usr/bin/env bash
MOD_JUNK_TO_GOLD_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/" && pwd )"

source $MOD_JUNK_TO_GOLD_ROOT"/conf/conf.sh.dist"

if [ -f $MOD_JUNK_TO_GOLD_ROOT"/conf/conf.sh" ]; then
    source $MOD_JUNK_TO_GOLD_ROOT"/conf/conf.sh"
fi
