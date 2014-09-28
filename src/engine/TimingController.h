#ifndef TIMINGCONTROLLER_H
#define TIMINGCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QTime>
#include <QDebug>

class TimingController : public QObject {
    Q_OBJECT
public:
    TimingController(QObject *parent = 0);
    void setDelay(quint16 delay);

public slots:
    void frameUpdated(void);

private:
    QTime m_time;
    QTimer m_timer;
    quint8 m_delay, m_factDelay;
    quint8 m_count;
    quint16 m_late[32];
    bool m_delaySet;

signals:
    void updateFrame(void);
};

#endif // TIMINGCONTROLLER_H
