#ifndef eMU_DATASERVER_USER_H
#define eMU_DATASERVER_USER_H

#include <vector>
#include "..\core\core.h"

class dataServerUser_t: public eMUCore::socketContext_t {
public:
    dataServerUser_t(int index);

private:
    dataServerUser_t();
    dataServerUser_t(const dataServerUser_t&);
    dataServerUser_t& operator=(const dataServerUser_t&);
};

#endif // eMU_DATASERVER_USER_H