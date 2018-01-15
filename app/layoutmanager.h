/*
*  Copyright 2017  Smith AR <audoban@openmailbox.org>
*                  Michail Vourlakos <mvourlakos@gmail.com>
*
*  This file is part of Latte-Dock
*
*  Latte-Dock is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License as
*  published by the Free Software Foundation; either version 2 of
*  the License, or (at your option) any later version.
*
*  Latte-Dock is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LAYOUTMANAGER_H
#define LAYOUTMANAGER_H

#include "importer.h"
#include "layout.h"
#include "layoutconfigdialog.h"
#include "launcherssignals.h"

#include <QAction>
#include <QObject>

#include <KLocalizedString>

class DockCorona;
class Importer;
class Layout;
class LayoutConfigDialog;
class LaunchersSignals;

namespace Latte {

//! This class is responsible to manipulate all layouts.
//! add,remove,rename, update configurations etc.
class LayoutManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString currentLayoutName READ currentLayoutName NOTIFY currentLayoutNameChanged)

    Q_PROPERTY(QStringList layouts READ layouts NOTIFY layoutsChanged)
    Q_PROPERTY(QStringList menuLayouts READ menuLayouts NOTIFY menuLayoutsChanged)

    Q_PROPERTY(QAction *addWidgetsAction READ addWidgetsAction NOTIFY addWidgetsActionChanged)

    Q_PROPERTY(LaunchersSignals *launchersSignals READ launchersSignals NOTIFY launchersSignalsChanged)

public:
    LayoutManager(QObject *parent = nullptr);
    ~LayoutManager() override;

    DockCorona *corona();
    Importer *importer();

    void load();
    void unload();
    void addDock(Plasma::Containment *containment, bool forceLoading = false, int expDockScreen = -1);
    void recreateDock(Plasma::Containment *containment);
    void syncDockViewsToScreens();

    bool layoutExists(QString layoutName) const;

    QString shouldSwitchToLayout(QString activityId);

    QString currentLayoutName() const;
    QString defaultLayoutName() const;

    QStringList layouts() const;
    QStringList menuLayouts() const;
    QStringList presetsPaths() const;

    Dock::LayoutsMemoryUsage memoryUsage() const;
    void setMemoryUsage(Dock::LayoutsMemoryUsage memoryUsage);

    QAction *addWidgetsAction();

    QHash<const Plasma::Containment *, DockView *> *currentDockViews() const;
    QHash<const Plasma::Containment *, DockView *> *layoutDockViews(const QString &layoutName) const;
    //! returns an active layout with that #id (name), it returns null if such
    //! layout cant be found
    Layout *activeLayout(QString id) const;
    int activeLayoutPos(QString id) const;

    LaunchersSignals *launchersSignals();

    QStringList activities();
    QStringList runningActivities();
    QStringList orphanedActivities(); //! These are activities that havent been assigned to specific layout

    void importDefaultLayout(bool newInstanceIfPresent = false);
    void importPresets(bool includeDefault = false);

public slots:
    //! switch to specified layout
    Q_INVOKABLE bool switchToLayout(QString layoutName);

    //! creates a new layout with layoutName based on the preset
    Q_INVOKABLE QString newLayout(QString layoutName, QString preset = i18n("Default"));

    Q_INVOKABLE void showLayoutConfigDialog();

signals:
    void activeLayoutsChanged();
    void addWidgetsActionChanged();
    void currentLayoutChanged();
    void currentLayoutNameChanged();
    void currentLayoutIsChanging();
    void launchersSignalsChanged();
    void layoutsChanged();
    void memoryUsageChanged();
    void menuLayoutsChanged();

private slots:
    void currentActivityChanged(const QString &id);
    void showInfoWindowChanged();
    void showWidgetsExplorer();
    void syncMultipleLayoutsToActivities(QString layoutForOrphans = QString());

private:
    void clearUnloadedContainmentsFromLinkedFile(QStringList containmentsIds);
    void confirmDynamicSwitch();
    //! it is used just in order to provide translations for the presets
    void ghostForTranslatedPresets();
    //! This function figures in the beginning if a dock with tasks
    //! in it will be loaded taking into account also the screens are present.
    //! returns true if it will be loaded, false otherwise
    //! firstContainmentWithTasks = the first containment containing a taskmanager plasmoid
    bool heuresticForLoadingDockWithTasks(int *firstContainmentWithTasks);
    void importLatteLayout(QString layoutPath);
    void importPreset(int presetNo, bool newInstanceIfPresent = false);
    void loadLatteLayout(QString layoutPath);
    void loadLayouts();
    void setMenuLayouts(QStringList layouts);
    void showInfoWindow(QString info, int duration, QStringList activities = {"0"});
    void updateCurrentLayoutNameInMultiEnvironment();

    bool layoutIsAssigned(QString layoutName);

    QString layoutPath(QString layoutName);

    QStringList validActivities(QStringList currentList);

private:
    QString m_currentLayoutNameInMultiEnvironment;
    QString m_shouldSwitchToLayout;

    QStringList m_layouts;
    QStringList m_menuLayouts;
    QStringList m_presetsPaths;

    QAction *m_addWidgetsAction{nullptr};

    QPointer<LayoutConfigDialog> m_layoutConfigDialog;

    QHash<const QString, QString> m_assignedLayouts;

    QTimer m_dynamicSwitchTimer;

    DockCorona *m_corona{nullptr};
    Importer *m_importer{nullptr};
    LaunchersSignals *m_launchersSignals{nullptr};
    Layout *m_currentLayout{nullptr};
    QList<Layout *> m_activeLayouts;

    friend class LayoutConfigDialog;
};

}

#endif // LAYOUTMANAGER_H
