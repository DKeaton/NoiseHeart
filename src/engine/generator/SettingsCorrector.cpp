#include "SettingsCorrector.h"

SettingsCorrector::SettingsCorrector(void)
    : m_curSettings(new Settings()) {
    memset(m_curSettings, 0, sizeof(Settings));
}

SettingsCorrector::~SettingsCorrector(void) {
    delete m_curSettings;
}

void SettingsCorrector::newSettings(const Settings &sets) {
    m_tarSettings = sets;
}

const Settings *SettingsCorrector::getSettngs(void) const {
    return m_curSettings;
}

void SettingsCorrector::correctSettings(quint8 step) {
    if (*m_curSettings != m_tarSettings) {
        for (quint8 i = 0; i < 6; i++) {
            bool diffStates = (m_curSettings->octaves[i] != m_tarSettings.octaves[i]);
            quint8 tar = (m_tarSettings.octaves[i]) ? m_tarSettings.amplitudes[i] : 0;
            if (tar != m_curSettings->amplitudes[i] || diffStates) {
                quint8 diff = qAbs(m_curSettings->amplitudes[i] - tar);
                m_curSettings->amplitudes[i] += ((step > diff) ? diff : step) * ((m_curSettings->amplitudes[i] > tar) ? -1 : 1);
                if (diffStates) {
                    if (m_tarSettings.octaves[i] && m_curSettings->amplitudes[i] != 0) {
                        m_curSettings->octaves[i] = true;
                    } else if (!m_tarSettings.octaves[i] && m_curSettings->amplitudes[i] == 0) {
                        m_curSettings->octaves[i] = false;
                    }
                }
            }
        }
    }
}
