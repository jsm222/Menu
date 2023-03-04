#include "controlwidget.h"
#include <QHBoxLayout>
#include <QPainter>

ControlWidget::ControlWidget(QWidget *parent) : QWidget(parent), m_widget(new VolumeWidget)
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setAlignment(Qt::AlignCenter); // Center QHBoxLayout vertically
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    setFixedSize(QSize(30, 30));

    connect(m_widget, &VolumeWidget::requestUpdateIcon, this, [=](QString iconName) {
        m_iconPixmap = QIcon::fromTheme(iconName).pixmap(24, 24);
        QWidget::update();
    });
}

QString ControlWidget::popupText()
{
    return QString(tr("Current volume") + ": %1%").arg(m_widget->volume());
}

void ControlWidget::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);
    const QRectF &pixmapRect = QRectF(m_iconPixmap.rect());
    painter.drawPixmap(rect().center() - pixmapRect.center() / m_iconPixmap.devicePixelRatioF(),
                       m_iconPixmap);
}
