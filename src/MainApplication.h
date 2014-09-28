#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QDebug>

#include "DataStore.h"

class MainApplication : public QThread, private DataStore {
    Q_OBJECT

public:
    MainApplication(QObject *parent = 0);
    ~MainApplication(void);

private:
    void setUp(void);
    quint8 getLayoutId(quint8 layout) const;

private slots:
    void lineRequested(quint8 *dst, quint8 layout);
    void internalRequestIn(quint8 type, quint8 id, quint8 request, const QVariant &arg0, const QVariant &arg1);
};

#endif // MAINAPPLICATION_H
