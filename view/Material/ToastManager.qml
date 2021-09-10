import QtQuick 2.15

/*
 * Source: https://gist.github.com/jonmcclung/bae669101d17b103e94790341301c129
 */
ListView {
    function show(text, duration) {
        if (duration === undefined) {
            model.insert(0, {text: text});
        } else {
            model.insert(0, {text: text, duration: duration});
        }
    }

    id: toastManager

    anchors.fill: parent
    anchors.bottomMargin: 10
    spacing: 5
    z: Infinity

    verticalLayoutDirection: ListView.BottomToTop
    interactive: false

    displaced: Transition {
        NumberAnimation {
            properties: "y"
            easing.type: Easing.InOutQuad
        }
    }
    
    delegate: Row {
        Rectangle {
            color: "transparent"
            height: 48
            width: (toastManager.width - toast.width) / 2
        }

        Toast {
            id: toast
            Component.onCompleted: {
                if (typeof duration === "undefined") {
                    show(text);
                } else {
                    show(text, duration);
                }
            }
        }
    }

    model: ListModel {id: model}
}