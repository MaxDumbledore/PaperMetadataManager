#include "utils.h"

QString listIntToString(const QList<int> &l){
    QString s;
    for(int i:qAsConst(l))
        s+=QString("%1,").arg(i);
    s.chop(1);
    return s;
}

QList<int> stringToListInt(const QString &s){
    auto c=s.split(',');
    QList<int> l;
    for(auto &i:qAsConst(c))
        if(!i.isEmpty())
            l.append(i.toInt());
    return l;
}
