#include "ComplexControlPanel.h"

namespace Interface {

    ComplexControlPanel::ComplexControlPanel(QWidget *parent)
        : QWidget(parent) {
    }

    ComplexControlPanel::~ComplexControlPanel(void) {
        for (QMap<BasicControlPanel *, quint8>::Iterator iter = m_map.begin();
             iter != m_map.end(); iter++) {
            iter.key()->deleteLater();
        }
    }

    void ComplexControlPanel::registerSignal(BasicControlPanel *sender, qint32 id) {
        if (!m_map.contains(sender)) {
            connect(sender, SIGNAL(requestOut(quint8,QVariant,QVariant)), SLOT(internalRequestIn(quint8,QVariant,QVariant)));
        }
        m_map[sender] = id;
    }

    qint32 ComplexControlPanel::getSenderId(BasicControlPanel *sender) const {
        if (!m_map.contains(sender)) {
            throw new NotSupportedException("This type of internal request is not supported.");
        }
        return m_map[sender];
    }

    void ComplexControlPanel::internalRequestIn(quint8, const QVariant &, const QVariant &) {
        throw new NotSupportedException("Processing internal requests is not supported for class.");
    }

}
