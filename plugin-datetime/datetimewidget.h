#ifndef DATETIMEWIDGET_H
#define DATETIMEWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QTimer>

class DateTimeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DateTimeWidget(QWidget *parent = nullptr);

private:
    void updateCurrentTimeString();
    void change();

private:
    QMenuBar *m_menubar;
    QMenu *m_menu;
    QTimer *m_refreshTimer;
    QAction *dateAction;
};

#endif // DATETIMEWIDGET_H
