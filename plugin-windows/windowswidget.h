#ifndef WINDOWSWIDGET_H
#define WINDOWSWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QTimer>

#include <KWindowSystem>

class WindowsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WindowsWidget(QWidget *parent = nullptr);
    void hideOthers(WId id);

private:
    void updateWindows();

    void activateWindow(WId id);
    void onWindowChanged(WId window, NET::Properties prop, NET::Properties2 prop2);

private:
    QMenuBar *m_menubar;
    QMenu *m_menu;
    QTimer *m_refreshTimer;
};

#endif // WINDOWSWIDGET_H
