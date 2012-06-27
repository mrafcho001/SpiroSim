#include "spirographwidget.h"
#include <QPainter>
#include <QtCore/qmath.h>
#include <QDebug>

SpiroGraphWidget::SpiroGraphWidget(QWidget *parent) :
    QWidget(parent), m_theta(0), m_granularity(2*PI/30), m_currentStep(0), m_maxRatio(0), m_timer(NULL)
{
    QPalette p = this->palette();
    p.setColor(this->backgroundRole(), Qt::black);
    this->setPalette(p);
    setAutoFillBackground(true);

    m_mirrorRatios = new QList<int>();
    m_mirrorOffset = new QList<double>();
}

SpiroGraphWidget::~SpiroGraphWidget()
{
    delete m_mirrorRatios;
    delete m_mirrorOffset;
    if(m_timer) delete m_timer;
}

QSize SpiroGraphWidget::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize SpiroGraphWidget::sizeHint() const
{
    return QSize(400, 400);
}

void SpiroGraphWidget::IncreaseGranularity()
{
    m_granularity /= 2.0;
}

void SpiroGraphWidget::DecreaseGranularity()
{
    m_granularity *= 2.0;
}

void SpiroGraphWidget::Advance()
{
    timedOut();
}

void SpiroGraphWidget::BeginAnimation()
{
    m_timer = new QTimer(this);
    m_timer->setInterval(20);
    m_timer->start();

    connect(m_timer, SIGNAL(timeout()), this, SLOT(timedOut()));

    m_currentStep = 1;
}

void SpiroGraphWidget::StopAnimation()
{
    m_timer->stop();
    disconnect(m_timer, SIGNAL(timeout()), this, SLOT(timedOut()));
    delete m_timer;
}

void SpiroGraphWidget::AnimatePeriod()
{
    calculateTheta();
    qDebug() << "Total Theta: " << m_theta;
    BeginAnimation();
}

void SpiroGraphWidget::NoAnimateUpdate()
{
    calculateTheta();
    m_currentStep = qCeil(m_theta/m_granularity);
    update();
}

void SpiroGraphWidget::AddMirror(int deg_offset, int ratio)
{
    m_mirrorOffset->append((deg_offset%360)/180.0*PI);
    m_mirrorRatios->append(ratio);
}

void SpiroGraphWidget::ClearMirrors()
{
    m_mirrorOffset->clear();
    m_mirrorRatios->clear();
}

void SpiroGraphWidget::SetMirrorRPM(int index, __int64_t rpm)
{
    (void)index;
    (void)rpm;
}

void SpiroGraphWidget::SetMirrorOffset(int index, __int64_t offset)
{
    (void)index;
    (void)offset;
}

void SpiroGraphWidget::timedOut()
{
    if(m_currentStep * m_granularity < m_theta)
        m_currentStep++;
    else
        this->StopAnimation();

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

    if(m_mirrorRatios->size() == 0)
        return;

    painter.translate(this->width()/2, this->height()/2);
    int scale = qMin(this->width()*.45,this->height()*.45);
    painter.scale(scale, -scale);

    double x_prev =0.0, y_prev=0.0;

    for(int itr = 0; itr < m_mirrorOffset->size(); itr++)
    {
        x_prev += qCos((*m_mirrorOffset)[itr]);
        y_prev += qSin((*m_mirrorOffset)[itr]);
    }

    x_prev /= m_mirrorOffset->size();
    y_prev /= m_mirrorOffset->size();

    for(int i = 1; i < m_currentStep; i++)
    {
        double x = 0, y = 0;
        for(int itr = 0; itr < m_mirrorRatios->size(); itr++)
        {
            double dTheta = (*m_mirrorOffset)[itr] + i * (m_granularity * (*m_mirrorRatios)[itr])/m_maxRatio;
            x += qCos(dTheta);
            y += qSin(dTheta);
        }

        x /= m_mirrorRatios->size();
        y /= m_mirrorRatios->size();

        painter.drawLine(QLineF(x_prev, y_prev, x, y));
        x_prev = x;
        y_prev = y;
    }
}

void SpiroGraphWidget::calculateTheta()
{
    int rotations = 1;

    if(m_mirrorRatios->size() > 1)
    {
        m_maxRatio = rotations = (*m_mirrorRatios)[0];
        for(int i = 1; i < m_mirrorOffset->size(); i++)
        {
            rotations = LCM(rotations, (*m_mirrorRatios)[i]);
            if((*m_mirrorRatios)[i] > m_maxRatio)
                m_maxRatio = (*m_mirrorRatios)[i];
        }
    }

    m_theta = PI * 2 * rotations + m_granularity;
}

int SpiroGraphWidget::GCD(int a, int b)
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

int SpiroGraphWidget::LCM(int a, int b)
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
