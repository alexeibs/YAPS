TEMPLATE = subdirs

SUBDIRS += \
    cryptopp \
    kbhook \
    yaps \
    yapslib

yapslib.depends = cryptopp kbhook
yaps.depends = yapslib
