TEMPLATE = subdirs

SUBDIRS += \
    cryptopp \
    kbhook \
    yaps

yaps.depends = cryptopp kbhook