#ifndef NOISESETTINGS_H
#define NOISESETTINGS_H

#include <QtGlobal>
#include <memory.h>

struct Settings {
    Settings &operator = (const Settings &b);
    bool operator != (const Settings &b) const;

    bool octaves[6];
    quint8 amplitudes[6];
};

#endif // NOISESETTINGS_H
