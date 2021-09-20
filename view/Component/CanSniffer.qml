import Canary 1.0
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

ListView {
    id: view
    width: parent.width
    height: 600
    clip: true

    function decimalToHex(d, padding) {
        var hex = Number(d).toString(16);
        padding = typeof (padding) === "undefined" || padding === null ? padding = 2 : padding;

        while (hex.length < padding) {
            hex = "0" + hex;
        }

        return hex.toUpperCase();
    }

    model: CanSnifferModel {}

    ScrollBar.vertical: ScrollBar {
        active: true
    }

    delegate: Column {
        width: view.width
        leftPadding: 10
        rightPadding: 10

        Row {
            width: parent.width - parent.leftPadding - parent.rightPadding

            RoundButton {
                id: focusButton
                width: 20
                height: 20
                padding: 0
                display: AbstractButton.IconOnly
                flat: true
                hoverEnabled : false
                onClicked: focused = !focused
                icon.source: (enabled) ? ((focused) ? "qrc:/resources/Icons/Star.svg"
                                                    : "qrc:/resources/Icons/StarOutline.svg") : ""
            }

            RoundButton {
                id: expandButton
                width: 20
                height: 20
                padding: 0
                display: AbstractButton.IconOnly
                flat: true
                hoverEnabled : false
                onClicked: expanded = !expanded
                icon.source: (enabled) ? ((expanded) ? "qrc:/resources/Icons/ChevronDoubleLeft.svg"
                                                     : "qrc:/resources/Icons/ChevronDoubleRight.svg") : ""
            }

            Column {
                width: parent.width - focusButton.width - expandButton.width

                RowLayout {
                    width: parent.width

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
                        width: parent.width
                        height: expanded ? 20 : 0
                        clip: true

                        Text {
                            Layout.fillWidth: true
                            Layout.preferredWidth: 3
                            text: modelData[0]
                        }

                        Text {
                            Layout.fillWidth: true
                            Layout.preferredWidth: 2
                            text: modelData[1]
                            leftPadding: 6
                        }
                    }
                }
            }
        }


    }
}