#-------------------------------------------------
#
# Project created by QtCreator 2016-02-29T10:17:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hmb-photocrop
TEMPLATE = app

CONFIG(release, release|debug) {
	win32-msvc*{
		message(Place release binaries to $$PWD/bin/msvs)
		DESTDIR = $$PWD/bin/msvs
	}

	win32-g++ {
		message(Place release binaries to $$PWD/bin/mingw)
		DESTDIR = $$PWD/bin/mingw
	}
}

win32-g++ {
	QMAKE_CXXFLAGS += -std=gnu++0x
#	QMAKE_LFLAGS += -static
#	QMAKE_LFLAGS += -static-libstdc++
#	QMAKE_LFLAGS += -static-libgcc
}

SOURCES += src/main.cpp\
		src/MainWindow.cpp \
	src/ImageContainer.cpp \
	src/GraphicsRibbonItem.cpp \
	src/CropGraphicsView.cpp \
	src/ApplicationParams.cpp \
	src/AspectInfo.cpp \
    src/ImagePreview.cpp \
    src/ImagesList.cpp

HEADERS  += src/MainWindow.h \
	src/ImageContainer.h \
	src/GraphicsRibbonItem.h \
	src/CropGraphicsView.h \
	src/ApplicationParams.h \
	src/AspectInfo.h \
    src/ImagePreview.h \
    src/ImagesList.h

FORMS    += src/MainWindow.ui

RESOURCES += \
	res/images.qrc \
	res/i18n.qrc

TRANSLATIONS = i18n/hmb-photocrop_ru.ts

OTHER_FILES = \
	i18n/hmb-photocrop_ru.ts \
	i18n/hmb-photocrop_ru.qm

DISTFILES += \
	README.md \
	LICENSE \
    VERSION.txt
