#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QString>
#include <QVector>
#include "track.h"

class Playlist
{
public:
    Playlist();

    const QString& getId() { return m_id; }
    const QString& getName() { return m_name; }
    const QVector<Track>& getTracks() { return m_tracks; }

    void setName(const QString& name) { m_name = name; }
    void setId(const QString& id) { m_id = id; }
    void setTracks(const QVector<Track>& tracks) { m_tracks = tracks; }

    void addTrack(Track track) { m_tracks.push_back(track); }
    void removeTrack(QString id);

private:
    QString m_id;
    QString m_name;
    QVector<Track> m_tracks;
};

#endif // PLAYLIST_H
