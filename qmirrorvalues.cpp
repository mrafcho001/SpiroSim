#include "qmirrorvalues.h"
#include <qmath.h>

QMirrorValues::QMirrorValues(QObject *parent) :
    QObject(parent), m_offset(0), m_ratio(0)
{
}

QMirrorValues::QMirrorValues(int ratio, double offset, QObject *parent) :
    QObject(parent), m_offset(offset), m_ratio(ratio)
{
}

void QMirrorValues::SetOffset(double offset)
{
    if(offset < 0.0 || offset > M_PI)
        return;
    m_offset = offset;
}
void QMirrorValues::SetRatio(int ratio)
{
    m_ratio = ratio;
}
