import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.12

import "qrc:/view/Component" as Component
import "qrc:/view/Component/Can" as CanComponent
import "qrc:/view/Material" as Material

ApplicationWindow {
    id: window
    visible: true

    header: ToolBar {
        ToolButton {
            icon.source: "qrc:/resources/Icons/OpenDrawer.svg"

            onClicked: drawer.open()
        }
    }

    CanComponent.BusConfiguration {
        id: busConfiguration
    }

    CanComponent.DatabaseConfigurationDialog {
        id: databaseConfigurationDialog
    }

    Drawer {
        id: drawer

        width: (0.66 * window.width > 300) ? 300 : (0.66 * window.width)
        height: window.height

        ScrollView {
            anchors.fill: parent

            Column {
                width: parent.width

                Button {
                    text: "Bus Configuration"
                    flat: true
                    width: parent.width

                    onClicked: busConfiguration.open()
                }

                Button {
                    text: "Database Configuration"
                    flat: true
                    width: parent.width

                    onClicked: databaseConfigurationDialog.open()
                }
            }
        }
    }

    Material.MaterialCard {
        width: 600

        header: "CAN Bus"
        subheader: "Test"

        thumbnail: Image {
            source: "qrc:/resources/Icons/OpenDrawer.svg"
            sourceSize.height: width
        }
        
        media: Component.CanSniffer {}
    }

    /* FIXME: toasts don't appear above the drawer */
    Material.ToastManager {
        id: toast
    }
}
