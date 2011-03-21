#ifndef eMU_CONNECTSERVER_USER_H
#define eMU_CONNECTSERVER_USER_H

#include "..\core\core.h"

class connectServerUser_t: public eMUCore::socketContext_t {
public:
	connectServerUser_t(int index);

private:
	connectServerUser_t();
	connectServerUser_t(const connectServerUser_t&);
	connectServerUser_t& operator=(const connectServerUser_t&);
};

#endif // eMU_CONNECTSERVER_USER_H