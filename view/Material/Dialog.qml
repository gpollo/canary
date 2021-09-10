import QtQuick 2.15
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12
import QtQuick.Layouts 1.15

Popup {
    id: view

    property string title: "Dialog Title"
    property alias content: contentLoader.sourceComponent
    property alias actions: actionsLoader.sourceComponent

    focus: true
    modal: true

    parent: Overlay.overlay
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    contentItem: Column {
        topPadding: 14
        leftPadding: 11
        rightPadding: leftPadding
        bottomPadding: 0

        ColumnLayout {
            Text {
                text: view.title
                font.family: "Roboto"
                font.pointSize: 14
                font.weight: Font.DemiBold
            }

            Rectangle {
                width: parent.width - 2 * parent.leftPadding
                height: 10
            }

            Loader {
                id: contentLoader

                width: parent.width - 2 * parent.leftPadding
            }

            Rectangle {
                width: parent.width - 2 * parent.leftPadding
                height: (actionsLoader.status == Loader.Null) ? 0 : 10
            }

            Loader {
                id: actionsLoader
                width: parent.width - 2 * parent.leftPadding
            }
        }
    }

    background: Rectangle {
        color: "transparent"

        Rectangle {
            id: pane
            anchors.fill: parent
            radius: 4
        }
        
        DropShadow {
            anchors.fill: pane
            horizontalOffset: -1
            verticalOffset: 2
            radius: 30.0
            samples: 50
            color: "#80000000"
            source: pane
        }
    }
}