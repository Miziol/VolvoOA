#include <QLoggingCategory>

#define cdebug qCDebug(category)
#define cinfo qCInfo(category)
#define cwarning qCWarning(category)
#define cerror qCCritical(category)
#define cfatal qCFatal(category)