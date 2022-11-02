#ifndef APPLICATIONINFO_H
#define APPLICATIONINFO_H

#include <QString>



class ApplicationInfo
{

public:
    explicit ApplicationInfo();
    ~ApplicationInfo();
    QString bundlePath(QString path);
    QString bundleName(unsigned long long id);
    QString applicationNiceNameForPath(QString path);
    QString bundlePathForPId(unsigned int pid);
    QString bundlePathForWId(unsigned long long id);
    QString pathForWId(unsigned long long id);
    QString applicationNiceNameForWId(unsigned long long id);


};

#endif // APPLICATIONINFO_H
