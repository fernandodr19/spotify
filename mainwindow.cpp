#include "mainwindow.h"

#include <QDebug>
#include <QtNetworkAuth>
#include <QJsonDocument>
#include <QJsonObject>
#include <QGridLayout>
#include <QLabel>
#include <QPlainTextEdit>

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

    m_gridLayout->addWidget(new QLabel("Spotify"),0,0);
    m_gridLayout->addWidget(m_plainText, 1, 0);

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

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
            return;
        }
        const auto data = reply->readAll();
        m_plainText->appendPlainText(data);

        const auto document = QJsonDocument::fromJson(data);
        const auto root = document.object();
        m_user.id = root.value("id").toString();
        m_user.country = root.value("country").toString();

        m_plainText->appendPlainText("Username: " + m_user.id + " from " + m_user.country);

        reply->deleteLater();
        getPlayLists();
    });
}

void MainWindow::getPlayLists()
{
    if (m_user.id.length() == 0) return;

    QUrl u("https://api.spotify.com/v1/users/" + m_user.id + "/playlists");

    auto reply = spotify.get(u);

    connect (reply, &QNetworkReply::finished, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
            return;
        }

        const auto data = reply->readAll();

        const auto document = QJsonDocument::fromJson(data);
        const auto root = document.object();

        m_plainText->appendPlainText(data);

        for(const QJsonValue& item : root.value("items").toArray()) {
            Playlist playList;
            playList.setId(item.toObject().value("id").toString());
            playList.setName(item.toObject().value("name").toString());
            m_playlists.push_back(playList);
        }

        for(Playlist p : m_playlists)
            qDebug() << p.getName() << p.getId();


        reply->deleteLater();
    });

}
