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

#include "extensionwidget.h"
#include <QEvent>
#include <QHBoxLayout>
#include <QApplication>
#include <QMouseEvent>
#include <QScreen>
#include <QDebug>

ExtensionWidget::ExtensionWidget(StatusBarExtension *extension, QWidget *parent)
    : QWidget(parent),
      m_extension(extension),
      m_popupTextDelayTimer(new QTimer(this)),
      m_popupText(new BlurWindow),
      m_popupWindow(new PopupWindow(this)),
      m_contentWidget(extension->itemWidget())
{
    m_popupTextDelayTimer->setInterval(300);
    m_popupTextDelayTimer->setSingleShot(true);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    m_contentWidget->setParent(this);
    m_contentWidget->setVisible(true);
    m_contentWidget->installEventFilter(this);
    layout->addWidget(m_contentWidget);

    setAttribute(Qt::WA_TranslucentBackground);

    if (extension->popupWindow())
        m_popupWindow->setWidget(extension->popupWindow());

    connect(m_popupTextDelayTimer, &QTimer::timeout, this, &ExtensionWidget::showPopupText);
}

QSize ExtensionWidget::sizeHint() const
{
    if (m_contentWidget)
        return m_contentWidget->sizeHint();

    return QWidget::sizeHint();
}

void ExtensionWidget::enterEvent(QEvent *e)
{
    m_popupTextDelayTimer->start();

    QWidget::enterEvent(e);
}

void ExtensionWidget::leaveEvent(QEvent *e)
{
    m_popupTextDelayTimer->stop();
    hidePopupText();

    QWidget::leaveEvent(e);
}

void ExtensionWidget::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);

    hidePopupText();
}

void ExtensionWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);

    showPopupWindow();
}

QPoint ExtensionWidget::popupPoint(int contentWidth)
{
    const int offset = 10;
    QPoint p(mapToGlobal(QPoint(pos().x(), height()) - pos()));
    p += QPoint(0, offset);

    // rekols: Screen edge processing.
    QRect screenGeometry = qApp->primaryScreen()->geometry();
    if (p.x() + m_popupText->geometry().x() >= screenGeometry.x()) {
        p.setX(screenGeometry.width() - contentWidth - offset);
    }

    return p;
}

void ExtensionWidget::showPopupText()
{
    if (m_extension->popupText().isEmpty())
        return;

    const int offset = 10;

    m_popupText->setText(m_extension->popupText());
    m_popupText->setVisible(true);

    QPoint p = popupPoint(m_popupText->width());
    m_popupText->move(p);
    m_popupText->update();
}

void ExtensionWidget::hidePopupText()
{
    m_popupText->hide();
}

void ExtensionWidget::showPopupWindow()
{
    if (!m_extension->popupWindow())
        return;

    QPoint p = popupPoint(m_popupWindow->width());
    m_popupWindow->move(p);
    m_popupWindow->exec(p);
}

bool ExtensionWidget::eventFilter(QObject *watched, QEvent *event)
{
    // if (watched == m_contentWidget) {
    //     if (event->type() == QEvent::MouseButtonPress ||
    //         event->type() == QEvent::MouseButtonRelease) {
    //         return true;
    //     }
    // }

    return false;
}