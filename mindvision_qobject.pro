TEMPLATE = subdirs

SUBDIRS += \
    mindvision_lib \
    mindvision_py

mindvision_lib.file = mindvision_lib.pro
mindvision_py.file = mindvision_py.pro

mindvision_py.depends = mindvision_lib