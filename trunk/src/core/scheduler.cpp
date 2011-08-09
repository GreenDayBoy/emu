#include "core.h"

using namespace eMUCore;

scheduler_t::scheduler_t():
    m_synchronizer(NULL) {}

void scheduler_t::insert(schedule_e::type_t type, const boost::function0<void> &callback, time_t delay) {
    schedule_t schedule = {callback, type, delay, 0};
    m_list.push_back(schedule);
}

void scheduler_t::worker() {
    for(std::vector<schedule_t>::iterator i = m_list.begin(); i != m_list.end(); ++i) {
        if((GetTickCount() - i->m_lastExecuteTime) >= (i->m_delay * 1000)) {
            if(i->m_type == schedule_e::_synchronized) {
                m_synchronizer->lock();
                i->m_callback();
                m_synchronizer->unlock();
            } else {
                i->m_callback();
            }

            i->m_lastExecuteTime = GetTickCount();
        }
    }
}