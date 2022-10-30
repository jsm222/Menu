#include "windowswidget.h"
#include <QHBoxLayout>
#include <QLocale>
#include <QProcess>
#include <QDebug>
#include <QApplication>
#include <KF5/KWindowSystem/KWindowSystem>

#include "../src/applicationwindow.h"


WindowsWidget::WindowsWidget(QWidget *parent)
    : QWidget(parent),
      m_menubar(new QMenuBar),
      m_menu(new QMenu),
      m_refreshTimer(new QTimer(this))
{

    m_menu->setTitle(tr("Windows"));

    QFont f = m_menu->menuAction()->font();
    f.setBold(true);
    m_menu->menuAction()->setFont(f);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(m_menubar);
    m_menubar->addMenu(m_menu);
    setLayout(layout);

    updateWindows();

    connect(KWindowSystem::self(), &KWindowSystem::activeWindowChanged, this, &WindowsWidget::updateWindows);
}

void WindowsWidget::updateWindows()

// FIXME: Instead of doing this whole thing each time the frontmost window changes
// we could probably keep some internal state and change just what needs to be changed;
// but writing the code for this is more complex and error-prone; so let's see whether
// we can get away with this

{
    m_menu->setTitle(applicationNiceNameForWId(KWindowSystem::activeWindow()));

    m_menu->clear();

    m_menu->setToolTipsVisible(true);

    QList<unsigned int> distinctPids = {};
    QList<WId> distinctApps = {};

    // Find out one process ID per application that has at least one window
    // TODO: No Dock windows
    for (WId id : KWindowSystem::windows()){
        KWindowInfo info(id, NET::WMPid | NET::WMWindowType | NET::WMState, NET::WM2TransientFor | NET::WM2WindowClass | NET::WM2WindowRole);
        // Don't add the Dock and the Menu to this menu
        // but do add Desktop (even though it could be filtered away using NET::Desktop)
        NET::WindowTypes mask = NET::AllTypesMask;
        if (info.windowType(mask) & (NET::Dock))
            continue;
        if (info.windowType(mask) & (NET::Menu))
            continue;
        if (info.state() & NET::SkipTaskbar)
            continue;

        if (! distinctPids.contains(info.pid())) {
            distinctPids.append(info.pid());
            distinctApps.append(id);
        }
    }

    // Hide frontmost app
    WId id = KWindowSystem::activeWindow();
    QAction *hideAction = m_menu->addAction(tr("Hide %1").arg(applicationNiceNameForWId(id)));
    hideAction->setShortcut(QKeySequence("Ctrl+H"));
    connect(hideAction, &QAction::triggered, this, [hideAction, id, this]() {
        KWindowSystem::minimizeWindow(id);
    });

    // Hide others
    QAction *hideOthersAction = m_menu->addAction(tr("Hide Others"));
    // hideOthersAction->setShortcut(QKeySequence("Shift+Alt+H"));
    connect(hideOthersAction, &QAction::triggered, this, [hideOthersAction, id, this]() {
        hideOthers(id);
    });

    // Show all
    QAction *showAllAction = m_menu->addAction(tr("Show All"));
    // showAllAction->setShortcut(QKeySequence("Shift+Ctrl+H"));
    connect(showAllAction, &QAction::triggered, this, [showAllAction, id, this]() {
        for (WId cand_id : KWindowSystem::windows()){
            KWindowSystem::unminimizeWindow(cand_id);
        }
    });

    m_menu->addSeparator();

    // Add one menu item for each appliction

    for (WId id : distinctApps){

        QString niceName = applicationNiceNameForWId(id);

        // Do not show this Menu application itself in the list of windows
        KWindowInfo info(id, NET::WMPid);
        if (qApp->applicationPid() == info.pid())
            continue;

        // Find out how many menus there are for this PID
        int windowsForThisPID = 0;
        for (WId cand_id : KWindowSystem::windows()){
            KWindowInfo cand_info(cand_id, NET::WMPid);
            if(cand_info.pid() == info.pid())
                windowsForThisPID++;
        }

        if(windowsForThisPID <2) {
            QAction *appAction = m_menu->addAction(niceName);
            appAction->setToolTip(QString("Window ID: %1\n"
                                          "Bundle: %2\n"
                                          "Launchee: %3").arg(id).arg(bundlePathForWId(id)).arg(pathForWId(id)));
            appAction->setCheckable(true);
            // appAction->setIcon(QIcon(KWindowSystem::icon(id))); // Why does this not work? TODO: Get icon from bundle?
            if(id == KWindowSystem::activeWindow()) {
                    appAction->setChecked(true);
                    appAction->setEnabled(false);
            } else {
                connect(appAction, &QAction::triggered, this, [appAction, id, this]() {
                    WindowsWidget::activateWindow(id);
                });
            }
        } else {

            // If there are multiple windows for the same PID, then add multiple submenus
            // So don't add an action here, but a submenu which contains all windows that beloong to that PID
            QMenu *subMenu = m_menu->addMenu(niceName);
            subMenu->setToolTipsVisible(true);
            for (WId cand_id : KWindowSystem::windows()){
                KWindowInfo cand_info(cand_id, NET::WMPid | NET::WMName);
                if(cand_info.pid() == info.pid()) {
                    QAction *appAction = subMenu->addAction(cand_info.name());
                    appAction->setToolTip(QString("Window ID: %1\n"
                                                  "Bundle: %2\n"
                                                  "Launchee: %3").arg(cand_id).arg(bundlePathForWId(cand_id)).arg(pathForWId(cand_id)));
                    appAction->setCheckable(true);
                    // appAction->setIcon(QIcon(KWindowSystem::icon(id))); // Why does this not work? TODO: Get icon from bundle?
                    if(cand_id == KWindowSystem::activeWindow()) {
                            appAction->setChecked(true);
                            appAction->setEnabled(false);
                    } else {
                        connect(appAction, &QAction::triggered, this, [appAction, cand_id, this]() {
                            WindowsWidget::activateWindow(cand_id);
                        });
                    }
                }
            }


        }


    }

    m_menu->addSeparator();

    // Show all
    QAction *fullscreenAction = m_menu->addAction(tr("Full Screen"));
    // TODO: Need a way to undo this...
    connect(fullscreenAction, &QAction::triggered, this, [fullscreenAction, id, this]() {
        KWindowSystem::setState(id, KWindowSystem::FullScreen);
    });

}

void WindowsWidget::hideOthers(WId id) {
    // TODO: Should we be hiding not all other windows, but only windows belonging to other applications (PIDs)?
    for (WId cand_id : KWindowSystem::windows()){
        if(cand_id != id)
            KWindowSystem::minimizeWindow(cand_id);
    }
}

void WindowsWidget::activateWindow(WId id) {

    KWindowSystem::activateWindow(id);

    // If Filer has no windows open but is selected, show the desktop = hide all windows
    // _NET_WM_WINDOW_TYPE(ATOM) = _NET_WM_WINDOW_TYPE_DESKTOP
    KWindowInfo info(id, NET::WMPid | NET::WMWindowType);
    NET::WindowTypes mask = NET::AllTypesMask;
    if (info.windowType(mask) & (NET::Desktop)) {
        qDebug() << "probono: Desktop selected, hence hiding all";
        for (WId cand_id : KWindowSystem::windows()) {
            KWindowSystem::minimizeWindow(cand_id);
        }
    }
}



