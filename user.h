#ifndef USER_H
#define USER_H

#include "playlist.h"

class User
{
public:
    User();

    const QString& getId() { return m_id; }
    const QString& getCountry() { return m_country; }
    QVector<Playlist>& getPlaylists() { return m_playlists; }

    void setId(const QString& id) { m_id = id; }
    void setCountry(const QString country) { m_country = country; }

private:
    QString m_id;
    QString m_country;
    QVector<Playlist> m_playlists;
};

#endif // USER_H
