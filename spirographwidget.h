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
    QSize minimumSizeHint() const;
    QSize sizeHint() const;


    
signals:
    
public slots:

    void Advance();
    void SetTime(__uint64_t ns);
    void BeginAnimation();
    void StopAnimation();
    void AnimatePeriod();
    // 0 <= deg_offset <= 360, rpm > 0
    void AddMirror(int deg_offset, int rpm);
    void ClearMirrors();
    void SetMirrorRPM(int index, __int64_t rpm);
    void SetMirrorOffset(int index, __int64_t offset);

private slots:
    void timedOut();

protected:
    void paintEvent(QPaintEvent *event);

private:
    void calculatePeriod();
    __int64_t GCD(__int64_t a, __int64_t b);
    __int64_t LCM(__int64_t a, __int64_t b);

    __uint64_t m_time;      // in us
    __uint64_t m_period;


    QTimer *timer;

    QList<double> *mirror_offset;       //in radians
    QList<double> *mirror_angular_velocity;      //In rad/us

    bool current_value_valid;
    double current_pos_x;
    double current_pos_y;
};

#endif // SPIROGRAPHWIDGET_H
