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

#include "shortcutsmodel.h"

#include "ui/view/iconcodes.h"
#include "translation.h"
#include "log.h"

using namespace mu::shortcuts;
using namespace mu::ui;

static QString shortcutsFileFilter()
{
    return mu::qtrc("shortcuts", "MuseScore Shortcuts File") + " (*.xml)";
}

ShortcutsModel::ShortcutsModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

QVariant ShortcutsModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const Shortcut& shortcut = m_shortcuts.at(index.row());

    switch (role) {
    case RoleTitle: return actionText(shortcut.action);
    case RoleIcon: return static_cast<int>(this->action(shortcut.action).iconCode);
    case RoleSequence: return sequencesToNativeText(shortcut.sequences);
    case RoleSearchKey: {
        UiAction action = this->action(shortcut.action);
        return QString::fromStdString(action.code) + action.title + action.description + sequencesToNativeText(shortcut.sequences);
    }
    }

    return QVariant();
}

const UiAction& ShortcutsModel::action(const std::string& actionCode) const
{
    return uiactionsRegister()->action(actionCode);
}

const QString& ShortcutsModel::actionTitle(const std::string& actionCode) const
{
    const UiAction& action = this->action(actionCode);
    return action.title;
}

const QString& ShortcutsModel::actionDescription(const std::string& actionCode) const
{
    const UiAction& action = this->action(actionCode);
    return action.description;
}

const QString& ShortcutsModel::actionText(const std::string& actionCode) const
{
    return actionDescription(actionCode) == "" ? actionTitle(actionCode) : actionDescription(actionCode);
}

int ShortcutsModel::rowCount(const QModelIndex&) const
{
    return m_shortcuts.size();
}

QHash<int, QByteArray> ShortcutsModel::roleNames() const
{
    static const QHash<int, QByteArray> roles {
        { RoleTitle, "title" },
        { RoleIcon, "icon" },
        { RoleSequence, "sequence" },
        { RoleSearchKey, "searchKey" }
    };

    return roles;
}

void ShortcutsModel::load()
{
    beginResetModel();
    m_shortcuts.clear();

    for (const UiAction& action : uiactionsRegister()->getActions()) {
        if (action.title.isEmpty()) {
            continue;
        }

        Shortcut shortcut = shortcutsRegister()->shortcut(action.code);
        if (shortcut.action != action.code) {
            shortcut.action = action.code;
            shortcut.context = action.scCtx;
        }

        m_shortcuts << shortcut;
    }

    shortcutsRegister()->shortcutsChanged().onNotify(this, [this]() {
        load();
    });

    std::sort(m_shortcuts.begin(), m_shortcuts.end(), [this](const Shortcut& s1, const Shortcut& s2) {
        return actionText(s1.action) < actionText(s2.action);
    });

    endResetModel();
}

bool ShortcutsModel::apply()
{
    ShortcutList shortcuts;

    for (const Shortcut& shortcut : qAsConst(m_shortcuts)) {
        if (!shortcut.sequences.empty()) {
            shortcuts.push_back(shortcut);
        }
    }

    Ret ret = shortcutsRegister()->setShortcuts(shortcuts);

    if (!ret) {
        LOGE() << ret.toString();
    }

    return ret;
}

void ShortcutsModel::reset()
{
    shortcutsRegister()->resetShortcuts();
}

QItemSelection ShortcutsModel::selection() const
{
    return m_selection;
}

QVariant ShortcutsModel::currentShortcut() const
{
    QModelIndex index = currentShortcutIndex();
    if (!index.isValid()) {
        return QVariant();
    }

    const Shortcut& sc = m_shortcuts.at(index.row());

    QVariantMap obj;
    obj["title"] = actionTitle(sc.action);
    obj["sequence"] = QString::fromStdString(sc.sequencesAsString());
    obj["context"] = QString::fromStdString(sc.context);
    return obj;
}

QModelIndex ShortcutsModel::currentShortcutIndex() const
{
    if (m_selection.size() == 1) {
        return m_selection.indexes().first();
    }

    return QModelIndex();
}

void ShortcutsModel::setSelection(const QItemSelection& selection)
{
    if (m_selection == selection) {
        return;
    }

    m_selection = selection;
    emit selectionChanged();
}

void ShortcutsModel::importShortcutsFromFile()
{
    io::path_t path = interactive()->selectOpeningFile(
        qtrc("shortcuts", "Import shortcuts"),
        globalConfiguration()->homePath(),
        shortcutsFileFilter());

    if (!path.empty()) {
        shortcutsRegister()->importFromFile(path);
    }
}

void ShortcutsModel::exportShortcutsToFile()
{
    io::path_t path = interactive()->selectSavingFile(
        qtrc("shortcuts", "Export shortcuts"),
        globalConfiguration()->homePath(),
        shortcutsFileFilter());

    if (path.empty()) {
        return;
    }

    Ret ret = shortcutsRegister()->exportToFile(path);
    if (!ret) {
        LOGE() << ret.toString();
    }
}

void ShortcutsModel::applySequenceToCurrentShortcut(const QString& newSequence)
{
    QModelIndex index = currentShortcutIndex();
    if (!index.isValid()) {
        return;
    }

    int row = index.row();
    m_shortcuts[row].sequences = Shortcut::sequencesFromString(newSequence.toStdString());

    notifyAboutShortcutChanged(index);
}

void ShortcutsModel::clearSelectedShortcuts()
{
    for (const QModelIndex& index : m_selection.indexes()) {
        Shortcut& shortcut = m_shortcuts[index.row()];
        shortcut.sequences.clear();
        shortcut.standardKey = QKeySequence::StandardKey::UnknownKey;

        notifyAboutShortcutChanged(index);
    }
}

void ShortcutsModel::notifyAboutShortcutChanged(const QModelIndex& index)
{
    emit dataChanged(index, index);
}

void ShortcutsModel::resetToDefaultSelectedShortcuts()
{
    for (const QModelIndex& index : m_selection.indexes()) {
        Shortcut& shortcut = m_shortcuts[index.row()];

        Shortcut defaultShortcut = shortcutsRegister()->defaultShortcut(shortcut.action);
        if (defaultShortcut.isValid()) {
            shortcut = defaultShortcut;
        } else {
            shortcut.sequences = {};
        }

        notifyAboutShortcutChanged(index);
    }
}

QVariantList ShortcutsModel::shortcuts() const
{
    QVariantList result;

    for (const Shortcut& shortcut : qAsConst(m_shortcuts)) {
        QVariantMap obj;
        obj["title"] = actionTitle(shortcut.action);
        obj["sequence"] = QString::fromStdString(shortcut.sequencesAsString());
        obj["context"] = QString::fromStdString(shortcut.context);

        result << obj;
    }

    return result;
}
