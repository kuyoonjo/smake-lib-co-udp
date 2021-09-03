const { LLVM_Darwin } = require('smake');
const { addLibs, framework } = require('@smake/libs');
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

const k = new co_darwin_arm64();
console.log(k.outputFileBasename);

module.exports = {
  targets: [
    co_darwin_arm64,
    addLibs(test, framework(co_darwin_arm64), buffer, udp),
  ],
};