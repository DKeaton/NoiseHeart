#include "BasicControlPanel.h"

namespace Interface {

    BasicControlPanel::BasicControlPanel(const QString &title, QWidget *parent)
        : QGroupBox(title, parent), m_mapper(new QSignalMapper(this)) {
        connect(m_mapper, SIGNAL(mapped(int)), SLOT(internalRequestIn(int)));
    }

    BasicControlPanel::~BasicControlPanel(void) {
        delete m_mapper;
    }

    void BasicControlPanel::registerSignal(QObject *sender, const char *signal, qint32 id) {
        connect(sender, signal, m_mapper, SLOT(map()));
        m_mapper->setMapping(sender, id);
    }

    void BasicControlPanel::internalRequestIn(int) {
        throw new NotSupportedException("Processing internal requests is not supported for class.");
    }

}
