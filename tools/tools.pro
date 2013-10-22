TEMPLATE = subdirs
SUBDIRS += \
    qmlmin
!android|android_app {
    SUBDIRS += \
        qml \
        qmlprofiler \
        qmlbundle
    qtHaveModule(quick) {
        SUBDIRS += qmlscene qmlplugindump
        qtHaveModule(widgets): SUBDIRS += qmleasing
    }
    qtHaveModule(qmltest): SUBDIRS += qmltestrunner
    contains(QT_CONFIG, private_tests): SUBDIRS += qmljs
}

# qmlmin & qmlbundle are build tools.
# qmlscene is needed by the autotests.
# qmltestrunner may be useful for manual testing.
# qmlplugindump cannot be a build tool, because it loads target plugins.
# The other apps are mostly "desktop" tools and are thus excluded.
qtNomakeTools( \
    qmlprofiler \
    qmlplugindump \
    qmleasing \
)

qtHaveModule(quick) {
    for(subdir, SUBDIRS): $${subdir}.depends += qmlimportscanner
    SUBDIRS += qmlimportscanner
}
