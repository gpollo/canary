import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15

import "qrc:/view/Component/Can" as Can
import "qrc:/view/Material" as MaterialComponent

MaterialComponent.Dialog {
    id: dialog
    title: "Database Configuration"

    content: ColumnLayout {
        width: 400
        height: 400
        
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            rightPadding: 20

            ListView {
                id: databaseList
                spacing: 10
                clip: true

                model: canBus.databases

                delegate: DatabaseConfiguration{
                    width: databaseList.width
                    canDatabase: modelData
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "transparent"
            }

            Button {
                text: "Close"
                flat: true
                onClicked: dialog.close()
            }

            Button {
                Material.foreground: Material.color(Material.Indigo)
                text: "New Database"
                flat: true
                onClicked: canBus.addDatabase()
            }
        }
    }
}