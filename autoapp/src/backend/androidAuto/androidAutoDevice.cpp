#include "androidAutoService.h"
#include "usbAndroidAutoDevice.h"

AndroidAutoDevice::AndroidAutoDevice(
    QObject *parent,
    QString logCategory,
    boost::asio::io_context &new_ioService,
    f1x::openauto::autoapp::service::AndroidAutoEntityFactory &new_androidAutoEntityFactory)
    : QObject(parent),
      category(logCategory.toStdString().c_str()),
      ioService(new_ioService),
      androidAutoEntityFactory(new_androidAutoEntityFactory),
      androidAutoEntity(nullptr) {}
