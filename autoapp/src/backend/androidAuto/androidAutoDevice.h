#ifndef AUTOAPP_ANDROIDAUTODEVICE_H
#define AUTOAPP_ANDROIDAUTODEVICE_H

#include <QObject>

#include "../logging/loggingCategory.h"
#include "f1x/openauto/autoapp/Service/AndroidAutoEntity.hpp"
#include "f1x/openauto/autoapp/Service/AndroidAutoEntityFactory.hpp"

class AndroidAutoDevice : public QObject {
    Q_OBJECT

protected:
    AndroidAutoDevice(QObject *parent,
                      const char *logCategory,
                      boost::asio::io_context &new_ioService,
                      f1x::openauto::autoapp::service::AndroidAutoEntityFactory &new_androidAutoEntityFactory);

    QLoggingCategory category;
    boost::asio::io_context &ioService;
    f1x::openauto::autoapp::service::AndroidAutoEntityFactory &androidAutoEntityFactory;
    f1x::openauto::autoapp::service::IAndroidAutoEntity::Pointer androidAutoEntity;

public slots:
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
};

#endif  // AUTOAPP_ANDROIDAUTODEVICE_H
