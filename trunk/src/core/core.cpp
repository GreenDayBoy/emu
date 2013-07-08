#include <glog/logging.h>
#include <core.hpp>

#ifdef eMU_TARGET
int main(int count, char *args[]) {
  google::InitGoogleLogging(args[0]);
}
#endif
