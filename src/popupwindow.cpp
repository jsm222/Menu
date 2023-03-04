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

#include "popupwindow.h"
#include <QVBoxLayout>

PopupWindow::PopupWindow(QWidget *parent) : QMenu(parent), m_mainLayout(new QVBoxLayout)
{
    m_mainLayout->setContentsMargins(10, 10, 10, 10);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);
    setVisible(false);
}

void PopupWindow::setWidget(QWidget *w)
{
    if (!w)
        return;

    w->setParent(this);
    m_mainLayout->addWidget(w);
    adjustSize();
}