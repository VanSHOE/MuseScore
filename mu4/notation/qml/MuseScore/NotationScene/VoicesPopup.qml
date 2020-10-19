import QtQuick 2.9
import QtQuick.Controls 2.2

import MuseScore.UiComponents 1.0

StyledPopup {
    id: root

    property var voicesVisibility: [ true, true, true, true ]

    height: contentColumn.implicitHeight + bottomPadding + topPadding
    width: 200

    Column {
        id: contentColumn

        width: parent.width

        spacing: 18

        StyledTextLabel {
            text: qsTrc("notation", "Voices visible on this score")
        }

        ListView {
            spacing: 8

            height: contentHeight
            width: parent.width

            model: root.voicesVisibility

            delegate: CheckBox {
                checked: modelData
                text: qsTrc("notation", "Voice ") + (model.index + 1)

                onClicked: {
                    checked = !checked
                    voicesVisibility[model.index] = checked
                }
            }
        }
    }
}
