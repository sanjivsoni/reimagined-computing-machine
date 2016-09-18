# Reimagined-Computing-Machine
> Reduce noise from signal waves using fourier transforms.

## Installation

Run the following commands in Terminal or cmd (windows folks) <br>
1. Clone the repo.
```sh
git clone https://github.com/sanjivsoni/reimagined-computing-machine.git
```

2. Compile the machine. Make sure you are in ~/NoiseReduction directory

```sh
make
```
3. Run the machine !
```sh
./a.out <signal.csv> <noise.csv> <noisefree.csv> <column>
```


## Usage example

Lets assume the following files <br>signal.csv <br> noise.csv <br>noiseFree.csv<br>
Both the noise and signal file should have same structure.<br>To get the noise free desired output from second column (yeah index begins from 1)
```sh
./a.out signal.csv noise.csv noiseFree.csv 2
```

## Meta

Your Name – [@sanjivsoni7](https://twitter.com/sanjivsoni7) – sanjiv1994@gmail.com

[https://github.com/yourname/github-link](https://github.com/sanjivsoni/)

[npm-image]: https://img.shields.io/npm/v/datadog-metrics.svg?style=flat-square
[npm-url]: https://npmjs.org/package/datadog-metrics
[npm-downloads]: https://img.shields.io/npm/dm/datadog-metrics.svg?style=flat-square
[travis-image]: https://img.shields.io/travis/dbader/node-datadog-metrics/master.svg?style=flat-square
[travis-url]: https://travis-ci.org/dbader/node-datadog-metrics
