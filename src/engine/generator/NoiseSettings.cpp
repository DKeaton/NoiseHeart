#include "NoiseSettings.h"

Settings &Settings::operator = (const Settings &b) {
    memcpy(octaves, b.octaves, 6);
    memcpy(amplitudes, b.amplitudes, 6);
    return *this;
}

bool Settings::operator != (const Settings &b) const {
    for (quint8 i = 0; i < 6; i++) {
        if (octaves[i] != b.octaves[i] || amplitudes[i] != b.amplitudes[i]) {
            return true;
        }
    }
    return false;
}
