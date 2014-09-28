#ifndef COMPLEXCONTROLPANEL_H
#define COMPLEXCONTROLPANEL_H

#include <QMap>

#include "../basic/BasicControlPanel.h"

#include "../../../utility/Requestable.h"

namespace Interface {

    class ComplexControlPanel : public QWidget, public Requestable {
        Q_OBJECT
    public:
        ComplexControlPanel(QWidget *parent = 0);
        virtual ~ComplexControlPanel(void);

    protected:
        void registerSignal(BasicControlPanel *sender, qint32 id);
        qint32 getSenderId(BasicControlPanel *sender) const;

    private:
        QMap<BasicControlPanel *, quint8> m_map;

    private slots:
        virtual void internalRequestIn(quint8, const QVariant&, const QVariant&);

    signals:
        void requestOut(quint8 id, quint8 request, const QVariant &arg0, const QVariant &arg1);
    };

}

#endif // COMPLEXCONTROLPANEL_H
