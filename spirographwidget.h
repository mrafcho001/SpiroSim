#ifndef SPIROGRAPHWIDGET_H
#define SPIROGRAPHWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QList>

#define PI  3.14159265

class SpiroGraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SpiroGraphWidget(QWidget *parent = 0);
    ~SpiroGraphWidget();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;


    
signals:
    
public slots:

    void IncreaseGranularity();
    void DecreaseGranularity();
    void Advance();
    void BeginAnimation();
    void StopAnimation();
    void AnimatePeriod();
    void NoAnimateUpdate();
    // 0 <= deg_offset <= 360, rpm > 0
    void AddMirror(int deg_offset, int ratio);
    void ClearMirrors();
    void SetMirrorRPM(int index, __int64_t rpm);
    void SetMirrorOffset(int index, __int64_t offset);

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

    QList<int> *m_mirrorRatios;
    QList<double> *m_mirrorOffset;

    QTimer *m_timer;

};

#endif // SPIROGRAPHWIDGET_H
