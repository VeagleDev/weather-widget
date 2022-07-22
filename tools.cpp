#include "widget.hpp"

QString Window::correctTS(QString input)
{
      // Récupération des valeurs dans le timestamp
      input = input.remove(input.size()-4, 4);
      QString year = input.sliced(0, 4);
      QString month = input.sliced(5,2);
      QString day = input.sliced(8,2);
      input = input.remove(0, 11);
      int hour = input.sliced(0,2).toInt();
      int min = input.sliced(3,2).toInt();

      bool yesterday = false;

      //Création de la date
      QDate *messDate = new QDate;
      messDate->setDate(year.toInt(), month.toInt(), day.toInt());


        // On regarde si la date est celle d'aujourd'hui ou d'hier
      if(messDate->toString() == QDate::currentDate().toString())
          input = "Aujourd'hui à ";
      else if((messDate->year() == QDate::currentDate().year())   &&
              (messDate->month() == QDate::currentDate().month()) &&
              (messDate->day() == ((QDate::currentDate().day())-1)))
      {
                  input = "Hier à ";
                  yesterday = true;
      }

      else
           input = "Le " + day + "/" + month + "/" + year + " à ";

      // Si on obtient une heure GMT+2 >= 24 heures, on enlève 22 pour retrouver la vraie heure
      if(hour + 2 >= 24)
       {
           hour-=22;
           if(yesterday)
               input = "Aujourd'hui à ";
       }
      else // Sinon on ajoute 2 heures pour avoir l'heure française et pas GMT
      {
          hour+=2;
      }
      // Commme c'est des nombres, ça s'affiche 1 si il est une heure alors qu'on veut 01h, on ajoute donc un 0 si c'est < à 10
       QString time = ((hour < 10) ? ("0"+QString::number(hour)) : (QString::number(hour))) + ":" + ((min < 10) ? ("0" + QString::number(min)) : (QString::number(min)));
      return input + time;
}
