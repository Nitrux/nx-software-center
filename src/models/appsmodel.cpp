#include "appsmodel.h"

#include <QDebug>

AppsModel::AppsModel(QObject *parent) : MauiList(parent) {}

void AppsModel::componentComplete() {}

FMH::MODEL_LIST AppsModel::items() const { return this->m_list; }

