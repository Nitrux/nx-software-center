#include "ApplicationORM.h"

#include "../Application.h"

ApplicationORM::ApplicationORM(const QSqlDatabase &database)
    : _database(database)
{
}
