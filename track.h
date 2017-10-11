#ifndef TRACK_H
#define TRACK_H

#include <QString>
#include <QVector>

struct Image {
    Image() {}
    Image(int h, int w, QString url) : height(h), width(w), url(url) {}

    int height;
    int width;
    QString url;
};

struct Album {
    QString id;
    QString name;
    Image *image;
};

struct Artist {
    QString id;
    QString name;
};

class Track
{
public:
    Track();

    const QString& getAddedAt() { return m_addedAt; }
    const QString& getAddedBy() { return m_addedBy; }
    int getDiscNumber() { return m_discNumber; }
    int getDuration() { return m_duration; }
    const QString& getId() { return m_id; }
    const QString& getName() { return m_name; }
    double getPopularity() { return m_popularity; }
    int getTrackNumber() { return m_trackNumber; }
    const QString& getPreviewUrl() { return m_previewUrl; }
    const Album& getAlbum() { return m_album; }
    QVector<Artist>& getArtists() { return m_artists; }

    void setAddedAt(const QString& addedAt) { m_addedAt = addedAt; }
    void setAddedBy(const QString& addedBy) { m_addedBy = addedBy; }
    void setDiscNumber(int discNumber) { m_discNumber = discNumber; }
    void setDuration(int duration) { m_duration = duration; }
    void setId(const QString& id) { m_id = id; }
    void setName(const QString& name) { m_name = name; }
    void setPopularity(double popularity) { m_popularity = popularity; }
    void setTrackNumber(int trackNumber) { m_trackNumber = trackNumber; }
    void setPreviewUrl(const QString& previewUrl) { m_previewUrl = previewUrl; }
    void setAlbum(const Album& album) { m_album = album; }

private:
    QString m_addedAt;
    QString m_addedBy;
    int m_discNumber;
    int m_duration;
    QString m_id;
    QString m_name;
    double m_popularity;
    int m_trackNumber;
    QString m_previewUrl;

    Album m_album;
    QVector<Artist> m_artists;

};

#endif // TRACK_H
