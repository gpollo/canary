import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import "."

Item {
    id: view
    
    property alias media: mediaLoader.sourceComponent
    property alias thumbnail: thumbnailLoader.sourceComponent
    // property Component[] buttons:
    // property Component[] icons:
    
    property string header: "My Card"
    property string subheader: ""
    property string text: ""

    height: childrenRect.height

    Rectangle {
        id: shadowRectangle

        width: parent.width
        height: childrenRect.height
        radius: 4

        Column {
            width: parent.width
            height: childrenRect.height

            Row {
                padding: 16
                topPadding: 12

                Loader {
                    id: thumbnailLoader
                    width: 40
                }

                Column {
                    leftPadding: 16

                    Text {
                        text: view.header
                        font.family: "Roboto"
                        font.pointSize: 14
                        font.weight: Font.DemiBold
                    }

                    Text {
                        text: view.subheader
                        color: Material.color(Material.BlueGrey)
                        font.pointSize: 10
                        font.family: "Roboto"
                    }
                }
            }

            Loader {
                id: mediaLoader

                width: parent.width
            }

            Column {
                width: parent.width
                padding: !(view.text == "") ? 16 : 8

                Text {
                    width: parent.width - 2 * parent.padding
                    wrapMode: Text.Wrap
                    visible: !(view.text == "")

                    text: view.text
                    color: Material.color(Material.BlueGrey)
                    font.pointSize: 10
                    font.family: "Roboto"
                }
            }
        }
    }

    DropShadow {
        anchors.fill: shadowRectangle
        horizontalOffset: -1
        verticalOffset: 2
        radius: 6.0
        samples: 17
        color: "#60000000"
        source: shadowRectangle
    }
}
