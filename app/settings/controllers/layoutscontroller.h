/*
 * Copyright 2020  Michail Vourlakos <mvourlakos@gmail.com>
 *
 * This file is part of Latte-Dock
 *
 * Latte-Dock is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * Latte-Dock is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef SETTINGSLAYOUTSCONTROLLER_H
#define SETTINGSLAYOUTSCONTROLLER_H

// local
#include "../data/layoutdata.h"
#include "../data/layoutstable.h"
#include "../models/layoutsmodel.h"
#include "../../lattecorona.h"
#include "../../../liblatte2/types.h"

// Qt
#include <QHash>
#include <QTableView>

namespace Latte {
class Corona;
class CentralLayout;
}

namespace Latte {
namespace Settings {
namespace Controller {

class Layouts : public QObject
{
    Q_OBJECT

public:
    explicit Layouts(QDialog *parent, Latte::Corona *corona, QTableView *view);
    ~Layouts();

    Model::Layouts *model() const;
    QTableView *view() const;

    bool dataAreChanged() const;

    bool inMultipleMode() const;
    void setInMultipleMode(bool inMultiple);

    void setOriginalInMultipleMode(bool inMultiple);

    bool selectedLayoutIsCurrentActive() const;
    const Data::Layout &selectedLayout() const;

    //! actions
    void reset();
    void save();
    void loadLayouts();
    void removeSelected();
    void toggleLockedForSelected();
    void toggleSharedForSelected();

    QString layoutNameForFreeActivities() const;
    void setLayoutNameForFreeActivities(const QString &name, bool updateOriginalData = false);

    void addLayoutForFile(QString file, QString layoutName = QString(), bool newTempDirectory = true, bool showNotification = true);
    void copySelectedLayout();
    //! import layouts from Latte versions <= v0.7.x
    void importLayoutsFromV1ConfigFile(QString file);

signals:
    void dataChanged();

private slots:
    void saveColumnWidths();

private:
    void initView();
    void syncActiveShares();

    int rowForId(QString id) const;
    int rowForName(QString layoutName) const;

    QString uniqueTempDirectory();
    QString uniqueLayoutName(QString name);

private:
    QDialog *m_parent{nullptr};
    Latte::Corona *m_corona{nullptr};
    QTableView *m_view{nullptr};

    //! original data
    bool o_originalInMultipleMode{false};
    Settings::Data::LayoutsTable o_layoutsOriginalData;

    //! current data
    Model::Layouts *m_model{nullptr};
    QHash<const QString, Latte::CentralLayout *> m_layouts;

    //! temp data
    QStringList m_tempDirectories;
};

}
}
}

#endif