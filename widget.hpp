/*
* Mysterious Developers © 2022
* Language : C++
* Author : pierrbt
* Name : WeathGet (weather-widget)
*/

#ifndef WIDGET_H
#define WIDGET_H

#include "includes.hpp"

class Window : public QWidget
{
  Q_OBJECT

public:
        Window(QString val = " - ", QString version = "N/A");
public slots:
        void replyFinished(QNetworkReply *resp);
        void updateReplyFinished(QNetworkReply *resp);
        void getNewInfos();
        void searchCity();
        void textRefresh(QString newText);
        void seeMoreInformations();
        void displayLicense();
        void updateInformations();
        void goForUpdate(float newVersion);
        QString lookForICAO(QString nameOfCity);
        QStringList findSimilarAirport(QString nameOfTheCity);
        QString correctTS(QString input);
        void setFavorityCity(QString city);
private:

        QSystemTrayIcon *trayIcon;

        QComboBox * menu = new QComboBox;

        QLabel * value;
        QLabel * name;
        QLabel * icon = new QLabel;
        QLabel  * station = new QLabel("Station : - ");
        QLabel  * date = new QLabel("Date : - ");
        QLabel  * temp = new QLabel("Température : -");
        QLabel  * wind_dir = new QLabel("Direction du vent : - ");
        QLabel  * wind_speed = new QLabel("Vitesse du vent : - ");
        QLabel  * visibility = new QLabel("Visibilité : - ");
        QLabel  * cover = new QLabel("Couverture : - ");
        QLabel * coverImage = new QLabel;
        QLineEdit * city = new QLineEdit;
        QLabel * c_label = new QLabel("Ville : ");
        QCompleter * completer;

        QVector<QString> infos;
        QVector<QString> names;

        QString airportName = "";
        QString airportIATA = "";
        QString latitude = "";
        QString longitude = "";
        QString country = "";
        QString cityName = "Nantes";
        QString stationCode = "LFRS";
        QString appVersion = "N/A";

        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QNetworkAccessManager *updateManager = new QNetworkAccessManager(this);

        QStringList cityList;

        QSqlDatabase airportsDB = QSqlDatabase::addDatabase("QSQLITE");

        bool hasLoaded = false;
};




#endif // WIDGET_H
