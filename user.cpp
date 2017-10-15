#include "user.h"

User::User()
{

}

Playlist User::getPlayListById(const QString id)
{
    for(Playlist playlist : m_playlists) {
        if (playlist.getId() == id)
            return playlist;
    }
}
