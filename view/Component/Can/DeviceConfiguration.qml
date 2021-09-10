import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import Canary 1.0

GroupBox {
    property CanDevice canDevice
    property bool   isConnected:      (canDevice != null) ? canDevice.connected : false
    property string currentDriver:    (canDevice != null) ? canDevice.driver    : 0
    property string currentInterface: (canDevice != null) ? canDevice.interface : 0
    property int    currentBitrate:   (canDevice != null) ? canDevice.bitrate   : 0

    Column {
        width: parent.width
        
        Row {
            width: parent.width

            Column {
                width: parent.width / 2 - 2
                spacing: -7

                Text {
                    width: parent.width
                    text: "Driver"
                }

                Item {
                    width: parent.width
                    height: driverTextField.implicitHeight

                    TextField {
                        id: driverTextField
                        enabled: !isConnected
                        width: parent.width
                        text: currentDriver
                        onEditingFinished: function() {
                            if (canDevice != null) {
                                canDevice.driver = text
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: driverTextField
                        enabled: isConnected
                        z: parent.z - 1

                        ToolTip {
                            text: "Device must be disconnected"
                            visible: parent.pressed
                        }
                    }
                }
            }

            Rectangle {
                width: 4
                height: parent.height
            }

            Column {
                width: parent.width / 2 - 2
                spacing: -7

                Text {
                    width: parent.width
                    text: "Interface"
                }

                Item {
                    width: parent.width
                    height: interfaceTextField.implicitHeight

                    TextField {
                        id: interfaceTextField
                        enabled: !isConnected
                        width: parent.width
                        text: currentInterface
                        onEditingFinished: function() {
                            if (canDevice != null) {
                                canDevice.interface = text
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: interfaceTextField
                        enabled: isConnected
                        z: parent.z - 1

                        ToolTip {
                            text: "Device must be disconnected"
                            visible: parent.pressed
                        }
                    }
                }
            }
        }

        Column {
            width: parent.width
            spacing: -7

            Text {
                width: parent.width
                text: "Bitrate"
            }

            Item {
                width: parent.width
                height: bitrateTextField.implicitHeight

                TextField {
                    id: bitrateTextField
                    enabled: !isConnected
                    width: parent.width
                    text: currentBitrate
                    onEditingFinished: function() {
                        if (canDevice != null) {
                            canDevice.bitrate = text
                        }
                    }
                    validator: IntValidator {
                        bottom: 0
                        top: 2000000
                    }
                }

                MouseArea {
                    anchors.fill: bitrateTextField
                    enabled: isConnected
                    z: parent.z - 1

                    ToolTip {
                        text: "Device must be disconnected"
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
                onClicked: canDevice.disconnect()
            }

            Button {
                visible: !isConnected
                text: "Connect"
                flat: true
                onClicked: canDevice.connect()
            }

            Item {
                width: removeButton.implicitWidth
                height: removeButton.implicitHeight

                Button {
                    Material.foreground: Material.color(Material.Indigo)
                    id: removeButton
                    enabled: !isConnected && (canBus.deviceCount > 1)
                    text: "Remove"
                    flat: true
                    onClicked: canBus.removeDevice(index)
                }

                MouseArea {
                    anchors.fill: removeButton
                    enabled: !removeButton.enabled
                    z: parent.z - 1

                    ToolTip {
                        text: isConnected ? "Device must be disconnected" : "There must at least be one device"
                        visible: parent.pressed
                    }
                }
            }
        }
    }
}