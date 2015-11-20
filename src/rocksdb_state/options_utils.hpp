#pragma once
#include <rocksdb/db.h>
namespace goobly {
inline ::rocksdb::Options dboptsFactory() {
  rocksdb::Options o{};
  // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
  // does starts a thread per core
  o.IncreaseParallelism();
  // For big databases, this is the best style compaction. Universal
  // Compaction has issues if your DB is > 150 GB.  See docs.
  // This option also sets compression after the first 2 levels
  o.OptimizeLevelStyleCompaction();
  // rocksdb mailing list optimization for queues
  // https://www.facebook.com/groups/rocksdb.dev
  o.max_open_files = -1;
  o.create_if_missing = true;
  o.create_missing_column_families = true;
  o.allow_mmap_reads = true;
  o.allow_mmap_writes = true;
  o.access_hint_on_compaction_start = rocksdb::DBOptions::SEQUENTIAL;
  rocksdb::BlockBasedTableOptions table_options;
  table_options.filter_policy.reset(rocksdb::NewBloomFilterPolicy(10, false));
  o.table_factory.reset(rocksdb::NewBlockBasedTableFactory(table_options));
  return o;
}

inline ::rocksdb::WriteOptions woptsFactory() {
  rocksdb::WriteOptions o{};
  // Note: If you don't end up using rocksdb for the index, then definitely
  // set this to false as you already have the write-ahead-log entry synced
  // on disk
  o.sync = true;
  return o;
}

inline ::rocksdb::ReadOptions roptsFactory(bool tailing=true) {
  rocksdb::ReadOptions o{};
  /*if true use read-only tailing iterators*/
  o.tailing = tailing;
  o.verify_checksums = false;
  return o;
}

}
