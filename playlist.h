#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QString>

class Playlist
{
public:
    Playlist();

    const QString& getId() { return m_id; }
    const QString& getName() { return m_name; }

    void setName(const QString& name) { m_name = name; }
    void setId(const QString& id) { m_id = id; }

private:
    QString m_id;
    QString m_name;
};

#endif // PLAYLIST_H
