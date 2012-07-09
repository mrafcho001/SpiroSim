#ifndef QMIRRORVALUES_H
#define QMIRRORVALUES_H

#include <QObject>
#include <QMetaType>

class QMirrorValues : public QObject
{
    Q_OBJECT
public:
    explicit QMirrorValues(QObject *parent = 0);
    QMirrorValues(int ratio, double offset, QObject *parent = 0);

    inline int GetRatio() const { return m_ratio; }
    inline double GetOffset() const { return m_offset; }

public slots:
    void SetOffset(double offset);
    void SetRatio(int ratio);

private:
    double m_offset;
    int m_ratio;
    
};

Q_DECLARE_METATYPE(QMirrorValues*)

#endif // QMIRRORVALUES_H
