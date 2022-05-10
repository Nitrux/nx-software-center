#pragma once

#include "../../stores/ResponseDTO/categoryresponsedto.h"
#include "../../stores/ResponseDTO/applicationresponsedto.h"

class StoreManager : public QObject {
    Q_OBJECT

    public:
        StoreManager(QObject *parent = nullptr) : QObject(parent) {}

        virtual void getCategories() = 0;
        virtual void getApplications() = 0;

        Q_SIGNAL void categoriesResponseReady(CategoryResponseDTO *response);
        Q_SIGNAL void applicationsResponseReady(ApplicationResponseDTO *response);
};