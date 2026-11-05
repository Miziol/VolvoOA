#include "androidAutoDevice.h"

AndroidAutoDevice::AndroidAutoDevice(
    QObject *parent,
    const char *logCategory,
    boost::asio::io_context &new_ioService,
    f1x::openauto::autoapp::service::AndroidAutoEntityFactory &new_androidAutoEntityFactory)
    : QObject(parent),
      category(logCategory),
      ioService(new_ioService),
      androidAutoEntityFactory(new_androidAutoEntityFactory),
      androidAutoEntity(nullptr) {}
