#include "spirographwidget.h"
#include <QPainter>
#include <QtCore/qmath.h>
#include <QDebug>

SpiroGraphWidget::SpiroGraphWidget(QWidget *parent) :
    QWidget(parent), m_time(1), timer(NULL), current_value_valid(false)
{
    QPalette p = this->palette();
    p.setColor(this->backgroundRole(), Qt::black);
    this->setPalette(p);
    setAutoFillBackground(true);
    mirror_offset = new QList<double>();
    mirror_angular_velocity = new QList<double>();

    mirror_offset->append(0.0);
    mirror_offset->append(1.0);
    mirror_angular_velocity->append(100000.0);
    mirror_angular_velocity->append(160000.0);
}

QSize SpiroGraphWidget::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize SpiroGraphWidget::sizeHint() const
{
    return QSize(400, 400);
}

void SpiroGraphWidget::Advance()
{
    if(timer)
        StopAnimation();
    else
        BeginAnimation();

    update();
}

void SpiroGraphWidget::SetTime(__uint64_t ns)
{
}

void SpiroGraphWidget::BeginAnimation()
{
    timer = new QTimer(this);
    timer->setInterval(20);
    timer->start();

    connect(timer, SIGNAL(timeout()), this, SLOT(timedOut()));
}

void SpiroGraphWidget::StopAnimation()
{
    timer->stop();
    disconnect(timer, SIGNAL(timeout()), this, SLOT(timedOut()));
    delete timer;
}

void SpiroGraphWidget::AnimatePeriod()
{
    calculatePeriod();
    qDebug() << "Period: " << m_period;
    BeginAnimation();
}

void SpiroGraphWidget::AddMirror(int deg_offset, int rpm)
{
    mirror_offset->append((deg_offset%360)/180.0*PI);
    mirror_angular_velocity->append(rpm*PI/30000000);
}

void SpiroGraphWidget::ClearMirrors()
{
    mirror_offset->clear();
    mirror_angular_velocity->clear();
}

void SpiroGraphWidget::SetMirrorRPM(int index, __int64_t rpm)
{
}

void SpiroGraphWidget::SetMirrorOffset(int index, __int64_t offset)
{
}

void SpiroGraphWidget::timedOut()
{
    m_time++;
    update();
}

void SpiroGraphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(Qt::blue);

    painter.drawLine(0, this->height()/2, this->width(), this->height()/2);
    painter.drawLine(this->width()/2, 0, this->width()/2, this->height());

    if(mirror_offset->size() == 0)
        return;

    painter.translate(this->width()/2, this->height()/2);
    int scale = qMin(this->width()*.45,this->height()*.45);
    painter.scale(scale, -scale);

    double x_prev =0.0, y_prev=0.0;

    for(int itr = 0; itr < mirror_offset->size(); itr++)
    {
        x_prev += qCos((*mirror_offset)[itr]);
        y_prev += qSin((*mirror_offset)[itr]);
    }

    x_prev /= mirror_offset->size();
    y_prev /= mirror_offset->size();

    for(__uint64_t i = 1; i < m_period; i++)
    {
        double x = 0, y = 0;
        for(int itr = 0; itr < mirror_offset->size(); itr++)
        {
            x += qCos((*mirror_offset)[itr] + (*mirror_angular_velocity)[itr]/1000000.0*i);
            y += qSin((*mirror_offset)[itr] + (*mirror_angular_velocity)[itr]/1000000.0*i);
        }

        x /= mirror_offset->size();
        y /= mirror_offset->size();

        painter.drawLine(QLineF(x_prev, y_prev, x, y));
        x_prev = x;
        y_prev = y;
    }

}

void SpiroGraphWidget::calculatePeriod()
{
    m_period = (__int64_t)((*mirror_angular_velocity)[0]*30000000/PI);

    if(mirror_offset->size() > 1)
    {
        for(int i = 1; i < mirror_offset->size(); i++)
            m_period = LCM(m_period, (__int64_t)((*mirror_angular_velocity)[i]*30000000/PI));
    }
}

__int64_t SpiroGraphWidget::GCD(__int64_t a, __int64_t b)
{
    __int64_t v1 = a;
    __int64_t v2 = b;
    __int64_t gcd = 1;

    if (v1>v2 && v1 % v2==0)
        return b;
    else if (v1>v2 && v2 % v1==0)
        return a;

    while (v2 != 0)
    {
        gcd = v2;
        v2 = v1 % v2;
        v1 = gcd;
    }
    return gcd;

}

__int64_t SpiroGraphWidget::LCM(__int64_t a, __int64_t b)
{
    if(a == 0)
        return b;
    else if(b == 0)
        return a;
    else if(a == b)
        return a;

    __int64_t v1 = qAbs(a);
    __int64_t v2 = qAbs(b);

    v1 = v1/GCD(v1,v2);
    return v1*v2;
}
