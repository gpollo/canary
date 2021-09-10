import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

/*
 * Source: https://gist.github.com/jonmcclung/bae669101d17b103e94790341301c129
 */
Rectangle {
    function show(text, duration) {
        message.text = text;
        if (typeof duration !== "undefined") {
            time = Math.max(duration, 2 * fadeTime);
        } else {
            time = defaultTime;
        }
        animation.start();
    }

    id: toast

    property bool expirable: true
    property real time: defaultTime
    property real margin: 10

    readonly property real defaultTime: 3000
    readonly property real fadeTime: 300

    width: content.width
    height: 48
    radius: 3

    color: "#333333"

    Row {
        id: content

        Rectangle {
            color: "transparent"
            width: 24
            height: 48
        }

        Text {
            id: message
            height: parent.height
            color: "white"
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 10
        }

        Rectangle {
            color: "transparent"
            width: 48
            height: 48
        }

        Button {
            text: "Close"
            flat: true
            highlighted: true
            hoverEnabled: false
            onPressed: toastManager.model.remove(index)
        }

        Rectangle {
            color: "transparent"
            width: 24
            height: 48
        }
    }

    SequentialAnimation on opacity {
        id: animation
        running: false

        NumberAnimation {
            to: .9
            duration: fadeTime
        }

        PauseAnimation {
            duration: time - 2 * fadeTime
        }

        NumberAnimation {
            to: 0
            duration: fadeTime
        }

        onRunningChanged: {
            if (!running && expirable) {
                toastManager.model.remove(index);
            }
        }
    }
}