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

    m_mirrors = new QList<QMirrorValues*>();
}

SpiroGraphWidget::~SpiroGraphWidget()
{
    delete m_mirrors;
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

double SpiroGraphWidget::GetGranularity() const
{
    return m_granularity;
}

void SpiroGraphWidget::IncreaseGranularity()
{
    m_granularity /= 2.0;
}

void SpiroGraphWidget::DecreaseGranularity()
{
    m_granularity *= 2.0;
}

void SpiroGraphWidget::SetGranularity(double granularity)
{
    m_granularity = qAbs(granularity);
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

bool SpiroGraphWidget::AddMirror(QMirrorValues *mirror)
{
    if(m_mirrors->contains(mirror))
        return false;

    m_mirrors->append(mirror);
    return true;
}

void SpiroGraphWidget::ClearMirrors()
{
    m_mirrors->clear();
}

void SpiroGraphWidget::SetMirrorRatio(int index, int ratio)
{
    (*m_mirrors)[index]->SetRatio(ratio);
}

void SpiroGraphWidget::SetMirrorOffset(int index, double offset)
{
    (*m_mirrors)[index]->SetOffset(offset);
}

bool SpiroGraphWidget::RemoveMirror(QMirrorValues *mirror)
{
    return m_mirrors->removeOne(mirror);
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

    if(m_mirrors->size() == 0)
        return;

    painter.translate(this->width()/2, this->height()/2);
    int scale = qMin(this->width()*.45,this->height()*.45);
    painter.scale(scale, -scale);

    double x_prev =0.0, y_prev=0.0;

    for(QList<QMirrorValues*>::iterator itr = m_mirrors->begin(); itr != m_mirrors->end(); ++itr)
    {
        x_prev += qCos((*itr)->GetOffset());
        y_prev += qSin((*itr)->GetOffset());
    }

    x_prev /= m_mirrors->size();
    y_prev /= m_mirrors->size();

    for(int i = 1; i < m_currentStep; i++)
    {
        double x = 0, y = 0;
        for(QList<QMirrorValues*>::iterator itr = m_mirrors->begin(); itr != m_mirrors->end(); ++itr)
        {
            double dTheta = (*itr)->GetOffset() + i * (m_granularity * (*itr)->GetRatio())/m_maxRatio;
            x += qCos(dTheta);
            y += qSin(dTheta);
        }

        x /= m_mirrors->size();
        y /= m_mirrors->size();

        painter.drawLine(QLineF(x_prev, y_prev, x, y));
        x_prev = x;
        y_prev = y;
    }
}

void SpiroGraphWidget::calculateTheta()
{
    int rotations = 1;

    if(m_mirrors->size() > 1)
    {
        m_maxRatio = rotations = (*m_mirrors)[0]->GetRatio();
        for(QList<QMirrorValues*>::iterator itr = m_mirrors->begin()++; itr != m_mirrors->end(); ++itr)
        //for(int i = 1; i < m_mirrorOffset->size(); i++)
        {
            rotations = LCM(rotations, (*itr)->GetRatio());
            if(qAbs((*itr)->GetRatio()) > m_maxRatio)
                m_maxRatio = qAbs((*itr)->GetRatio());
        }
    }

    m_theta = PI * 2 * rotations + m_granularity;
}

int SpiroGraphWidget::GCD(int a, int b)
{
    int v1 = a;
    int v2 = b;
    int gcd = 1;

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

    int v1 = qAbs(a);
    int v2 = qAbs(b);

    v1 = v1/GCD(v1,v2);
    return v1*v2;
}
