#ifndef BASICCONTROLPANEL_H
#define BASICCONTROLPANEL_H

#include <QGroupBox>
#include <QSignalMapper>
#include <QVariant>

#include "../../../utility/Requestable.h"

namespace Interface {

    class BasicControlPanel : public QGroupBox, public Requestable {
        Q_OBJECT
    public:
        BasicControlPanel(const QString &title, QWidget *parent = 0);
        virtual ~BasicControlPanel(void);

    protected:
        void registerSignal(QObject *sender, const char *signal, qint32 id);

    private:
        QSignalMapper *m_mapper;

    private slots:
        virtual void internalRequestIn(int);

    signals:
        void requestOut(quint8 request, const QVariant &arg0, const QVariant &arg1);
    };

}

#endif // BASICCONTROLPANEL_H
