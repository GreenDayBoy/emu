#include <dataserver/database/mySqlInterface.hpp>

namespace eMU
{
namespace dataserver
{
namespace database
{

MySqlInterface::MySqlInterface():
    queryResult_(nullptr) {}

bool MySqlInterface::initialize()
{
    if(mysql_init(&handle_) != nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool MySqlInterface::connect(const std::string &hostname, uint16_t port,
                         const std::string &userName, const std::string &password, const std::string &databaseName)
{
    if(mysql_real_connect(&handle_, hostname.c_str(), userName.c_str(), password.c_str(),
                          databaseName.c_str(), port, 0, CLIENT_MULTI_RESULTS | CLIENT_MULTI_STATEMENTS) != nullptr)
    {
        bool reconnect = true;
        mysql_options(&handle_, MYSQL_OPT_RECONNECT, &reconnect);

        return true;
    }
    else
    {
        return false;
    }
}

void MySqlInterface::cleanup()
{
    mysql_close(&handle_);
}

bool MySqlInterface::executeQuery(const std::string &query)
{
    if(mysql_real_query(&handle_, query.c_str(), query.size()) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::string MySqlInterface::getErrorMessage()
{
    return mysql_error(&handle_);
}

const QueryResult&& MySqlInterface::fetchQuery()
{
    queryResult_ = mysql_store_result(&handle_);

    if(queryResult_ == nullptr)
    {
        return std::move(QueryResult());
    }

    QueryResult queryResult;
    queryResult.getFields() = this->fetchQueryFields();
    queryResult.getRows() = this->fetchQueryRows();

    mysql_free_result(queryResult_);

    return std::move(queryResult);
}

QueryResult::Fields&& MySqlInterface::fetchQueryFields()
{
    MYSQL_FIELD *field = mysql_fetch_field(queryResult_);
    size_t i = 0;

    QueryResult::Fields fields;

    while(field != nullptr)
    {
        fields[field->name] = i++;
        field = mysql_fetch_field(queryResult_);
    }

    return std::move(fields);
}

QueryResult::Rows&& MySqlInterface::fetchQueryRows()
{
    MYSQL_ROW mysqlRow = mysql_fetch_row(queryResult_);
    size_t fieldsCount = mysql_num_fields(queryResult_);

    QueryResult::Rows rows;

    while(mysqlRow != nullptr)
    {
        QueryResult::Row row;

        for(size_t i = 0; i < fieldsCount; ++i)
        {
            row.push_back(mysqlRow[i]);
        }

        mysqlRow = mysql_fetch_row(queryResult_);

        rows.push_back(row);
    }

    return std::move(rows);
}

}
}
}
