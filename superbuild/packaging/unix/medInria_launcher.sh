#!/bin/bash

usage() {
    local ret="$1"
    echo "Usage: $0 [--prefix prefix_command] [--debug] [--help] parameters"
    exit ${ret}
}

locate() {
    local SOURCE="${BASH_SOURCE[0]}"
    while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
        local DIR="$(cd -P "$( dirname "$SOURCE" )" && pwd)"
        SOURCE="$(readlink "$SOURCE")"
        # If $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
        [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"
    done
    DIR="$(cd -P "$( dirname "$SOURCE" )/.." && pwd)"
    echo ${DIR}
}


if [ "$1" == "--help" ]; then
    usage 0
fi

med_prefix=""
if [ "$1" == "--prefix" ]; then
	shift
	med_prefix="$1"
    shift
fi

if [ "$1" == "--debug" ]; then
    med_prefix="gdb --args"
fi

#   Locate the directory containing this script.

MEDINRIA_DIR="$(locate)"
MEDINRIA_BIN="${MEDINRIA_DIR}/bin/medInria.app/Contents/MacOS/medInria"

#   Set the plugins and library paths.
export MEDINRIA_PLUGINS_DIR="${MEDINRIA_DIR}/plugins:${MEDINRIA_DIR}/bin/plugins:${MEDINRIA_USER_PLUGINS_DIRS}"
export MEDINRIA_PLUGINS_DIR_LEGACY="${MEDINRIA_DIR}/plugins_legacy:${MEDINRIA_DIR}/bin/plugins_legacy:${MEDINRIA_USER_PLUGINS_DIRS_LEGACY}"
export LD_LIBRARY_PATH=${MEDINRIA_DIR}/lib:${MEDINRIA_DIR}/lib64:${MEDINRIA_DIR}/plugins_legacy:$LD_LIBRARY_PATH

#   Call medInria

exec ${med_prefix} "${MEDINRIA_BIN}" $*
