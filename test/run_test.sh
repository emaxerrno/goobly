#!/bin/bash --login


export GLOG_logtostderr=${GLOG_logtostderr:='1'}
export GLOG_v=${GLOG_v:='1'}
export GLOG_vmodule=${GLOG_vmodule,''}
export GLOG_logbufsecs=${GLOG_logbufsecs:='0'}
export GLOG_log_dir=${GLOG_log_dir:='.'}
export GTEST_COLOR=${GTEST_COLOR:='no'}
export GTEST_SHUFFLE=${GTEST_SHUFFLE:='1'}
export GTEST_BREAK_ON_FAILURE=${GTEST_BREAK_ON_FAILURE:='1'}
export GTEST_REPEAT=${GTEST_REPEAT:='1'}

# Runs a test in a temporary directory
TMP_DIR=$(mktemp -d)
cp -r $2/* $TMP_DIR
pushd $TMP_DIR
TEST=$1

echo "Test: $@ in directory: $TMP_DIR"
$TEST

STATUS=$?
if [[ $STATUS == 0 ]]; then
    echo "Removing directory: $TMP_DIR for test $1"
    rm -rf $TMP_DIR
fi
popd
exit $STATUS
