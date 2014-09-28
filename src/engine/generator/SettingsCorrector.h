#ifndef SETTINGSCORRECTOR_H
#define SETTINGSCORRECTOR_H

#include "NoiseSettings.h"

class SettingsCorrector {
public:
    SettingsCorrector(void);
    ~SettingsCorrector(void);
    void newSettings(const Settings &sets);
    const Settings *getSettngs(void) const;
    void correctSettings(quint8 step);

private:
    Settings m_tarSettings;
    Settings *m_curSettings;
};

#endif // SETTINGSCORRECTOR_H
