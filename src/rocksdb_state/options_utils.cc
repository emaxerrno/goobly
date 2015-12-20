#include "rocksdb_state/options_utils.hpp"

namespace goobly {

// https://github.com/mongodb-partners/mongo-rocks/blob/master/src/rocks_engine.cpp#L502
// These options were mostly taken from the rocks_engine.cpp from the mongodb
// backing store by facebook
::rocksdb::Options dboptsFactory() {
  rocksdb::Options options;
  // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
  // does starts a thread per core
  options.IncreaseParallelism();
  options.rate_limiter = rocksdb::NewGenericRateLimiter(
    static_cast<int64_t>(_maxWriteMBPerSec) * 1024 * 1024);
  rocksdb::BlockBasedTableOptions table_options;
  table_options.block_cache =
    rocksdb::NewLRUCache(cacheSizeGB * 1024 * 1024 * 1024LL, 6);
  table_options.filter_policy.reset(rocksdb::NewBloomFilterPolicy(10, false));
  table_options.block_size = 16 * 1024; // 16KB
  table_options.format_version = 2;
  options.table_factory.reset(
    rocksdb::NewBlockBasedTableFactory(table_options));

  options.write_buffer_size = 64 * 1024 * 1024; // 64MB
  options.level0_slowdown_writes_trigger = 8;
  options.max_write_buffer_number = 4;
  options.max_background_compactions = 8;
  options.max_background_flushes = 2;
  options.target_file_size_base = 64 * 1024 * 1024; // 64MB
  options.soft_rate_limit = 2.5;
  options.hard_rate_limit = 3;
  options.level_compaction_dynamic_level_bytes = true;
  options.max_bytes_for_level_base = 512 * 1024 * 1024; // 512 MB
  // This means there is no limit on open files. Make sure to always set ulimit
  // so that it can
  // keep all RocksDB files opened.
  options.max_open_files = -1;
  options.optimize_filters_for_hits = true;

  // FIXME(agallego)
  // Look into the prefixdeletingcompactionfilterfactory from the
  // rocks_engine.cpp form rocks-mongodb
  // options.compaction_filter_factory.reset(
  //   new PrefixDeletingCompactionFilterFactory(this));

  options.enable_thread_tracking = true;

  options.compression_per_level.resize(3);
  options.compression_per_level[0] = rocksdb::kNoCompression;
  options.compression_per_level[1] = rocksdb::kNoCompression;
  options.compression_per_level[2] = rocksdb::kSnappyCompression;
  // create the DB if it's not already present
  options.create_if_missing = true;
  // allow override
  return options;
}

::rocksdb::WriteOptions woptsFactory() {
  rocksdb::WriteOptions o{};
  // Note: If you don't end up using rocksdb for the index, then definitely
  // set this to false as you already have the write-ahead-log entry synced
  // on disk
  o.sync = true;
  return o;
}

::rocksdb::ReadOptions roptsFactory(bool tailing) {
  rocksdb::ReadOptions o{};
  /*if true use read-only tailing iterators*/
  o.tailing = tailing;
  o.verify_checksums = false;
  return o;
}
}
