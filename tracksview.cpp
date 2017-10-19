#include "tracksview.h"
#include <QGridLayout>
#include <QLabel>
#include <QTableWidget>
#include "playlist.h"
#include "track.h"
#include <QDesktopServices>
#include <QPushButton>
#include <QUrl>
#include <QPlainTextEdit>
#include <mainwindow.h>
#include <QOAuth2AuthorizationCodeFlow>
#include <QDebug>
#include <QLineEdit>

TracksView::TracksView(User user, QWidget *parent) :
    m_user(user), QScrollArea(parent)
{
    setWidgetResizable(true);
    setFrameShape(QFrame::NoFrame);

    QWidget *proxyWidget = new QWidget;
    setWidget(proxyWidget);

    QVBoxLayout *verticalLayout = new QVBoxLayout;
    proxyWidget->setLayout(verticalLayout);

    QGridLayout *gridLayout = new QGridLayout;
    verticalLayout->addLayout(gridLayout);

    verticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    gridLayout->addWidget(new QLabel("Tracks"), 0, 0);

    m_localTracks = new QTableWidget(0, 6);
    gridLayout->addWidget(m_localTracks, 1, 0);
    m_plainText = new QPlainTextEdit();
//    gridLayout->addWidget(m_plainText, 5, 0);
    m_localTracks->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_localTracks->setHorizontalHeaderLabels({"Name", "Duration", "Popularity", "Added at", "Playlist", "Preview"});

    int row = 0;
    for(Playlist playlist : m_user.getPlaylists()) {
        for(Track track : playlist.getTracks()) {
            int col = 0;

            m_localTracks->insertRow(row);
            QTableWidgetItem *nameItem = new QTableWidgetItem(track.getName());
            nameItem->setData(Qt::UserRole, track.getId());
            nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
            m_localTracks->setItem(row, col++, nameItem);

            QTableWidgetItem *durationItem = new QTableWidgetItem(QString::number(track.getDuration()));
            durationItem->setFlags(durationItem->flags() & ~Qt::ItemIsEditable);
            m_localTracks->setItem(row, col++, durationItem);

            QTableWidgetItem *popularityItem = new QTableWidgetItem(QString::number(track.getPopularity()));
            popularityItem->setFlags(popularityItem->flags() & ~Qt::ItemIsEditable);
            m_localTracks->setItem(row, col++, popularityItem);

            QTableWidgetItem *addedAtItem = new QTableWidgetItem(track.getAddedAt());
            addedAtItem->setFlags(addedAtItem->flags() & ~Qt::ItemIsEditable);
            m_localTracks->setItem(row, col++, addedAtItem);

            QTableWidgetItem *playlistItem = new QTableWidgetItem(playlist.getName());
            playlistItem->setFlags(playlistItem->flags() & ~Qt::ItemIsEditable);
            m_localTracks->setItem(row, col++, playlistItem);

            QWidget* widget = new QWidget();
            QPushButton* playButton = new QPushButton("Play");
            QString previewUrl = track.getPreviewUrl();
            connect(playButton, &QPushButton::clicked, [previewUrl](){
                QDesktopServices::openUrl(QUrl(previewUrl));
            });
            QHBoxLayout* pLayout = new QHBoxLayout(widget);
            pLayout->addWidget(playButton);
            pLayout->setAlignment(Qt::AlignCenter);
            pLayout->setContentsMargins(0, 0, 0, 0);
            widget->setLayout(pLayout);
            m_localTracks->setCellWidget(row, col++, widget);
        }
    }

    gridLayout->addWidget(new QLabel("Procure por novas músicas:"), 2, 0);
    m_searchForTracks = new QLineEdit();
    connect(m_searchForTracks, &QLineEdit::textChanged, [this](QString text) {
        searchTracks(text);
    });
    gridLayout->addWidget(m_searchForTracks, 3, 0);
    m_searchedTracks = new QTableWidget(0, 4);
    m_searchedTracks->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_searchedTracks->setHorizontalHeaderLabels({"Name", "Duration", "Popularity", "Preview"});
    gridLayout->addWidget(m_searchedTracks, 4, 0);
}

void TracksView::searchTracks(const QString &trackName)
{
    qDebug() << trackName;
    QUrl u("https://api.spotify.com/v1/search?q=" + trackName + "&type=track");
    auto reply = mainWindow->spotify.get(u);

    connect (reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
            return;
        }

        const auto data = reply->readAll();
        m_plainText->appendPlainText(data);

        const auto document = QJsonDocument::fromJson(data);
        const auto root = document.object();

        QVector<Track> tracks;
        for(const QJsonValue& item : root.value("tracks").toObject().value("items").toArray()) {
            Track track;
            track.setAddedAt(item.toObject().value("added_at").toString());
            track.setAddedBy(item.toObject().value("added_by").toString());

            const QJsonObject& trackItem = item.toObject();
            track.setDiscNumber(trackItem.value("disc_number").toInt());
            track.setDuration(trackItem.value("duration_ms").toInt());
            track.setId(trackItem.value("id").toString());
            track.setName(trackItem.value("name").toString());
            track.setPopularity(trackItem.value("popularity").toDouble());
            track.setPreviewUrl(trackItem.value("preview_url").toString());

            QVector<Artist>& artists = track.getArtists();
            for(const QJsonValue& artistItem : trackItem.value("artists").toArray()) {
                Artist artist;
                artist.id = artistItem.toObject().value("id").toString();
                artist.name = artistItem.toObject().value("name").toString();
                artists.push_back(artist);
            }

            tracks.push_back(track);
        }

        int row = 0;
        m_searchedTracks->setSortingEnabled(false);
        m_searchedTracks->clearContents();
        m_searchedTracks->setRowCount(0);
        for(Track track : tracks) {
            int col = 0;
            m_searchedTracks->insertRow(row);
            QTableWidgetItem *nameItem = new QTableWidgetItem(track.getName());
            nameItem->setData(Qt::UserRole, track.getId());
            nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
            m_searchedTracks->setItem(row, col++, nameItem);

            QTableWidgetItem *durationItem = new QTableWidgetItem(QString::number(track.getDuration()));
            durationItem->setFlags(durationItem->flags() & ~Qt::ItemIsEditable);
            m_searchedTracks->setItem(row, col++, durationItem);

            QTableWidgetItem *popularityItem = new QTableWidgetItem(QString::number(track.getPopularity()));
            popularityItem->setFlags(popularityItem->flags() & ~Qt::ItemIsEditable);
            m_searchedTracks->setItem(row, col++, popularityItem);

            QWidget* widget = new QWidget();
            QPushButton* playButton = new QPushButton("Play");
            QString previewUrl = track.getPreviewUrl();
            connect(playButton, &QPushButton::clicked, [previewUrl](){
                QDesktopServices::openUrl(QUrl(previewUrl));
            });
            QHBoxLayout* pLayout = new QHBoxLayout(widget);
            pLayout->addWidget(playButton);
            pLayout->setAlignment(Qt::AlignCenter);
            pLayout->setContentsMargins(0, 0, 0, 0);
            widget->setLayout(pLayout);
            m_searchedTracks->setCellWidget(row, col++, widget);
        }
        m_searchedTracks->setSortingEnabled(true);
//        m_searchedTracks->resizeColumnsToContents();
    });
}

