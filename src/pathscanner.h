#ifndef PATHSCANNER_H
#define PATHSCANNER_H

#include <QObject>
#include <QtDebug>
#include <QStringList>
#include <QThread>
#include <QDir>
#include <QDirIterator>

class PathScanner : public QThread
{
  Q_OBJECT

 public:
  PathScanner(QObject *parent = 0);
  //~PathScanner();

  void setPaths(QList<QDir> *dirs);
  void run();

 signals:
  void finished(QStringList *images);
    
 private:
  bool checkDirs(void);

  QStringList *_images;
  QList<QDir> *_dirs;
};
   

#endif // PATHSCANNER_H
