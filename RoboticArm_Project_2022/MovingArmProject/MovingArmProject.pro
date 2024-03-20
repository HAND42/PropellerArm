QT      += core gui
QT    += charts
QT    += serialport

QT    += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arm_prop_controller.cpp \
    arm_prop_simulator.cpp \
    hilsModeSerialCommunicator.cpp \
    main.cpp \
    mainwindow.cpp \
    model_file_setting.cpp \
    model_formule.cpp \
    model_setting_PC_controller.cpp \
    model_setting_sample_time.cpp \
    model_time.cpp \
    serialCommunicator2.cpp \
    threadSimulatorController.cpp \
    verification_saisie_utilisateur.cpp \
    view_about.cpp \
    view_fly_arm.cpp \
    model_draw.cpp \
    view_formule.cpp \
    view_formule_info.cpp \
    view_graph.cpp \
    view_messages.cpp \
    view_setting_PC_controller.cpp \
    view_setting_sample_time.cpp

HEADERS += \
    arm_prop_controller.h \
    arm_prop_simulator.h \
    hilsModeSerialCommunicator.h \
    include_define.h \
    mainwindow.h \
    model_file_setting.h \
    model_formule.h \
    model_setting_PC_controller.h \
    model_setting_sample_time.h \
    model_time.h \
    serialCommunicator2.h \
    threadSimulatorController.h \
    verification_saisie_utilisateur.h \
    view_about.h \
    view_fly_arm.h \
    model_draw.h \
    view_formule.h \
    view_formule_info.h \
    view_graph.h \
    view_messages.h \
    view_setting_PC_controller.h \
    view_setting_sample_time.h

FORMS += \
    mainwindow.ui \
    view_about.ui \
    view_fly_arm.ui \
    view_fly_arm.ui \
    view_formule.ui \
    view_formule_info.ui \
    view_setting_PC_controller.ui \
    view_setting_sample_time.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    app.ico \
    app_icone.rc \
    file_setting.txt \
    infos.txt

RC_FILE += app_icone.rc

RESOURCES += \
    ressources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../v0.2-ressources_communes/Hexa_PC_com/ -lftd2xx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../v0.2-ressources_communes/Hexa_PC_com/ -lftd2xx

INCLUDEPATH += $$PWD/../v0.2-ressources_communes/Hexa_PC_com
DEPENDPATH += $$PWD/../v0.2-ressources_communes/Hexa_PC_com
