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

      /*
      station->setText("Station : " + ((latitude != "" && longitude != "") ? ("<a href=\"https://www.google.com/maps/place/" + latitude + "," + longitude + "\">") : "") + ( airportName == "" ? " Inconnu" : (airportName)) + (country != "" ? ", " + country : ", Inconnu") + ( airportIATA == "" ? " - Inconnu" : " - " + airportIATA ) +  + ((latitude != "" && longitude != "") ? "</a>" : ""));
      date->setText("Date : " + correctTS(infos[2]));

      // Si la temperature est connue, ça affiche la température avec °C, sinon ça affiche N/A
      temp->setText("Température : " + ((infos[5] != "Inconnu") ? (infos[5] + "°C") : ("N/A")));

      wind_dir->setText("Direction du vent : " + infos[7] + "°");
      wind_speed->setText("Vitesse du vent : " + QString::number(round(infos[8].toDouble()*1.852*10)/10) + " km/h");
      visibility->setText("Visibilité : " + ((infos[10] == "6.21") ? "> 10" : QString::number((round((infos[10].toDouble())*1.609344*10))/10)) + " km");


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
      std::map<QString, QString>::iterator it = skyCover.find(infos[22]);
      std::map<QString, QPixmap>::iterator it2 = skyImages.find(infos[22]);

      if(it != skyCover.end() && it2 != skyImages.end())
      {         // Si c'est trouvé, on met la définition
          cover->setText("Couverture : " + it->second);
          coverImage->setPixmap(it2->second.scaled(35,28));
      }
      else
      {
          cover->setText("Couverture : " + infos[22]); // Si ça ne trouve pas la définition, ça met le code
      }*/

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

/*int i(0), j(0), k(0), l(0), w(0);
QString toAdd(""), lfrs(""), name("");
QVector<QString> lines;
infos.clear();
names.clear();
QByteArray buffer = resp->read(20971520);
while(i < buffer.size())
  {
    if(QString(buffer[i]) == "\n")
      {
        lines.push_back(toAdd);
        toAdd = "";
      }
    else
        toAdd += QString(buffer[i]);
    i++;
  }

for(QString k : lines)
  {
    if(k.startsWith(stationCode, Qt::CaseInsensitive))
      {
        lfrs += k;
      }
    if(k.startsWith("raw_text", Qt::CaseInsensitive))
        name += k;
  }
 if(lfrs == "")
  {
    station->setText("Station : N/A");
    date->setText("Date : N/A");
    temp->setText("Température : N/A");
    wind_dir->setText("Direction du vent : N/A");
    wind_speed->setText("Vitesse du vent : N/A");
    visibility->setText("Visibilité : N/A");
    cover->setText("Couverture : N/A");
    return;
  }
toAdd = "";
while(w < name.length())
  {
    if(name[w] == ',')
      {
        names.push_back(toAdd);
        toAdd = "";
      }
    else
        toAdd += name[w];
    w++;
  }
names.push_back(toAdd);
toAdd = "";
while(j < lfrs.length())
  {
    if(lfrs[j] == ',')
      {
        if(toAdd == "")
          toAdd = "Inconnu";
        infos.push_back(toAdd);
        toAdd = "";
      }
    else
        toAdd += lfrs[j];
    j++;
  }
if(toAdd == "")
  toAdd = "Inconnu";
infos.push_back(toAdd);
toAdd = "";*/





QString Window::lookForICAO(QString nameOfCity)
{
    int msb = QTime::currentTime().msecsSinceStartOfDay();
    QSqlQuery * query = new QSqlQuery(airportsDB);
    qDebug() << query->isActive() << query->isValid();
    if(!query->exec("SELECT icao,iata,name,country,latitude,longitude FROM airports WHERE name LIKE '%" + nameOfCity + "%' OR city LIKE '%" + nameOfCity + "%'"))
    {
        qDebug() << "Erreur commande";
        return "";
    }
    if(query->next())
    {
        airportIATA = query->value(1).toString();
        airportName = query->value(2).toString();
        country = query->value(3).toString();
        latitude = query->value(4).toString();
        longitude = query->value(5).toString();
        qDebug() << "Temps " << (QTime::currentTime().msecsSinceStartOfDay() - msb) << " ms";
        return query->value(0).toString();
    }
    else
    {
        qDebug() << "Rien trouvé";
        return "";
    }
}


QStringList Window::findSimilarAirport(QString nameOfTheCity)
{
    QSqlQuery * query = new QSqlQuery(airportsDB);
    if(!query->exec("SELECT airports.name FROM airports WHERE airports.city LIKE '%" + nameOfTheCity + "%' OR airports.name LIKE '%" + nameOfTheCity + "%' COLLATE NOCASE"))
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
      qDebug() << air;
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
        coverImage->setPixmap(it2->second.scaled(35,28));
    }
    else
    {
        cover->setText("Couverture : " + m_cover); // Si ça ne trouve pas la définition, ça met le code
    }
}










/*
QTime before = QTime::currentTime();
int msb = before.msecsSinceStartOfDay();
QFile * airports = new QFile(QDir::currentPath() + "/tools/airports.csv");
QVector<QString> lines, infos;
QString toAdd(""), icao(""), iata(""), name("");

airports->open(QIODeviceBase::ReadOnly);
QString content = QString::fromUtf8(((airports->readAll())));
airports->close();

for(int i = 0; i < content.size(); i++)
{
  if(content[i] == '\n')
    {
       lines.push_back(toAdd);
       toAdd = "";
    }
  else
    {
      toAdd += content[i];
    }
}
toAdd = "";
for(int j = 0; j < lines.size(); j++)
{
  if((lines[j].contains(nameOfCity, Qt::CaseInsensitive) && nameOfCity.length() > 3) || (lines[j].contains(nameOfCity.toUpper(), Qt::CaseSensitive) && nameOfCity.length() == 3))
    {
      for(int k = 0; k < lines[j].size(); k++)
        {
          if(lines[j][k] == ';' || lines[j][k] == '\0' || lines[j][k] == '\n' || lines[j][k] == '\r')
            {
              infos.push_back(toAdd);
              toAdd = "";
            }
          else
            {
              toAdd += lines[j][k];
            }
        }
    }
}
if(infos.size() < 9)
{
  qDebug() << "Infos non collectée (" << infos.size() << ")";
  return "Error";
}
else if(infos.size() > 9)


{
  qDebug() << infos;
  int factor = infos.size() / 10;

  bool hasFound = false;

  if(!hasFound)
    {
      icao = infos[0];
      iata = infos[1];
      name = infos[2];
    }

  qDebug() << "Aéroport de " << nameOfCity << " (" << name << ") - ICAO : " << icao << " - IATA : " << iata;
  country = infos[7];
  latitude = infos[3];
  longitude = infos[4];
  airportIATA = iata;
  airportName = name;



  QTime after = QTime::currentTime();
  int msa = after.msecsSinceStartOfDay();
  qDebug() << "ms pour trouver ICAO : " << QString::number(msa - msb);
  return icao;
}
else
{

  icao = infos[0];
  iata = infos[1];
  name = infos[2];
  country = infos[7];
  latitude = infos[3];
  longitude = infos[4];
  qDebug() << "Aéroport de " << nameOfCity << " (" << name << ") - ICAO : " << icao << " - IATA : " << iata;
  airportIATA = iata;
  airportName = name;
  }
  cityList.clear();
  cityList << infos[2];
  return icao;*/


/*QFile * airports = new QFile(QDir::currentPath() + "/tools/airports.csv");
QVector<QString> lines, infos;
QString toAdd("");
QStringList cityAirportList;
airports->open(QIODeviceBase::ReadOnly);
QString content = QString::fromUtf8(((airports->readAll())));
airports->close();

for(int i = 0; i < content.size(); i++)
  {
    if(content[i] == '\n')
      {
         lines.push_back(toAdd);
         toAdd = "";
      }
    else
      {
        toAdd += content[i];
      }
  }

toAdd = "";

for(int j = 0; j < lines.size(); j++)
  {

    if((lines[j].contains(nameOfTheCity, Qt::CaseInsensitive) && nameOfTheCity.length() > 3) || (nameOfTheCity.length() == 3 && lines[j].contains(nameOfTheCity.toUpper(), Qt::CaseSensitive)))
      {
        for(int k = 0; k < lines[j].size(); k++)
          {
            if(lines[j][k] == ';' || lines[j][k] == '\0' || lines[j][k] == '\n' || lines[j][k] == '\r')
              {
                infos.push_back(toAdd);
                toAdd = "";
              }
            else
              {
                toAdd += lines[j][k];
              }
          }
      }
  }
if(infos.size() < 9)
  {
    qDebug() << "Infos non collectée (" << infos.size() << ")";
  }
else if(infos.size() > 9)
  {
    for(int g = 2; g < infos.size(); g += 10)
    {
        cityAirportList << infos[g];
    }
  }
else
  {
        cityAirportList << infos[2];
  }*/
