#pragma once

#include <QtGlobal>

#if defined YAPSLIB_EXPORTS
#define YAPSLIB_API Q_DECL_EXPORT
#else
#define YAPSLIB_API Q_DECL_IMPORT
#endif
