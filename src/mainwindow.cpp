/*
 * Copyright (C) 2020 PandaOS Team.
 *
 * Author:     rekols <revenmartin@gmail.com>
 * Portions:   probono <probono@puredarwin.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "applicationinfo.h"
#include "mainwindow.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QScreen>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QApplication>
#include <QLibraryInfo>
#include <KF5/KWindowSystem/KWindowSystem>

#include <QX11Info>
#include <QScreen>
#include <QMenu>
#include <QLabel>
#include <xcb/xcb.h>
#include <X11/Xlib.h>


MainWindow::MainWindow(QWidget *parent)
    : QFrame(parent),
      //m_fakeWidget(new QWidget(nullptr)),
      applicationStartingLabel(new QLabel("", this)),
      m_MainWidget(new MainWidget(parent))
{
    this->setObjectName("menuBar");
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    qDebug() << "translated: tr(\"Log Out\"):" << tr("Log Out");
    qDebug() << "translated: tr(\"About This Computer\"):" << tr("About This Computer");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addSpacing(7); // Left screen edge; if space is too small, blue box overlaps rounded corner
    layout->addWidget(m_MainWidget);

    applicationStartingLabel->hide();
    // TODO: Instead of having applicationStartingLabel here, we might want to make it a part of m_MainWidget
    // to allow for it to be animated from the center to the side and morph into a menu with an animation...
    applicationStartingLabel->setFixedHeight(22); // FIXME: Dynamically get the height of a QMenuItem and use that
    applicationStartingLabel->setStyleSheet("align: center; font-weight: bold;");
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(applicationStartingLabel, 0, Qt::AlignCenter);

    layout->addSpacing(7); // Right screen edge
    layout->setMargin(0);
    layout->setSpacing(10); // ?
    setLayout(layout);

    //m_fakeWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus | Qt::SplashScreen);
  //  m_fakeWidget->setAttribute(Qt::WA_TranslucentBackground);

    // Prevent menubar from becoming faded/translucent if we use a compositing manager
    // that fades/makes translucent inactive windows
   // m_MainWidget->setWindowFlags(Qt::WindowDoesNotAcceptFocus);

    setAttribute(Qt::WA_NoSystemBackground, false);
    // setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus);

    KWindowSystem::setOnDesktop(effectiveWinId(), NET::OnAllDesktops);

    // "Indicates a toplevel menu (AKA macmenu).
    // This is a KDE extension to the _NET_WM_WINDOW_TYPE mechanism."
    // Source:
    // https://api.kde.org/frameworks/kwindowsystem/html/classNET.html#a4b3115c0f40e7bc8e38119cc44dd60e0
    // Can be inspected with: xwininfo -wm, it contains "Window type: Kde Net Wm Window Type Topmenu"
    // This should allow e.g., picom to set different settings regarding shadows and transparency
    KWindowSystem::setType(winId(), NET::TopMenu);

    //TODO:
    //Call this when the user sets the primary display via xrandr
    initSize();

    //subscribe to changes on our display like if we change the screen resolution, orientation etc..
    connect(qApp->primaryScreen(), &QScreen::geometryChanged, this, &MainWindow::initSize);
    connect(qApp->primaryScreen(), &QScreen::orientationChanged, this, &MainWindow::initSize);
    connect(qApp->primaryScreen(), &QScreen::virtualGeometryChanged, this, &MainWindow::initSize);
    connect(qApp->primaryScreen(), &QScreen::availableGeometryChanged, this, &MainWindow::initSize);
    connect(qApp->primaryScreen(), &QScreen::logicalDotsPerInchChanged, this, &MainWindow::initSize);
    connect(qApp->primaryScreen(), &QScreen::physicalDotsPerInchChanged, this, &MainWindow::initSize);
    connect(qApp->primaryScreen(), &QScreen::physicalSizeChanged, this, &MainWindow::initSize);
    connect(qApp->primaryScreen(), &QScreen::primaryOrientationChanged, this, &MainWindow::initSize);
   
    // Appear with an animation
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(1500);
    animation->setStartValue(QPoint(qApp->primaryScreen()->geometry().x(), -2 * qApp->primaryScreen()->geometry().height()));
    animation->setEndValue(QPoint(qApp->primaryScreen()->geometry().x(),qApp->primaryScreen()->geometry().y()));
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
    //this->activateWindow(); // probono: Ensure that we have the focus when menu is launched so that one can enter text in the search box
    //m_MainWidget->raise(); // probono: Trying to give typing focus to the search box that is in there. Needed? Does not seem tp hurt

    connect(KWindowSystem::self(), &KWindowSystem::activeWindowChanged, this, &MainWindow::hideApplicationName);
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *e)
{

    // probono: Draw black rounded corners on the top edges
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    int round_pixels = 5; // like /usr/local/etc/xdg/picom.conf // probono: Make this relative to the height of the MainWindow?
    // QPainterPath::subtracted() takes InnerPath and subtracts it from OuterPath to produce the final shape
    QPainterPath OuterPath;
    OuterPath.addRect(0, 0, qApp->primaryScreen()->geometry().width(), 2*round_pixels);
    QPainterPath InnerPath;
    InnerPath.addRoundedRect(QRect(0, 0, qApp->primaryScreen()->geometry().width(), 4*round_pixels), round_pixels, round_pixels);
    QPainterPath FillPath;
    FillPath = OuterPath.subtracted(InnerPath);
    p.fillPath(FillPath, Qt::black);

    // Draw the other widgets
    QWidget::paintEvent(e);
}

void MainWindow::initSize()
{
    QRect primaryRect = qApp->primaryScreen()->geometry();

    setFixedWidth(primaryRect.width());

    // probono: Construct a populated(!) QMenuBar so that we can determine
    // its height and use the same height for the MainWindow. Is there a better way?
    /*QMenuBar *dummyMenuBar = new QMenuBar;
    dummyMenuBar->setContentsMargins(0, 0, 0, 0);
    dummyMenuBar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    QMenu *dummyMenu = new QMenu;
    dummyMenuBar->addMenu(dummyMenu);
    qDebug() << "probono: dummyMenu->sizeHint().height():" << dummyMenu->sizeHint().height();- */
    //use m_MainWidget instead of dummyMenu
    setFixedHeight(m_MainWidget->sizeHint().height());

    //move this to the active screen and xrandr position
    move(qApp->primaryScreen()->geometry().x(), qApp->primaryScreen()->geometry().y());

    setStrutPartial();
    
    KWindowSystem::setState(winId(), NET::SkipTaskbar); // Do not show in Dock
    KWindowSystem::setState(winId(), NET::StaysOnTop);
    KWindowSystem::setState(winId(), NET::SkipPager);
    KWindowSystem::setState(winId(), NET::SkipSwitcher);
    // How can we set _NET_WM_STATE_ABOVE? KDE krunner has it set

    // https://stackoverflow.com/a/27964691
    // "window should be of type _NET_WM_TYPE_DOCK and you must first map it then move it
    // to position, otherwise the WM may sometimes place it outside of it own strut."
    // _NET_WM_WINDOW_TYPE_DOCK
    KWindowSystem::setType(winId(), NET::Dock);

    // probono: Set background gradient
    // Commenting this out because possibly this interferes with theming via a QSS file via QtPlugin?
    // this->setStyleSheet( "MainWindow { background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #fff, stop: 0.1 #eee, stop: 0.39 #eee, stop: 0.4 #ddd, stop: 1 #eee); }");
}

void MainWindow::setStrutPartial()
{
    //不清真的作法，kwin设置blur后设置程序支撑导致模糊无效
    //TRANSLATED Unclear practice, setting program support after kwin set blur causes blur invalid
    QRect r(geometry());
    r.setHeight(1);
    r.setWidth(1);


    const QRect windowRect = this->rect();
    NETExtendedStrut strut;

    strut.top_width = height(); // + 1; // 1 pixel between menu bar and maximized window not needed if we have a shadow
    strut.top_start = x();
    strut.top_end = x() + width();

    KWindowSystem::setExtendedStrut(winId(),
                                     strut.left_width,
                                     strut.left_start,
                                     strut.left_end,
                                     strut.right_width,
                                     strut.right_start,
                                     strut.right_end,
                                     strut.top_width,
                                     strut.top_start,
                                     strut.top_end,
                                     strut.bottom_width,
                                     strut.bottom_start,
                                     strut.bottom_end);
}

QString MainWindow::showApplicationName(const QString &arg)
{
    QString message = QString("showApplicationName(\"%1\") got called").arg(arg);
    qDebug() << "showApplicationName" << arg << "got called";

    // Find out whether we already have a window open from this application;
    // in which case we don't show the application name
    bool alreadyRunningApp = false;

    /*
     * probono: Doing this here is too slow.
     * We need to set these things as properties on the window
     * whenever a new window appears for the first time, so that we
     * can query it here quickly.
     * It is crucial that we show the name of the application being launched
     * very quickly; otherwise the application launch may be over before we
     * even start to show the name.
     *
    for (WId id : KWindowSystem::windows()){
        qDebug() << "applicationNiceNameForWId:" << applicationNiceNameForWId(id);
        qDebug() << "bundlePathForWId:" << bundlePathForWId(id);
        qDebug() << "pathForWId:" << pathForWId(id);
        if(bundlePathForWId(id) == arg || pathForWId(id) == arg){
            alreadyRunningApp = true;
            break;
        }
    }
    */

    // This seeme to be a reasonable compromise in terms of speed,
    // but we are not showing the second of two applications with the same name
    // being launched from two different locations. Maybe this is good enough for now
    ApplicationInfo *ai = new ApplicationInfo();
    for (WId id : KWindowSystem::windows()){
        if(ai->applicationNiceNameForWId(id) == arg){
            alreadyRunningApp = true;
            break;
        }
    }
    ai->~ApplicationInfo();

    if (! alreadyRunningApp) {
        m_MainWidget->hide();
        applicationStartingLabel->setText(arg.split("/").last());
        applicationStartingLabel->show();
        // applicationStartingLabel->setVisible(true);
        QTimer::singleShot(30000, this, SLOT(hideApplicationName()));
        return QString("showApplicationName(\"%1\") got executed").arg(arg); // Return to calling application via D-Bus
    } else {
        return QString("showApplicationName(\"%1\") ignored, an application at this path is already running").arg(arg); // Return to calling application via D-Bus
    }
}

void MainWindow::hideApplicationName()
{
    MainWindow::applicationStartingLabel->hide();
    m_MainWidget->show();
}
