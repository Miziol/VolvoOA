#include "androidAutoService.h"

#include "androidAutoDevice.h"

AndroidAutoService::AndroidAutoService() : category("ANDROID AUTO SERVICE") {
    createSSLContext();

    // Services
}

AndroidAutoService::~AndroidAutoService() {
    // TODO clear children SSL
    clearSSLContext();
}

void AndroidAutoService::addDevice(libusb_device *device, libusb_device_descriptor descriptor) {
    AAdevices.append(new AndroidAutoDevice(device, descriptor, context));

    cinfo << "Adding device";
    emit aaDevicesChanged();
}

void AndroidAutoService::createSSLContext() {
    cinfo << "Creating SSL context";

    BIO *bio;
    // X509
    bio = BIO_new_mem_buf(certificate.toLatin1(), certificate.size());
    if (bio == nullptr) {
        cerror << "Failed to open BIO for certificate reading";
    }
    serverCertificate = PEM_read_bio_X509(bio, nullptr, nullptr, nullptr);
    BIO_free(bio);

    // PKEY
    bio = BIO_new_mem_buf(key.toLatin1(), key.size());
    if (bio == nullptr) {
        cerror << "Failed to open BIO for private key reading";
    }
    serverKey = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr);
    BIO_free(bio);

    // Method
    sslMethod = TLS_server_method();

    // Context
    context = SSL_CTX_new(sslMethod);
}

void AndroidAutoService::clearSSLContext() {
    SSL_CTX_free(context);
    EVP_PKEY_free(serverKey);
    X509_free(serverCertificate);
}
