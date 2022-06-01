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

import MuseScore.NotationScene 1.0
import MuseScore.UiComponents 1.0
import MuseScore.Ui 1.0


import MuseScore.Shortcuts 1.0
import "internal"

Item {
    id: root

    property alias orientation: gridView.orientation

    property alias navigation: keynavSub

    property bool floating: false

    property int maximumWidth: 0
    property int maximumHeight: 0

    width: gridView.isHorizontal ? childrenRect.width : 76
    height: !gridView.isHorizontal ? childrenRect.height : 40

    NavigationPanel {
        id: keynavSub
        name: "NoteInputBar"
        enabled: root.enabled && root.visible
        accessible.name: qsTrc("notation", "Note Input toolbar")
    }
    ShortcutsModel {
        id: shortcutsModel
        selection: shortcutsView.sourceSelection
    }


    EditShortcutDialog {
        id: editShortcutDialog

        onApplySequenceRequested: function(newSequence, shortcutAction) {
            shortcutsModel.applySequenceToShortcut(shortcutAction, newSequence)
            shortcutsModel.apply()
            noteInputModel.load()
        }

        property bool canEditCurrentShortcut: Boolean(shortcutsModel.currentShortcut)

        function startEditShortcut(shortcut2change) {
            editShortcutDialog.startEdit(shortcut2change, shortcutsModel.shortcuts(), true)
        }
    }



    NoteInputBarModel {
        id: noteInputModel
    }

    QtObject {
        id: prv

        function resolveHorizontalGridViewWidth() {
            if (root.floating) {
                return gridView.contentWidth
            }

            var requiredFreeSpace = gridView.cellWidth * 3 + gridView.rowSpacing * 4

            if (root.maximumWidth - gridView.contentWidth < requiredFreeSpace) {
                return gridView.contentWidth - requiredFreeSpace
            }

            return gridView.contentWidth
        }

        function resolveVerticalGridViewHeight() {
            if (root.floating) {
                return gridView.contentHeight
            }

            var requiredFreeSpace = gridView.cellHeight * 3 + gridView.rowSpacing * 4

            if (root.maximumHeight - gridView.contentHeight < requiredFreeSpace) {
                return gridView.contentHeight - requiredFreeSpace
            }

            return gridView.contentHeight
        }
    }

    Component.onCompleted: {
        noteInputModel.load()
        shortcutsModel.load()
    }

    GridViewSectional {
        id: gridView

        sectionRole: "section"

        rowSpacing: 4
        columnSpacing: 4

        cellWidth: 32
        cellHeight: cellWidth

        clip: true

        model: noteInputModel

        sectionDelegate: SeparatorLine {
            orientation: gridView.orientation
            visible: itemIndex !== 0
        }

        itemDelegate: FlatButton {
            id: btn

            property var item: Boolean(itemModel) ? itemModel.itemRole : null
            property var hasMenu: Boolean(item) && item.subitems.length !== 0
//            property var action: Boolean(item) ? item.action : ""

            width: gridView.cellWidth
            height: gridView.cellWidth

            enabled: Boolean(item) && item.enabled

            accentButton: (Boolean(item) && item.checked) || menuLoader.isMenuOpened
            transparent: !accentButton

            icon: Boolean(item) ? item.icon : IconCode.NONE
            iconFont: ui.theme.toolbarIconsFont

            toolTipTitle: Boolean(item) ? item.title/* + "(" + item.action + ")"*/ : ""
            toolTipDescription: Boolean(item) ? item.description : ""
            toolTipShortcut: Boolean(item) ? item.shortcuts : ""

            navigation.panel: keynavSub
            navigation.name: Boolean(item) ? item.id : ""
            navigation.order: Boolean(itemModel) ? itemModel.order : 0
            isClickOnKeyNavTriggered: false
            navigation.onTriggered: {
                if (menuLoader.isMenuOpened || hasMenu) {
                    toggleMenuOpened()
                } else {
                    handleMenuItem()
                }
            }

            mouseArea.pressAndHoldInterval: 200
            mouseArea.acceptedButtons:  Qt.LeftButton | Qt.RightButton


            function toggleMenuOpened() {

                menuLoader.toggleOpened(item.subitems)
            }

            function handleMenuItem() {
                Qt.callLater(noteInputModel.handleMenuItem, item.id)
            }

            onClicked: function(mouse) {
                if (menuLoader.isMenuOpened // If already menu open, close it
                        || (hasMenu // Or if can open menu
                            && (!itemModel.isMenuSecondary // And _should_ open menu
                                || mouse.button === Qt.RightButton))) {
                    console.log("Opening secondary Menu :(")
                    toggleMenuOpened()
                    return
                }
                else if(mouse.button === Qt.RightButton) {
                    showCellMenu()
                }
                else if (mouse.button === Qt.LeftButton) {
                    handleMenuItem()
                }

            }

            onPressAndHold: {
                if (menuLoader.isMenuOpened || !hasMenu) {
                    return
                }

                toggleMenuOpened()
            }
//            onDoubleClicked: {
//                console.log("Adding shortcut for: " + btn.item.action)
//                editShortcutDialog.startEditShortcut(shortcutsModel.getShortcut(btn.item.action))
//            }

            Canvas {
                visible: Boolean(itemModel) && itemModel.isMenuSecondary

                property color fillColor: ui.theme.fontPrimaryColor
                onFillColorChanged: {
                    requestPaint()
                }

                width: 4
                height: 4

                anchors.margins: 2
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                onPaint: {
                    const ctx = getContext("2d");
                    ctx.fillStyle = fillColor;
                    ctx.moveTo(width, 0);
                    ctx.lineTo(width, height);
                    ctx.lineTo(0, height);
                    ctx.closePath();
                    ctx.fill();
                }
            }

            function showCellMenu() {
                contextMenu.toggleOpened(contextMenu.items, mouseArea.mouseX, mouseArea.mouseY)
            }

            StyledMenuLoader {
                id: contextMenu

                property var modelIndex: null
                property bool canEdit: true

                property var items: [
                    { id: "add", title: qsTrc("notation", "Assign Shortcut"), icon: IconCode.CONFIGURE, enabled: true },
                    { id: "remove", title: qsTrc("notation","Clear current shortcut"), icon: IconCode.DELETE_TANK, enabled: true }
                ]

                onHandleMenuItem: {
                    switch(itemId) {
                    case "add":
                        console.log("Adding shortcut for: " + btn.item.action)
                        editShortcutDialog.startEditShortcut(shortcutsModel.getShortcut(btn.item.action))
                        break
                    case "remove":
                        shortcutsModel.clearSequenceOfShortcut(btn.item.action)
                        shortcutsModel.apply()
                        noteInputModel.load()
                        break
                    }
                }
            }

            StyledMenuLoader {
                id: menuLoader

                onHandleMenuItem: function(itemId) {
                    noteInputModel.handleMenuItem(itemId)
                }
            }
        }
    }

    FlatButton {
        id: customizeButton

        anchors.margins: 4

        width: gridView.cellWidth
        height: gridView.cellHeight

        icon: IconCode.SETTINGS_COG
        iconFont: ui.theme.toolbarIconsFont
        toolTipTitle: qsTrc("notation", "Customise toolbar")
        toolTipDescription: qsTrc("notation", "Show/hide toolbar buttons")
        transparent: true
        navigation.panel: keynavSub
        navigation.order: 100
        navigation.accessible.name: qsTrc("notation", "Customise toolbar")

        onClicked: {
            customizePopup.toggleOpened()
        }

        NoteInputBarCustomisePopup {
            id: customizePopup

            anchorItem: !root.floating ? ui.rootItem : null
        }
    }

    states: [
        State {
            when: gridView.isHorizontal

            PropertyChanges {
                target: gridView
                width: prv.resolveHorizontalGridViewWidth()
                height: root.height
                sectionWidth: 1
                sectionHeight: root.height
                rows: 1
                columns: gridView.noLimit
            }

            AnchorChanges {
                target: customizeButton
                anchors.left: gridView.right
                anchors.verticalCenter: root.verticalCenter
            }
        },
        State {
            when: !gridView.isHorizontal

            PropertyChanges {
                target: gridView
                width: root.width
                height: prv.resolveVerticalGridViewHeight()
                sectionWidth: root.width
                sectionHeight: 1
                rows: gridView.noLimit
                columns: 2
            }

            AnchorChanges {
                target: customizeButton
                anchors.top: gridView.bottom
                anchors.right: parent.right
            }
        }
    ]
}
