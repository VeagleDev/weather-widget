/*
* Mysterious Developers © 2022
* Language : C++
* Author : pierrbt
* Name : WeathGet (weather-widget)
*/

#include "widget.hpp"


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

    a.setApplicationName("WeathGet");
    a.setApplicationDisplayName("WeathGet");
    a.setApplicationVersion("2.5");

  QFile * file = new QFile(QDir::currentPath() + "/tools/version.txt");
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
