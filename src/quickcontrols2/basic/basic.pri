HEADERS += \
    $$PWD/qquickbasicstyle_p.h \
    $$PWD/qquickbasictheme_p.h

SOURCES += \
    $$PWD/qquickbasicstyle.cpp \
    $$PWD/qquickbasictheme.cpp

QML_FILES += \
    $$PWD/AbstractButton.qml \
    $$PWD/Action.qml \
    $$PWD/ActionGroup.qml \
    $$PWD/ApplicationWindow.qml \
    $$PWD/BusyIndicator.qml \
    $$PWD/Button.qml \
    $$PWD/ButtonGroup.qml \
    $$PWD/CheckBox.qml \
    $$PWD/CheckDelegate.qml \
    $$PWD/ComboBox.qml \
    $$PWD/Container.qml \
    $$PWD/Control.qml \
    $$PWD/DelayButton.qml \
    $$PWD/Dial.qml \
    $$PWD/Dialog.qml \
    $$PWD/DialogButtonBox.qml \
    $$PWD/Drawer.qml \
    $$PWD/Frame.qml \
    $$PWD/GroupBox.qml \
    $$PWD/HorizontalHeaderView.qml \
    $$PWD/ItemDelegate.qml \
    $$PWD/Label.qml \
    $$PWD/Menu.qml \
    $$PWD/MenuBar.qml \
    $$PWD/MenuBarItem.qml \
    $$PWD/MenuItem.qml \
    $$PWD/MenuSeparator.qml \
    $$PWD/Page.qml \
    $$PWD/PageIndicator.qml \
    $$PWD/Pane.qml \
    $$PWD/Popup.qml \
    $$PWD/ProgressBar.qml \
    $$PWD/RadioButton.qml \
    $$PWD/RadioDelegate.qml \
    $$PWD/RangeSlider.qml \
    $$PWD/RoundButton.qml \
    $$PWD/ScrollBar.qml \
    $$PWD/ScrollIndicator.qml \
    $$PWD/ScrollView.qml \
    $$PWD/SelectionRectangle.qml \
    $$PWD/Slider.qml \
    $$PWD/SpinBox.qml \
    $$PWD/SplitView.qml \
    $$PWD/StackView.qml \
    $$PWD/SwipeDelegate.qml \
    $$PWD/Switch.qml \
    $$PWD/SwitchDelegate.qml \
    $$PWD/SwipeView.qml \
    $$PWD/TabBar.qml \
    $$PWD/TabButton.qml \
    $$PWD/TextArea.qml \
    $$PWD/TextField.qml \
    $$PWD/ToolBar.qml \
    $$PWD/ToolButton.qml \
    $$PWD/ToolSeparator.qml \
    $$PWD/ToolTip.qml \
    $$PWD/Tumbler.qml \
    $$PWD/VerticalHeaderView.qml

qtConfig(quick-treeview) {
    QML_FILES += \
        $$PWD/TreeViewDelegate.qml \
}
