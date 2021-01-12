#pragma once

#undef slots
#define slots _slots

#undef _LARGEFILE_SOURCE

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#undef slots
#define slots Q_SLOTS
