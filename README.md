# Portfolio - Optimizer

<table>
<tr>
  <td>Latest Release</td>
  <td><img src="https://img.shields.io/pypi/v/portfolio-optimizer.svg" alt="latest release" /></td>
</tr>
<tr>
  <td>Build Status</td>
  <td>
    <a href="https://travis-ci.org/alpha-miner/portfolio-optimizer">
    <img src="https://travis-ci.org/alpha-miner/portfolio-optimizer.svg?branch=master" alt="travis build status" />
    </a>
  </td>
</tr>
<tr>
  <td>Coverage</td>
  <td><img src="https://coveralls.io/repos/alpha-miner/portfolio-optimizer/badge.svg?branch=master&service=github" alt="coverage" /></td>
</tr>
</table>

该项目在[Alpha-Mind](https://github.com/wegamekinglc/alpha-mind)中被使用。

## TODO lists

Potfolio - ~optimizer由于有大量的c++代码，造成其使用困难。对于依赖的库而言（例如：alpha-mind)，也是使得被依赖库难以使用~。所以Portfolio - Optimizer将有一次重大的重构，包括：

- [x] 提供完整的python接口；
- [x] 作为标准的python包在pypi上发布；
- [ ] 增加多期优化的能力。
