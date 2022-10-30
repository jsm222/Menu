#include "windowswidget.h"
#include <QHBoxLayout>
#include <QLocale>
#include <QProcess>
#include <QDebug>
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

    // Add menu items
    // m_menu->addAction(dateAction);
    // connect(dateAction, &QAction::triggered, this, &WindowsWidget::change);

    updateWindows();

    connect(KWindowSystem::self(), &KWindowSystem::activeWindowChanged, this, &WindowsWidget::updateWindows);
}

void WindowsWidget::updateWindows()
{
    m_menu->setTitle(applicationNiceNameForWId(KWindowSystem::activeWindow()));
}
