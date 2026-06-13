pygds: Python Interface to CoreArray Genomic Data Structure (GDS) Files
===

![GPLv3](http://www.gnu.org/graphics/gplv3-88x31.png)
[GNU General Public License, GPLv3](http://www.gnu.org/copyleft/gpl.html) (2017)

[![Build Status](https://travis-ci.org/CoreArray/pygds.png)](https://travis-ci.org/CoreArray/pygds)

pre-release version: v0.1


## Features

This package provides a high-level Python interface to CoreArray Genomic Data Structure (GDS) data files, which are portable across platforms with hierarchical structure to store multiple scalable array-oriented data sets with metadata information. It is suited for large-scale datasets, especially for data which are much larger than the available random-access memory. The pygds package offers the efficient operations specifically designed for integers of less than 8 bits, since a diploid genotype, like single-nucleotide polymorphism (SNP), usually occupies fewer bits than a byte. Data compression and decompression are available with relatively efficient random access.


## Package Maintainer

Dr. Xiuwen Zheng ([zhengxwen@gmail.com](zhengxwen@gmail.com))


## Prerequisites

Python 3 (3.8+)

NumPy 1.20 or later

liblzma in [xz](https://tukaani.org/xz/) utilities (for XZ/LZMA compression).
The bundled CoreArray sources also build in ZLIB and LZ4, so the supported
compression methods are: `ZIP`/`ZIP_RA` (zlib), `LZMA`/`LZMA_RA` (xz), and
`LZ4`/`LZ4_RA`.

On macOS with Homebrew, install xz with `brew install xz`; the build probes
`/opt/homebrew/opt/xz` and `/usr/local/opt/xz` automatically, or set the
`LZMA_PREFIX` environment variable to point at a custom prefix.


## Installation

```sh
pip install git+https://github.com/CoreArray/pygds.git
```

Or, from a checkout, build the extension in place:

```sh
python3 setup.py build_ext --inplace
PYTHONPATH=. python3 tests/test_pygds.py   # run the test suite
```


## Citation

#### Original papers (implemented in R/Bioconductor):

[gdsfmt](http://bioconductor.org/packages/gdsfmt), [SeqArray](http://bioconductor.org/packages/SeqArray)

Zheng X, Levine D, Shen J, Gogarten SM, Laurie C, Weir BS (2012). A High-performance Computing Toolset for Relatedness and Principal Component Analysis of SNP Data. *Bioinformatics*. [DOI: 10.1093/bioinformatics/bts606](http://dx.doi.org/10.1093/bioinformatics/bts606).

Zheng X, Gogarten S, Lawrence M, Stilp A, Conomos M, Weir BS, Laurie C, Levine D (2017). SeqArray -- A storage-efficient high-performance data format for WGS variant calls. *Bioinformatics*. [DOI: 10.1093/bioinformatics/btx145](http://dx.doi.org/10.1093/bioinformatics/btx145).



## Examples

```python
import pygds as gds

fn = gds.get_example_path('ceu_exon.gds')
f = gds.gdsfile()
f.open(fn)
f.show()
f.close()
```

```
File: pygds/data/ceu_exon.gds (32.5K)
+    [  ] *
|--+ description   [  ] *
|--+ sample.id   { Str8 90 LZMA_ra(35.8%), 258B } *
|--+ variant.id   { Int32 1348 LZMA_ra(16.8%), 906B } *
|--+ position   { Int32 1348 LZMA_ra(64.6%), 3.4K } *
|--+ chromosome   { Str8 1348 LZMA_ra(4.63%), 158B } *
|--+ allele   { Str8 1348 LZMA_ra(16.7%), 902B } *
|--+ genotype   [  ] *
|  |--+ data   { Bit2 1348x90x2 LZMA_ra(26.3%), 15.6K } *
|  |--+ extra.index   { Int32 0x3 LZMA_ra, 19B } *
|  \--+ extra   { Int16 0 LZMA_ra, 19B }
|--+ phase   [  ]
|  |--+ data   { Bit1 1348x90 LZMA_ra(0.91%), 138B } *
|  |--+ extra.index   { Int32 0x3 LZMA_ra, 19B } *
|  \--+ extra   { Bit1 0 LZMA_ra, 19B }
|--+ annotation   [  ]
|  |--+ id   { Str8 1348 LZMA_ra(38.4%), 5.5K } *
|  |--+ qual   { Float32 1348 LZMA_ra(2.26%), 122B } *
|  \--+ filter   { Int32,factor 1348 LZMA_ra(2.26%), 122B } *
\--+ sample.annotation   [  ]
   \--+ family   { Str8 90 LZMA_ra(57.1%), 222B }
```



## Also See

[PySeqArray](https://github.com/CoreArray/PySeqArray): data manipulation of whole-genome sequencing variants in Python
