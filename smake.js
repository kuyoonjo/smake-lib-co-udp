const { LLVM_Darwin } = require('smake');
const { addLibs, addFrameworks } = require('@smake/libs');
const { co_Darwin } = require('@smake/co');
const { buffer } = require('@smake/buffer');
const { udp } = require('./lib');
const { sync } = require('glob');

class co_darwin_arm64 extends co_Darwin {
  prefix = '';
  ar = 'ar';
  ARCH = 'arm64';
}

class test extends LLVM_Darwin {
  ARCH = 'arm64';
  prefix = '';
  files = [
    ...sync('src/**/*.cc'),
  ];
  cxxflags = [
    ...super.cxxflags,
    '-std=c++17',
  ];
}

module.exports = {
  targets: [
    co_darwin_arm64,
    addLibs(addFrameworks(test, co_darwin_arm64), buffer, udp),
  ],
};