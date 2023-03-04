#ifndef APPLICATIONINFO_H
#define APPLICATIONINFO_H

#include <QString>
#include <QWindow>

/*
 * https://en.wikipedia.org/wiki/Rule_of_three_(computer_programming)
 * Currently being used in:
 * Menu (master)
 * launch (copy)
 */

class ApplicationInfo
{

public:
    explicit ApplicationInfo();
    ~ApplicationInfo();
    QString bundlePath(QString path);
    QString applicationNiceNameForPath(QString path);
    QString bundlePathForPId(unsigned int pid);
    const QString bundlePathForWId(unsigned long long winId);
    QString pathForWId(unsigned long long id);
    QString applicationNiceNameForWId(unsigned long long id);
    void setWindowProperty(WId winId, const QByteArray name, const QByteArray value);
    const QString getWindowProperty(WId id, const QByteArray &name);
    QString environmentVariableForPId(unsigned int pid, const QByteArray &environmentVariable);
    const QString environmentVariableForWId(unsigned long long winId,
                                            const QByteArray &environmentVariable);
};

#endif // APPLICATIONINFO_H
