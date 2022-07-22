/*
* Mysterious Developers © 2022
* Language : C++
* Author : pierrbt
* Name : WeathGet (weather-widget)
*/

#include "widget.hpp"


void Window::replyFinished(QNetworkReply *resp){
 QTime before = QTime::currentTime();
 int msb = before.msecsSinceStartOfDay();
  bool ok;
  bool test;
  int code = resp->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&ok);
  if(code == 200 && ok)
  {
      int i(0), j(0), k(0), l(0), w(0);
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
      toAdd = "";

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

      // On cherche pour le code donné
      std::map<QString, QString>::iterator it = skyCover.find(infos[22]);
      if(it != skyCover.end()) // Si c'est trouvé, on met la définition
          cover->setText("Couverture : " + it->second);
      else
          cover->setText("Couverture : " + infos[22]); // Si ça ne trouve pas la définition, ça met le code

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
  QTime before = QTime::currentTime();
  int msb = before.msecsSinceStartOfDay();
  QFile * airports = new QFile(QDir::currentPath() + "/airports.csv");
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
      return icao;
}


QStringList findSimilarAirport(QString nameOfTheCity)
{
    QFile * airports = new QFile(QDir::currentPath() + "/airports.csv");
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
  if(cityName == "")
    return;
  stationCode = lookForICAO(cityName);
  getNewInfos();
}

void Window::getNewInfos()
{
  manager->get(QNetworkRequest(QUrl("https://aviationweather-cprk.ncep.noaa.gov/adds/dataserver_current/current/metars.cache.csv")));
}
