#ifndef __MENUFILTER_PROXY_H
#define __MENUFILTER_PROXY_H
#include <QObject>
#include <QSortFilterProxyModel>
#include <QAction>
class MenuFilterProxy: public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit MenuFilterProxy(QObject *parent);
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
};
#endif
