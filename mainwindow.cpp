#include "mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QScrollArea(parent)
{
    setWidgetResizable(true);
    setMinimumWidth(800);
    setMinimumHeight(600);
    setFrameShape(QFrame::NoFrame);

    QWidget *proxyWidget = new QWidget;
    setWidget(proxyWidget);

    m_gridLayout = new QGridLayout;
    proxyWidget->setLayout(m_gridLayout);

    m_plainText = new QPlainTextEdit();
//    m_gridLayout->addWidget(new QLabel("Spotify"),0,0);
//    m_gridLayout->addWidget(m_plainText, 1, 0);

    spotify.setReplyHandler(new QOAuthHttpServerReplyHandler(8080, this));
    spotify.setAuthorizationUrl(QUrl("https://accounts.spotify.com/authorize"));
    spotify.setAccessTokenUrl(QUrl("https://accounts.spotify.com/api/token"));
    spotify.setClientIdentifier("31a72eb6056441a090521075d2166716");
    spotify.setClientIdentifierSharedKey("79552b59f96a4bc5b9234d24618e6b89");
    spotify.setScope("user-read-private user-top-read playlist-read-private playlist-modify-public playlist-modify-private");

    connect(&spotify, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
             &QDesktopServices::openUrl);

    connect(&spotify, &QOAuth2AuthorizationCodeFlow::granted,
            this, &MainWindow::granted);

    spotify.grant();
}

MainWindow::~MainWindow()
{
}


void MainWindow::granted ()
{
    getUserInfo();
}

void MainWindow::getUserInfo()
{
    QUrl u("https://api.spotify.com/v1/me");

    auto reply = spotify.get(u);

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
            return;
        }
        const auto data = reply->readAll();
        m_plainText->appendPlainText(data);

        const auto document = QJsonDocument::fromJson(data);
        const auto root = document.object();
        m_user.setId(root.value("id").toString());
        m_user.setCountry(root.value("country").toString());

        m_plainText->appendPlainText("Username: " + m_user.getId() + " from " + m_user.getCountry());

        reply->deleteLater();
        getPlayLists();
    });
}

void MainWindow::getPlayLists()
{
    if (m_user.getId().isEmpty())
        return;

    QUrl u("https://api.spotify.com/v1/users/" + m_user.getId() + "/playlists");

    auto reply = spotify.get(u);

    connect (reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
            return;
        }

        const auto data = reply->readAll();

        const auto document = QJsonDocument::fromJson(data);
        const auto root = document.object();

        m_plainText->appendPlainText(data);

        QVector<Playlist>& playlists = m_user.getPlaylists();
        for(const QJsonValue& item : root.value("items").toArray()) {
            Playlist playList;
            playList.setId(item.toObject().value("id").toString());
            playList.setName(item.toObject().value("name").toString());
            const QJsonArray& images = item.toObject().value("images").toArray();
            if(!images.isEmpty()) {
                const QJsonObject& firstImage = images[0].toObject(); //only load highest resolution image
                playList.setImage(new Image(firstImage.value("height").toInt(), firstImage.value("width").toInt(), firstImage.value("url").toString()));
            }
            playlists.push_back(playList);
        }

        for(Playlist p : playlists)
            qDebug() << p.getName() << p.getId();


        reply->deleteLater();
        getTracks();
    });
}

void MainWindow::getTracks()
{
    for(int i = 0; i < m_user.getPlaylists().size(); i++) {
        QUrl u("https://api.spotify.com/v1/users/" + m_user.getId() + "/playlists/" + m_user.getPlaylists()[i].getId() + "/tracks");

        auto reply = spotify.get(u);

        connect (reply, &QNetworkReply::finished, [this, reply, i]() {
            if (reply->error() != QNetworkReply::NoError) {
                qDebug() << reply->errorString();
                return;
            }

            const auto data = reply->readAll();

            const auto document = QJsonDocument::fromJson(data);
            const auto root = document.object();

            m_plainText->appendPlainText("\n\nTRACKS\n\n");
            m_plainText->appendPlainText(data);

            Playlist& playlist = m_user.getPlaylists()[i];
            QVector<Track>& tracks = playlist.getTracks();
            for(const QJsonValue& item : root.value("items").toArray()) {
                Track track;
                track.setAddedAt(item.toObject().value("added_at").toString());
                track.setAddedBy(item.toObject().value("added_by").toString());

                const QJsonObject& trackItem = item.toObject().value("track").toObject();
                track.setDiscNumber(trackItem.value("disc_number").toInt());
                track.setDuration(trackItem.value("duration_ms").toInt());
                track.setId(trackItem.value("id").toString());
                track.setName(trackItem.value("name").toString());
                track.setPopularity(trackItem.value("popularity").toDouble());
                track.setPreviewUrl(trackItem.value("preview_url").toString());

                Album album;
                const QJsonObject& albumItem = trackItem.value("album").toObject();
                album.id = albumItem.value("id").toString();
                album.name = albumItem.value("name").toString();
                const QJsonArray& images = albumItem.value("images").toArray();
                if(!images.isEmpty()) {
                    const QJsonObject& firstImage = images[0].toObject(); //only load highest resolution image
                    album.image = new Image(firstImage.value("height").toInt(), firstImage.value("width").toInt(), firstImage.value("url").toString());
                }
                track.setAlbum(album);

                QVector<Artist>& artists = track.getArtists();
                for(const QJsonValue& artistItem : trackItem.value("artists").toArray()) {
                    Artist artist;
                    artist.id = artistItem.toObject().value("id").toString();
                    artist.name = artistItem.toObject().value("name").toString();
                    artists.push_back(artist);
                }

                qDebug() << track.getName() << track.getId() << track.getAlbum().name;

                tracks.push_back(track);
            }

            reply->deleteLater();

            showInterface();
        });
    }
}

void MainWindow::showInterface()
{
    m_spotifyView = new SpotifyView(m_user, this);
    m_gridLayout->addWidget(m_spotifyView, 2, 0);
}
