#include <dataserver/database/mySqlInterface.hpp>

#include <glog/logging.h>
#include <boost/algorithm/string/replace.hpp>

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

void MySqlInterface::executeQuery(std::string query)
{
    boost::algorithm::replace_all(query, "'", "\\'");
    boost::algorithm::replace_all(query, "\\", "\\\\");

    if(mysql_real_query(&handle_, query.c_str(), query.size()) == 0)
    {
        LOG(INFO) << "Executed query: " << query;
    }
    else
    {
        LOG(ERROR) << "Execute query failed. Reason: " << this->getErrorMessage() << ", query: " << query;
    }
}

std::string MySqlInterface::getErrorMessage()
{
    return std::move(std::string(mysql_error(&handle_)));
}

QueryResult MySqlInterface::fetchQueryResult()
{
    queryResult_ = mysql_store_result(&handle_);
    QueryResult queryResult;

    if(queryResult_ != nullptr)
    {
        queryResult.getFields() = this->fetchQueryFields();
        queryResult.getRows() = this->fetchQueryRows();

        mysql_free_result(queryResult_);
    }

    return std::move(queryResult);
}

QueryResult::Fields MySqlInterface::fetchQueryFields()
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

QueryResult::Rows MySqlInterface::fetchQueryRows()
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
