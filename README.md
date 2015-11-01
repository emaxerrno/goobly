# goobly
goobly - project to port over a  raft impl w/ gRPC in modern C++ - Still work in progress.

# how to contribute?
* use clang-format on all your cpp code. This is what I use in emacs:
```
(global-set-key (kbd "<C-M-tab>") 'clang-format-buffer)
```
* submit PR's for any change / brainstorm ideas
* file issues

# how to get up and running quickly? 
* build the transitive dependencies:

```
# assuming bash
# tested on ubuntu 14.04 x86_64 only
$ cd meta && source source_ansible_bash && ansible-playbook playbook/dev_all.yml
```

* That's it, just build & run the unit tests:


```
# add -j<cpus> to both make & ctest for parallelism
#
$ mkdir build && cd build && cmake .. && make && ctest -V
```

/cc [@gallegoxx](https://twitter.com/gallegoxx)
