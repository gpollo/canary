import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import Canary 1.0

GroupBox {
    property CanDatabase canDatabase
    property bool   isConnected:      (canDatabase != null) ? canDatabase.connected : false
    property string currentPath:      (canDatabase != null) ? canDatabase.path    : 0

    Column {
        width: parent.width
        
        Column {
            width: parent.width
            spacing: -7

            Text {
                width: parent.width
                text: "Path"
            }

            Item {
                width: parent.width
                height: pathTextField.implicitHeight

                TextField {
                    id: pathTextField
                    enabled: !isConnected
                    width: parent.width
                    text: currentPath
                    onEditingFinished: function() {
                        if (canDatabase != null) {
                            canDatabase.path = text
                        }
                    }
                }

                MouseArea {
                    anchors.fill: pathTextField
                    enabled: isConnected
                    z: parent.z - 1

                    ToolTip {
                        text: "Database must be disconnected"
                        visible: parent.pressed
                    }
                }
            }
        }

        RowLayout {
            width: parent.width

            Rectangle {
                color: "transparent"
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Button {
                Material.foreground: Material.color(Material.Red)
                visible: isConnected
                text: "Disconnect"
                flat: true
                onClicked: canDatabase.disconnect()
            }

            Button {
                visible: !isConnected
                text: "Connect"
                flat: true
                onClicked: canDatabase.connect()
            }

            Item {
                width: removeButton.implicitWidth
                height: removeButton.implicitHeight

                Button {
                    Material.foreground: Material.color(Material.Indigo)
                    id: removeButton
                    enabled: !isConnected && (canBus.databaseCount > 1)
                    text: "Remove"
                    flat: true
                    onClicked: canBus.removeDatabase(index)
                }

                MouseArea {
                    anchors.fill: removeButton
                    enabled: !removeButton.enabled
                    z: parent.z - 1

                    ToolTip {
                        text: isConnected ? "Database must be disconnected" : "There must at least be one database"
                        visible: parent.pressed
                    }
                }
            }
        }
    }
}