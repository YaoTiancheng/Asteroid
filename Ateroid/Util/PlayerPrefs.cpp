#include "Precompile.h"
#include "PlayerPrefs.h"
#include "Archives.h"

namespace ASTEROID_NAMESPACE
{
    static const char kPlayerPrefsFilename[] = "PlayerPrefs.json";
    PlayerPrefs* PlayerPrefs::_Singleton = nullptr;

    void PlayerPrefs::Save()
    {
        std::ofstream fs(kPlayerPrefsFilename);
        JSONOutputArchive archive(fs);
        archive(ASTEROID_ARCHIVE_MAKE_NVP("Integers", m_IntegerValues));
        archive(ASTEROID_ARCHIVE_MAKE_NVP("Singles", m_SingleValues));
        archive(ASTEROID_ARCHIVE_MAKE_NVP("Strings", m_StringValues));
    }

    bool PlayerPrefs::Load()
    {
        std::ifstream fs(kPlayerPrefsFilename);
        if (fs)
        {
            JSONInputArchive archive(fs);
            archive(ASTEROID_ARCHIVE_MAKE_NVP("Integers", m_IntegerValues));
            archive(ASTEROID_ARCHIVE_MAKE_NVP("Singles", m_SingleValues));
            archive(ASTEROID_ARCHIVE_MAKE_NVP("Strings", m_StringValues));
            return true;
        }
        ASTEROID_LOG_INFO_F("Open PlayerPref file \"%s\" failed with err \"%s\"",
            kPlayerPrefsFilename,
            strerror(errno));
        return false;
    }
}