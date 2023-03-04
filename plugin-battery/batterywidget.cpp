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

#include "batterywidget.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QDebug>
#include <QIcon>

BatteryWidget::BatteryWidget(QWidget *parent)
    : QWidget(parent),
      // m_percentLabel(new QLabel),
      m_iconLabel(new QLabel),
      m_primaryBattery(nullptr)
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    // layout->addWidget(m_percentLabel, 0, Qt::AlignVCenter);
    layout->addWidget(m_iconLabel, 0, Qt::AlignVCenter);
    setLayout(layout);

    m_iconLabel->setFixedWidth(30);

    const QList<Solid::Device> devices =
            Solid::Device::listFromType(Solid::DeviceInterface::Battery, QString());

    if (devices.isEmpty()) {
        qDebug() << "No Battery";
    }

    for (Solid::Device device : devices) {
        Solid::Battery *battery = device.as<Solid::Battery>();

        if (battery->type() == Solid::Battery::PrimaryBattery) {
            m_primaryBattery = battery;
            connect(m_primaryBattery, &Solid::Battery::chargePercentChanged, this,
                    &BatteryWidget::updateIcon);
            connect(m_primaryBattery, &Solid::Battery::chargeStateChanged, this,
                    &BatteryWidget::updateIcon);
        }
    }

    updateIcon();
}

BatteryWidget::~BatteryWidget() { }

QString BatteryWidget::popupText()
{
    if (m_primaryBattery) {
        return QString("%1% - %2")
                .arg(m_primaryBattery->chargePercent())
                .arg(stateToString(m_primaryBattery->chargeState()));
    }

    return QString();
}

void BatteryWidget::updateIcon()
{
    Solid::Battery::ChargeState state = m_primaryBattery->chargeState();
    int percent = m_primaryBattery->chargePercent();
    int range = percent / 10 * 10;

    switch (state) {
    case Solid::Battery::ChargeState::Charging:
        m_iconPixmap = QIcon::fromTheme(QString("battery-level-%1-charging-symbolic").arg(range))
                               .pixmap(24, 24);
        break;
    case Solid::Battery::ChargeState::Discharging:
        m_iconPixmap =
                QIcon::fromTheme(QString("battery-level-%1-symbolic").arg(range)).pixmap(24, 24);
        break;
    case Solid::Battery::ChargeState::FullyCharged:
        m_iconPixmap =
                QIcon::fromTheme(QString("battery-level-%1-symbolic").arg(100)).pixmap(24, 24);
        break;
    }

    m_iconLabel->setPixmap(m_iconPixmap);
    // m_percentLabel->setText(QString("%1%").arg(percent));

    QWidget::update();

    qDebug() << "update: " << state << percent;
}

QString BatteryWidget::stateToString(Solid::Battery::ChargeState state)
{
    switch (state) {
    case Solid::Battery::NoCharge:
        return tr("Empty");
    case Solid::Battery::Discharging:
        return tr("Discharging");
    case Solid::Battery::FullyCharged:
        return tr("Fully charged");
    case Solid::Battery::Charging:
    default:
        return tr("Charging");
    }
}
