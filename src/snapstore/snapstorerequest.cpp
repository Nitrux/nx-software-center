#include "snapstorerequest.h"


SnapStoreRequest::SnapStoreRequest(const QString &storeUrl, QObject *parent) : QObject(parent), m_storeUrl(storeUrl)
{
}

QString SnapStoreRequest::response() const
{
    return m_response;
}

QString SnapStoreRequest::errorString() const
{
    return m_errorString;
}

QString SnapStoreRequest::storeUrl() const
{
    return m_storeUrl;
}
