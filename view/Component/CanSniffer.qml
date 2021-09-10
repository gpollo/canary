import Canary 1.0
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

ListView {
    id: view
    width: parent.width
    height: 600

    function decimalToHex(d, padding) {
        var hex = Number(d).toString(16);
        padding = typeof (padding) === "undefined" || padding === null ? padding = 2 : padding;

        while (hex.length < padding) {
            hex = "0" + hex;
        }

        return hex.toUpperCase();
    }

    model: CanSnifferModel {}

    delegate: Column {
        width: view.width
        leftPadding: 10
        rightPadding: 10

        RowLayout {
            width: parent.width - parent.leftPadding - parent.rightPadding

            Button {
                Layout.alignment: Qt.AlignVCenter
                width: parent.height / 1.5
                height: parent.height / 1.5
                display: AbstractButton.IconOnly
                // enabled: (Object.getOwnPropertyNames(modelData.signals).length != 0)
                icon.source: (enabled) ? ((expanded) ? "qrc:/resources/Icons/ChevronDoubleLeft.svg"
                                                     : "qrc:/resources/Icons/ChevronDoubleRight.svg") : ""
                flat: true
                hoverEnabled : false
                onClicked: expanded = !expanded
            }

            Text {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.preferredWidth: 2
                text: timestamp
                font.pointSize: 11
            }

            Text {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.preferredWidth: 1
                text: identifier
                font.pointSize: 11
                horizontalAlignment: Text.AlignHCenter
            }

            Row {
                id: bytesRow
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true
                Layout.preferredWidth: 2

                Repeater {
                    model: bytes

                    Text {
                        text: decimalToHex(modelData)
                        font.pointSize: 11
                        width: bytesRow.width/8
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
        }

        Repeater {
            model: values

            RowLayout {
                width: view.width

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 1
                    height: 10
                }

                Text {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 1
                    text: modelData[0]
                }

                Text {
                    Layout.fillWidth: true
                    Layout.preferredWidth: 1
                    text: modelData[1]
                }

            }
        }
    }
}