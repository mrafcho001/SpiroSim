#ifndef SPIROGRAPHWIDGET_H
#define SPIROGRAPHWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QList>
#include "qmirrorvalues.h"

#define PI  3.14159265

class SpiroGraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SpiroGraphWidget(QWidget *parent = 0);
    ~SpiroGraphWidget();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    double GetGranularity() const;

signals:
    
public slots:

    void IncreaseGranularity();
    void DecreaseGranularity();
    void SetGranularity(double granularity);

    void BeginAnimation();
    void StopAnimation();
    void Advance();
    void AnimatePeriod();
    void NoAnimateUpdate();

    bool AddMirror(QMirrorValues *mirror);
    void ClearMirrors();
    void SetMirrorRatio(int index, int ratio);
    void SetMirrorOffset(int index, double offset);
    bool RemoveMirror(QMirrorValues *mirror);

private slots:
    void timedOut();

protected:
    void paintEvent(QPaintEvent *event);

private:
    void calculateTheta();
    int GCD(int a, int b);
    int LCM(int a, int b);

    double m_theta;
    double m_granularity;
    int m_currentStep;
    int m_maxRatio;

    QList<QMirrorValues*> *m_mirrors;
    QTimer *m_timer;

};

#endif // SPIROGRAPHWIDGET_H
