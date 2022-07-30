/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import MuseScore.Ui 1.0
import MuseScore.UiComponents 1.0
import MuseScore.Shortcuts 1.0

Dialog {
    id: root

    signal applySequenceRequested(string newSequence, int conflictShortcutIndex, var shortcut2change)

    function clearConflicts() {
        model.clearConflicts()
    }

    function startEdit(shortcut, allShortcuts) {
        model.load(shortcut, allShortcuts)
        console.log(model.originAction + " is the action in editshortdialog.qml")
        open()
        content.forceActiveFocus()
    }

    height: 240
    width: 538

    title: qsTrc("shortcuts", "Enter shortcut sequence")

    standardButtons: Dialog.NoButton

    EditShortcutModel {
        id: model

        onApplyNewSequenceRequested: function(newSequence, conflictShortcutIndex) {
            root.applySequenceRequested(newSequence, conflictShortcutIndex, model.originAction)
        }
    }

    Rectangle {
        id: content

        anchors.fill: parent

        color: ui.theme.backgroundPrimaryColor

        focus: true

        Column {
            anchors.fill: parent
            anchors.margins: 8

            spacing: 20

            StyledTextLabel {
                anchors.horizontalCenter: parent.horizontalCenter

                width: parent.width
                text: qsTrc("shortcuts", "Define keyboard shortcut")
                horizontalAlignment: Text.AlignLeft
                font:ui.theme.headerBoldFont
            }

            Column {
                width: parent.width

                spacing: 12

                StyledTextLabel {
                    width: parent.width
                    horizontalAlignment: Qt.AlignLeft

                    text: model.conflictWarning
                }

                RowLayout {
                    width: parent.width
                    height: childrenRect.height

                    spacing: 12

                    StyledTextLabel {
                        Layout.alignment: Qt.AlignVCenter

                        text: qsTrc("shortcuts", "Old shortcut:")
                    }

                    TextInputField {
                        Layout.fillWidth: true

                        enabled: false
                        currentText: model.originSequence
                    }
                }

                RowLayout {
                    width: parent.width
                    height: childrenRect.height

                    spacing: 12

                    StyledTextLabel {
                        Layout.alignment: Qt.AlignVCenter

                        text: qsTrc("shortcuts", "New shortcut:")
                    }

                    TextInputField {
                        id: newSequenceField

                        Layout.fillWidth: true

                        hint: qsTrc("shortcuts", "Type to set shortcut")
                        readOnly: true
                        currentText: model.newSequence

                        onActiveFocusChanged: {
                            if (activeFocus) {
                                content.forceActiveFocus()
                            }
                        }
                    }
                }
            }

            RowLayout {
                width: parent.width
                height: childrenRect.height

                readonly property int buttonWidth: 100

                Item { Layout.fillWidth: true }

                FlatButton {
                    width: parent.buttonWidth

                    text: qsTrc("global", "Cancel")

                    onClicked: {
                        root.reject()
                    }
                }

                FlatButton {
                    width: parent.buttonWidth

                    text: qsTrc("global", "Save")

                    onClicked: {
                        model.applyNewSequence()
                        root.accept()
                    }
                }
            }
        }

        Keys.onShortcutOverride: function(event) {
            event.accepted = true
        }

        Keys.onPressed: function(event) {
            model.inputKey(event.key, event.modifiers)
        }
    }
}

