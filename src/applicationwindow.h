#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

#include <QString>

QString bundlePathForPId(unsigned int pid);
QString bundlePathForWId(unsigned long long id);
QString pathForWId(unsigned long long id);

QString applicationNiceNameForWId(unsigned long long id);

#endif // APPLICATIONWINDOW_H
