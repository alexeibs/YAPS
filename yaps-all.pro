TEMPLATE = subdirs

SUBDIRS += \
    cryptopp \
    kbhook \
    yaps \
    yapslib \
    tests

yapslib.depends = cryptopp kbhook
yaps.depends = yapslib
