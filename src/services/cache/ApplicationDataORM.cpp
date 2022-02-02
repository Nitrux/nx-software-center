#include "ApplicationDataORM.h"

ApplicationDataORM::ApplicationDataORM(const QSqlDatabase &database)
    : _database(database)
{
}
