#include "menufilterproxy.h"
#include <QDebug>

MenuFilterProxy::MenuFilterProxy(QObject *parent) : QSortFilterProxyModel(parent) {

}
bool MenuFilterProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    QAction * action = sourceModel()->data(index0).value<QAction*>();

    if(action) {
    return action->text().contains(filterRegExp());
    }
    return true;
}

