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
    QVector<Track>& getTracks() { return m_tracks; }
    Image* getImage() { return m_image; }

    void setName(const QString& name) { m_name = name; }
    void setId(const QString& id) { m_id = id; }
    void setImage(Image* image) { m_image = image; }

    void addTrack(Track track) { m_tracks.push_back(track); }
    void removeTrack(QString id);

private:
    QString m_id;
    QString m_name;
    QVector<Track> m_tracks;
    Image *m_image;
};

#endif // PLAYLIST_H
