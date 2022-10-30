#ifndef WINDOWSWIDGET_H
#define WINDOWSWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QTimer>

class WindowsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WindowsWidget(QWidget *parent = nullptr);

private:
    void updateWindows();
    void hideOthers(WId id);
    void activateWindow(WId id);

private:
    QMenuBar *m_menubar;
    QMenu *m_menu;
    QTimer *m_refreshTimer;
};

#endif // WINDOWSWIDGET_H
