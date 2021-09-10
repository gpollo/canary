import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Image {
    id: image

    mipmap: true
    height: (width * sourceSize.height) / sourceSize.width

    layer.enabled: true
    layer.effect: OpacityMask {
        maskSource: Item {
            width: image.width
            height: image.height
            
            Rectangle {
                anchors.centerIn: parent
                radius: Math.min(width, height)
                width: Math.min(image.width, image.height)
                height: width
            }
        }
    }
}
