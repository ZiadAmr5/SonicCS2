#ifndef SFX_H
#define SFX_H

#include <QSoundEffect>
#include <QUrl>
#include <QString>

// Game audio.
//  - Sfx::play(id)  : short low-latency sound effects (QSoundEffect)
//  - Sfx::startMusic(): looping background music
//
// All clips live in assets.qrc. If a clip is missing, playing it is simply a no-op,
// so the game still runs fine without audio.
namespace Sfx
{
enum Id { Jump = 0, Coin, PowerUp, Fireball, Stomp, Die, OneUp, LevelClear, Bump, COUNT };

inline QSoundEffect* effect(Id id)
{
    static QSoundEffect* fx[COUNT] = {};
    static const char* const files[COUNT] = {
        "qrc:/assets/assets/smb3_jump.wav",
        "qrc:/assets/assets/smb3_coin.wav",
        "qrc:/assets/assets/smb3_power-up.wav",
        "qrc:/assets/assets/smb3_fireball.wav",
        "qrc:/assets/assets/smb3_stomp.wav",
        "qrc:/assets/assets/smb3_player_down.wav",
        "qrc:/assets/assets/smb3_1-up.wav",
        "qrc:/assets/assets/smb3_level_clear.wav",
        "qrc:/assets/assets/smb3_bump.wav",
    };
    if (!fx[id]) {
        fx[id] = new QSoundEffect;
        fx[id]->setSource(QUrl(QString::fromLatin1(files[id])));
        fx[id]->setVolume(0.6);
    }
    return fx[id];
}

// Re-triggers the effect from the start. Safe to call even if the clip is missing.
inline void play(Id id)
{
    QSoundEffect* e = effect(id);
    if (e->status() == QSoundEffect::Error) return;
    e->play();
}

// Load every clip up front so the first play() isn't delayed.
inline void preload() { for (int i = 0; i < COUNT; ++i) effect(Id(i)); }

} // namespace Sfx

#endif // SFX_H
