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

public slots:
  void request_stop();

 signals:
  void finished(QStringList *images);
  void stopped(void);

    
 private:
  bool checkDirs(void);

  QStringList *_images;
  QList<QDir> *_dirs;
  volatile bool stop_requested;
};
   

#endif // PATHSCANNER_H
