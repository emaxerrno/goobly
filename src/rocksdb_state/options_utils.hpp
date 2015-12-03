#pragma once
#include <rocksdb/db.h>
namespace goobly {
::rocksdb::Options dboptsFactory();

::rocksdb::WriteOptions woptsFactory();

::rocksdb::ReadOptions roptsFactory(bool tailing = true);
}
