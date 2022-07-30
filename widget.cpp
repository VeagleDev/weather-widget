/*
* Mysterious Developers © 2022
* Language : C++
* Author : pierrbt
* Name : WeathGet (weather-widget)
*/

#include "widget.hpp"


void Window::replyFinished(QNetworkReply *resp){

  QSqlQuery * query = new QSqlQuery(airportsDB);

  bool ok;
  int code = resp->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok);
  if(code == 200 && ok)
  {
      QString buffer = QString(resp->read(20971520));
      QVector<QVector<QString>> infos;
      QVector<QString> line;
      QString cell("");

      if(!query->exec("DELETE FROM metars"))
          qDebug() << "Echec du troncatage";

      for(int i = 0; i != buffer.size(); ++i)
      {
          if(buffer[i] == '\n' || buffer[i] == '\r')
          {
              line.push_back((cell == "" ? "NULL" : cell));
              infos.push_back(line);
              cell.clear();
              line.clear();
          }
          else if(buffer[i] == ',')
          {
              line.push_back((cell == "" ? "NULL" : cell));
              cell.clear();
          }
          else
          {
              cell += buffer[i];
          }
      }

      infos.remove(0,6);

      for(int j = 0; j != infos.size(); ++j)
      {
          if(!query->exec("INSERT INTO metars("
                      "icao, "
                      "raw, "
                      "timest, "
                      "temperature, "
                      "dewpoint, "
                      "winddir, "
                      "windspeed, "
                      "visibility, "
                      "cover) "
                      "VALUES ("
                      "'" + infos[j][1] + "', "
                      "'" + infos[j][0] + "', "
                      "'" + infos[j][2] + "', "
                      + infos[j][5] + ", "
                      + infos[j][6] + ", "
                      + infos[j][7] + ", "
                      + infos[j][8] + ", "
                      + infos[j][10] + ", "
                      "'" + infos[j][22] + "')"))
          {
              qDebug() << "Super erreur" << " - " << query->lastError() << " - " << query->lastQuery();
          }
          else
          {
              hasLoaded = true;
          }
      }

  }
  else
  {
      qDebug() << "Erreur de lecture";
      QMessageBox msgBox;
      msgBox.setText("L'application n'a pas pu télécharger le bilan méteo.");
      msgBox.setInformativeText("Voulez-vous réessayer de le télécharger ?");
      msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Retry);
      msgBox.setDefaultButton(QMessageBox::Retry);
      msgBox.setWindowIcon(QIcon(QDir::currentPath() + "/img/icon.png"));
      int ret = msgBox.exec();
      switch(ret)
      {
      case QMessageBox::Retry:
          manager->get(QNetworkRequest(QUrl("https://aviationweather-cprk.ncep.noaa.gov/adds/dataserver_current/current/metars.cache.csv")));
          break;
      case QMessageBox::Cancel:
          qApp->exit(1);
          break;
      default:
          qApp->exit(2);
          break;
      }
      return;

  }
}


QString Window::lookForICAO(QString nameOfCity)
{

    int msb = QTime::currentTime().msecsSinceStartOfDay();
    QSqlQuery * query = new QSqlQuery(airportsDB);


    if(nameOfCity.size() == 3 or 4)
    {
        if(!query->exec("SELECT icao,iata,name,country,latitude,longitude FROM airports WHERE iata LIKE '" + nameOfCity + "' OR icao LIKE '" + nameOfCity + "' COLLATE NOCASE"))
        {
            qDebug() << "iata/icao error";
            return "";
        }
        else
        {
            if(!query->next())
            {
                if(!query->exec("SELECT icao,iata,name,country,latitude,longitude FROM airports WHERE name LIKE '%" + nameOfCity + "%' OR city LIKE '%" + nameOfCity + "%' COLLATE NOCASE"))
                {
                    qDebug() << "Erreur commande";
                    return "";
                }
                else
                {
                    if(!query->next())
                    {
                                qDebug() << "Rien trouvé";
                                return "";
                    }
                }
            }
        }
    }
    else
    {
        if(!query->exec("SELECT icao,iata,name,country,latitude,longitude FROM airports WHERE name LIKE '%" + nameOfCity + "%' OR city LIKE '%" + nameOfCity + "%' COLLATE NOCASE"))
        {
            qDebug() << "Erreur commande";
            return "";
        }
        if(!query->next())
        {
                    qDebug() << "Rien trouvé";
                    return "";
        }
    }



        airportIATA = query->value(1).toString();
        airportName = query->value(2).toString();
        country = query->value(3).toString();
        latitude = query->value(4).toString();
        longitude = query->value(5).toString();
        qDebug() << "Temps " << (QTime::currentTime().msecsSinceStartOfDay() - msb) << " ms";
        return query->value(0).toString();

}


QStringList Window::findSimilarAirport(QString nameOfTheCity)
{
    QSqlQuery * query = new QSqlQuery(airportsDB);
    if(!query->exec("SELECT airports.name FROM airports WHERE" + ((nameOfTheCity.size() == 3 || nameOfTheCity.size() == 4) ? (" iata LIKE '" + nameOfTheCity + "' OR icao LIKE '" + nameOfTheCity + "' OR ") : " ") + "airports.city LIKE '%" + nameOfTheCity + "%' OR airports.name LIKE '%" + nameOfTheCity + "%' COLLATE NOCASE"))
    {
        qDebug() << "Erreur de similaritude - " << query->lastError() << " - " << query->executedQuery() ;
        return QStringList();
    }
    QStringList cityAirportList;
    while(query->next())
    {
        cityAirportList << query->value(0).toString();
    }
    return cityAirportList;
}

void Window::textRefresh(QString newText)
{
  cityName = newText;
  if(newText.size() > 2)
  {
      QStringList air = findSimilarAirport(newText);
      completer = new QCompleter(air, this);
      //qDebug() << air;
      completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
      completer->setCaseSensitivity(Qt::CaseInsensitive);
      completer->setMaxVisibleItems(10);
      city->setCompleter(completer);
  }
  else
  {
      city->setCompleter(new QCompleter(this));
  }
}

void Window::searchCity()
{
  if(cityName == "" || !hasLoaded)
    return;
  stationCode = lookForICAO(cityName);
  updateInformations();
}

void Window::updateInformations()
{

    QSqlQuery * query = new QSqlQuery(airportsDB);
    if(!query->exec("SELECT airports.icao, "
                    "airports.iata, "
                    "airports.latitude,"
                    "airports.longitude, "
                    "airports.name, "
                    "metars.timest, "
                    "airports.country, "
                    "metars.temperature, "
                    "metars.winddir, "
                    "metars.windspeed, "
                    "metars.visibility, "
                    "metars.cover "

            "FROM airports "
            "LEFT JOIN metars "
            "ON airports.icao = metars.icao "
            "WHERE airports.icao = '" + stationCode + "'"))
    {
        qDebug() << "Erreur de la récup";
        station->setText("Station : N/A");
        date->setText("Date : N/A");
        temp->setText("Température : N/A");
        wind_dir->setText("Direction du vent : N/A");
        wind_speed->setText("Vitesse du vent : N/A");
        visibility->setText("Visibilité : N/A");
        cover->setText("Couverture : N/A");
        return;
    }
    QString m_iata, m_lat, m_lon, m_name, m_ts, m_country, m_temp, m_winddir, m_windspeed, m_visibility, m_cover;
    if(query->next())
    {
        m_iata = (query->value(1).toString() != "NULL" ? query->value(1).toString() : "Inconnu");
        m_lat = query->value(2).toString();
        m_lon = query->value(3).toString();
        m_name = query->value(4).toString();
        m_ts = (query->value(5).toString() == "" ? "Inconnu" : correctTS(query->value(5).toString()));
        m_country = (query->value(6).toString() != "NULL" ? query->value(6).toString() : "Inconnu");
        m_temp = (query->value(7).toString() != "NULL" ? query->value(7).toString() + "°C" : "N/A");
        m_winddir = query->value(8).toString() + "°";
        m_windspeed = QString::number(round(query->value(9).toDouble()*1.852*10)/10) + " km/h";
        m_visibility = (query->value(10).toString() == "6.21") ? "> 10 km" : QString::number((round((query->value(10).toDouble())*1.609344*10))/10) + " km";
        m_cover = query->value(11).toString();

        for(int i = 0; i < 12; i++)
        {
            qDebug() << query->value(i).toString();
        }
    }
    else
    {
        station->setText("Station : N/A");
        date->setText("Date : N/A");
        temp->setText("Température : N/A");
        wind_dir->setText("Direction du vent : N/A");
        wind_speed->setText("Vitesse du vent : N/A");
        visibility->setText("Visibilité : N/A");
        cover->setText("Couverture : N/A");
        qDebug() << "Pas de résultats";
        return;
    }

    station->setText("Station : " + ((m_lat != "NULL" && m_lon != "NULL") ? ("<a href=\"https://www.google.com/maps/place/" + m_lat + "," + m_lon + "\">") : "") + ( m_name == "NULL" ? " Inconnu" : (m_name)) + (country != "NULL" ? ", " + m_country : ", Inconnu") + ( m_iata == "NULL" ? " - Inconnu" : " - " + m_iata ) +  + ((m_lat != "NULL" && m_lon != "NULL") ? "</a>" : ""));
    date->setText("Date : " + m_ts);

    // Si la temperature est connue, ça affiche la température avec °C, sinon ça affiche N/A
    temp->setText("Température : " + m_temp);

    wind_dir->setText("Direction du vent : " + m_winddir);
    wind_speed->setText("Vitesse du vent : " + m_windspeed);
    visibility->setText("Visibilité : " + m_visibility);


    // On créé un dictionnaire qui au code, renvoie la définition
    std::map<QString, QString> skyCover;
    skyCover["SKC"] = "Aucun Nuage (0/8)";
    skyCover["FEW"] = "Quelques Nuages (1/8)";
    skyCover["SCT"] = "Nuages Épars (3/8)";
    skyCover["BKN"] = "Nuages Fragmentés (6/8)";
    skyCover["OVC"] = "Ciel Couvert (8/8)";
    skyCover["NSC"] = "Pas de Nuages Significatifs (1/8)";
    skyCover["NCD"] = "Aucun Nuage (0/8)";
    skyCover["CAVOK"] = "Aucun Nuage, bonne visibilité (0/8)";

    std::map<QString, QPixmap> skyImages;
    skyImages["SKC"] = QPixmap(QDir::currentPath() + "/img/clouds/skc.png", "PNG");
    skyImages["FEW"] = QPixmap(QDir::currentPath() + "/img/clouds/few.png", "PNG");
    skyImages["SCT"] = QPixmap(QDir::currentPath() + "/img/clouds/sct.png", "PNG");
    skyImages["BKN"] = QPixmap(QDir::currentPath() + "/img/clouds/bkn.png", "PNG");
    skyImages["OVC"] = QPixmap(QDir::currentPath() + "/img/clouds/ovc.png", "PNG");
    skyImages["NSC"] = QPixmap(QDir::currentPath() + "/img/clouds/few.png", "PNG");
    skyImages["NCD"] = QPixmap(QDir::currentPath() + "/img/clouds/skc.png", "PNG");
    skyImages["CAVOK"] = QPixmap(QDir::currentPath() + "/img/clouds/skc.png", "PNG");

    // On cherche pour le code donné
    std::map<QString, QString>::iterator it = skyCover.find(m_cover);
    std::map<QString, QPixmap>::iterator it2 = skyImages.find(m_cover);

    if(it != skyCover.end() && it2 != skyImages.end())
    {         // Si c'est trouvé, on met la définition
        cover->setText("Couverture : " + it->second);
        coverImage->setPixmap(it2->second);
    }
    else
    {
        cover->setText("Couverture : " + m_cover); // Si ça ne trouve pas la définition, ça met le code
    }
}
