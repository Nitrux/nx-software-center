#include "release.h"

Release::Release(QString id, QString app_id)
{
    this->id = id;
    this->app_id = app_id;
}

Release::~Release()
{

}

bool Release::operator ==(const Release &r) const {
    return !Compare(r);
}

bool Release::operator <(const Release &r) const {
    return version.compare(r.version) < 0;
}

int Release::Compare(const Release &r) const {
    return id.compare(r.id);
}
