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
#ifndef MU_INSTRUMENTSSCENE_INSTRUMENTLISTMODEL_H
#define MU_INSTRUMENTSSCENE_INSTRUMENTLISTMODEL_H

#include <QAbstractListModel>

#include "async/asyncable.h"

#include "modularity/ioc.h"
#include "notation/iinstrumentsrepository.h"

#include "uicomponents/view/itemmultiselectionmodel.h"

namespace mu::instrumentsscene {
class InstrumentListModel : public QAbstractListModel, public async::Asyncable
{
    Q_OBJECT

    INJECT(instruments, notation::IInstrumentsRepository, repository)

    Q_PROPERTY(QStringList genres READ genres NOTIFY genresChanged)
    Q_PROPERTY(QStringList groups READ groups NOTIFY groupsChanged)

    Q_PROPERTY(int currentGenreIndex READ currentGenreIndex WRITE setCurrentGenreIndex NOTIFY currentGenreChanged)
    Q_PROPERTY(int currentGroupIndex READ currentGroupIndex WRITE setCurrentGroupIndex NOTIFY currentGroupChanged)

    Q_PROPERTY(bool hasSelection READ hasSelection NOTIFY selectionChanged)

public:
    InstrumentListModel(QObject* parent = nullptr);

    QVariant data(const QModelIndex& index, int role) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

    QStringList genres() const;
    QStringList groups() const;

    int currentGenreIndex() const;
    int currentGroupIndex() const;

    bool hasSelection() const;

    Q_INVOKABLE void load(bool canSelectMultipleInstruments, const QString& currentInstrumentId);

    Q_INVOKABLE void setSearchText(const QString& text);

    Q_INVOKABLE void selectInstrument(int instrumentIndex);
    Q_INVOKABLE QVariantList selectedInstruments() const;

public slots:
    void setCurrentGenreIndex(int index);
    void setCurrentGroupIndex(int index);

signals:
    void genresChanged();
    void groupsChanged();

    void currentGenreChanged();
    void currentGroupChanged();

    void selectionChanged();

    void focusRequested(int groupIndex, int instrumentIndex);

private:
    enum Roles {
        RoleName = Qt::UserRole + 1,
        RoleIsSelected,
        RoleTraits,
        RoleCurrentTraitIndex
    };

    struct CombinedInstrument
    {
        QString name;
        notation::InstrumentTemplateList templates;
        int currentTemplateIndex = 0;
    };

    using Instruments = QList<CombinedInstrument>;

    void init(const QString& genreId, const QString& groupId);

    QString resolveInstrumentGroupId(const QString& instrumentId) const;
    void focusOnInstrument(const QString& instrumentId);

    void loadInstruments();
    void sortInstruments(Instruments& instruments) const;

    notation::InstrumentGenreList availableGenres() const;
    notation::InstrumentGroupList availableGroups() const;

    bool isSearching() const;
    void updateGenreStateBySearch();
    bool isInstrumentAccepted(const notation::InstrumentTemplate& instrument, bool compareWithCurrentGroup = true) const;
    bool isInstrumentIndexValid(int index) const;

    void setCurrentGenre(const QString& genreId);
    void setCurrentGroup(const QString& groupId);
    void doSetCurrentGroup(const QString& groupId);

    QString m_currentGenreId;
    QString m_savedGenreId;
    QString m_currentGroupId;
    QString m_searchText;

    Instruments m_instruments;
    uicomponents::ItemMultiSelectionModel* m_selection = nullptr;

    bool m_instrumentsLoadingAllowed = false;
};
}

#endif // MU_INSTRUMENTSSCENE_INSTRUMENTLISTMODEL_H
