/*
* Mysterious Developers © 2022
* Language : C++
* Author : pierrbt
* Name : WeathGet (weather-widget)
* Website : mysteriousdev.fr
*/

#include "widget.hpp"
#include <fstream>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QFile * file = new QFile(QDir::currentPath() + "/version.txt");
  QString version;
  if(file->open(QFile::ReadOnly))
  {
      version = QString(file->readAll());
  }
  else
  {
      version = "N/A";
  }

   file->close();
   delete file;


  Window * w = new Window(" - ", version);
  w->show();
  return a.exec();
}
