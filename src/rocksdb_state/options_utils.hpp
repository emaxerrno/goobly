#pragma once

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
  o.setMaxOpenFiles(-1);
  o.create_if_missing = true;
  o.create_missing_column_families = true;
  o.allow_mmap_reads = true;
  o.allow_mmap_writes = true;
  o.access_hint_on_compaction_start = rocksdb::DBOptions::SEQUENTIAL;
  // TODO(add block table config & use bloomfilter)
  // final BlockBasedTableConfig tableOptions = new BlockBasedTableConfig();
  // tableOptions.setFilter(new BloomFilter(10, false));
  // options.setTableFormatConfig(tableOptions);
  // Please make sure you set up BloomFilter with parameter useBlockBasedMode=false
  return o;
}

inline ::rocksdb::WriteOptions woptsFactory() {
  rocksdb::WriteOptions o{};
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
