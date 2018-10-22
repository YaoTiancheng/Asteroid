#include "Precompile.h"
#include "Asteroid.h"
#include "PlayerPrefs.h"
#include "Archives.h"

namespace ASTEROID_NAMESPACE
{
    PlayerPrefs::NamedValueMap<int32_t>     PlayerPrefs::_IntegerValues;
    PlayerPrefs::NamedValueMap<float>       PlayerPrefs::_SingleValues;
    PlayerPrefs::NamedValueMap<std::string> PlayerPrefs::_StringValues;

    static const char kPlayerPrefsFilename[] = "PlayerPrefs.json";

    void PlayerPrefs::Save()
    {
        std::ofstream fs(kPlayerPrefsFilename);
        JSONOutputArchive archive(fs);
        archive(ASTEROID_ARCHIVE_MAKE_NVP("Integers", _IntegerValues));
        archive(ASTEROID_ARCHIVE_MAKE_NVP("Singles", _SingleValues));
        archive(ASTEROID_ARCHIVE_MAKE_NVP("Strings", _StringValues));
    }

    bool PlayerPrefs::Load()
    {
        std::ifstream fs(kPlayerPrefsFilename);
        if (fs)
        {
            JSONInputArchive archive(fs);
            archive(ASTEROID_ARCHIVE_MAKE_NVP("Integers", _IntegerValues));
            archive(ASTEROID_ARCHIVE_MAKE_NVP("Singles", _SingleValues));
            archive(ASTEROID_ARCHIVE_MAKE_NVP("Strings", _StringValues));
            return true;
        }
        ASTEROID_LOG_INFO_F("Open PlayerPref file \"%s\" failed with err \"%s\"",
            kPlayerPrefsFilename,
            strerror(errno));
        return false;
    }
}