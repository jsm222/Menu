#ifndef APPLICATIONINFO_H
#define APPLICATIONINFO_H

#include <QObject>

class ApplicationInfo : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationInfo(QObject *parent = nullptr);
    ~ApplicationInfo();
    QString bundlePath(QString path);
    QString bundleName(unsigned long long id);
    QString applicationNiceNameForPath(QString path);
    QString bundlePathForPId(unsigned int pid);
    QString bundlePathForWId(unsigned long long id);
    QString pathForWId(unsigned long long id);
    QString applicationNiceNameForWId(unsigned long long id);


signals:

};

#endif // APPLICATIONINFO_H
