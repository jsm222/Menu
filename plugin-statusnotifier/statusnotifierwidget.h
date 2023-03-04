/*
 * Copyright (C) 2020 PandaOS Team.
 *
 * Author:     rekols <revenmartin@gmail.com>
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

#ifndef STATUSNOTIFIERWIDGET_H
#define STATUSNOTIFIERWIDGET_H

#include <QDir>
#include <QHBoxLayout>
#include "statusnotifierbutton.h"
#include "statusnotifierwatcher.h"

class StatusNotifierWidget : public QWidget
{
    Q_OBJECT

public:
    StatusNotifierWidget(QWidget *parent = nullptr);
    ~StatusNotifierWidget();

public slots:
    void itemAdded(QString serviceAndPath);
    void itemRemoved(const QString &serviceAndPath);

private:
    QHBoxLayout *m_layout;
    StatusNotifierWatcher *m_watcher;

    QHash<QString, StatusNotifierButton *> m_services;
};

#endif // STATUSNOTIFIERWIDGET_H