CONFIG(debug, debug|release) {
	MODE = debug
}
CONFIG(release, debug|release) {
	MODE = release
}


TEMPLATE = app
TARGET = UJPS
CONFIG += c++11
DESTDIR = $$MODE
OBJECTS_DIR = $$DESTDIR/objects
MOC_DIR = $$DESTDIR/moc
QT = core xmlpatterns gui widgets
DEFINES -= QT_NO_DEBUG


win32-g++:LIBS += ../StaticLibs/SOURCES/QtGameControllerModif/$$MODE/libQtGameControllerModif.a
win32-g++:LIBS += ../StaticLibs/SOURCES/VirtualJoysticks/$$MODE/libvirtualJoysticks.a
win32-g++:LIBS += ../StaticLibs/SOURCES/UjpsCore/$$MODE/libujpsCore.a
win32-g++:LIBS += ../vJoy218SDK-291116/SDK/lib/vJoyInterface.lib

win32-msvc*:LIBS += ../StaticLibs/SOURCES/QtGameControllerModif/$$MODE/QtGameControllerModif.lib
win32-msvc*:LIBS += ../StaticLibs/SOURCES/VirtualJoysticks/$$MODE/virtualJoysticks.lib
win32-msvc*:LIBS += ../StaticLibs/SOURCES/UjpsCore/$$MODE/ujpsCore.lib
win32-msvc*:LIBS += ../vJoy218SDK-291116/SDK/lib/amd64/vJoyInterface.lib


INCLUDEPATH +=  ../StaticLibs/INCLUDES/QtGameControllerModif/ \
				../StaticLibs/INCLUDES/Other/ \
				../StaticLibs/INCLUDES/VirtualJoysticks/ \
				../StaticLibs/INCLUDES/RealJoysticks/ \
				../StaticLibs/INCLUDES/UjpsCore/


HEADERS +=  CODE/ProfileEngine.h \
			CODE/VigemPidWhiteLister.h \
			CODE/HMI/MainWindow.h \
			CODE/HMI/TextEdit.h \
			CODE/COMPILER/AbstractCompiler.h \
			CODE/COMPILER/QtCompiler.h \
			CODE/COMPILER/QtCompilationProcess.h \
			CODE/COMPILER/CompilationWidget.h \
			CODE/SETTINGS/ApplicationSettings.h \
			CODE/SETTINGS/AbstractSettingsWidget.h \
			CODE/SETTINGS/SettingsDialog.h \
			CODE/SETTINGS/MyFileDialog.h \
			CODE/SETTINGS/GeneralSettingsWidget.h \
			CODE/SETTINGS/VJoySettingsWidget.h \
			CODE/SETTINGS/VigemSettingsWidget.h \
			CODE/XML/AbstractXmlInfo.h \
			CODE/XML/XmlValidationMessageHandler.h \
			CODE/XML/GenericPropertiesInfo.h
			
			
SOURCES +=  CODE/main.cpp \
			CODE/VigemPidWhiteLister.cpp \
			CODE/ProfileEngine.cpp \
			CODE/otherFunctions.cpp \
			CODE/HMI/MainWindow.cpp \
			CODE/HMI/TextEdit.cpp \
			CODE/COMPILER/QtCompiler.cpp \
			CODE/COMPILER/QtCompilationProcess.cpp \
			CODE/COMPILER/CompilationWidget.cpp \
			CODE/SETTINGS/ApplicationSettings.cpp \
			CODE/SETTINGS/SettingsDialog.cpp \
			CODE/SETTINGS/MyFileDialog.cpp \
			CODE/SETTINGS/GeneralSettingsWidget.cpp \
			CODE/SETTINGS/VJoySettingsWidget.cpp \
			CODE/SETTINGS/VigemSettingsWidget.cpp \
			CODE/XML/AbstractXmlInfo.cpp \
			CODE/XML/XmlValidationMessageHandler.cpp \
			CODE/XML/GenericPropertiesInfo.cpp


RESOURCES += resources.qrc
RC_FILE += icone.rc

