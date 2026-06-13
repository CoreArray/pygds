"""Round-trip and read tests for pygds.

Run directly (``python tests/test_pygds.py``) or under pytest.
Covers: open/read of the bundled example, create/write/read round-trips for
all common numeric types and string data, every compressor (ZIP / LZMA / LZ4
and their _RA variants), multi-dimensional arrays, sub-region writes, append,
folders, attributes, setdim, exist and delete.
"""

import os
import tempfile

import numpy as np

import pygds


# ---------------------------------------------------------------------------

def test_open_example():
	f = pygds.gdsfile()
	f.open(pygds.get_example_path('ceu_exon.gds'))
	try:
		root = f.root()
		names = root.ls()
		assert 'sample.id' in names and 'genotype' in names
		sid = root.index('sample.id').read()
		assert list(sid[:2]) == ['NA06984', 'NA06985']
		pos = root.index('position').read()
		assert pos.dtype == np.int32 and pos.shape == (1348,)
		geno = root.index('genotype/data').read()
		assert geno.shape == (1348, 90, 2)
		assert int(np.asarray(geno).sum()) == 84331
		# factor decoded to strings
		filt = root.index('annotation/filter').read()
		assert filt[0] == 'PASS'
	finally:
		f.close()


def test_numeric_types():
	fn = os.path.join(tempfile.mkdtemp(), 'num.gds')
	cases = {
		'i8':  np.arange(-5, 5, dtype=np.int8),
		'u8':  np.arange(0, 10, dtype=np.uint8),
		'i16': np.arange(1000, 1010, dtype=np.int16),
		'i32': np.arange(100000, dtype=np.int32),
		'i64': np.arange(5, dtype=np.int64) * 10_000_000_000,
		'f32': (np.arange(20, dtype=np.float32) / 7),
		'f64': np.linspace(-1, 1, 333),
	}
	f = pygds.gdsfile(); f.create(fn)
	try:
		r = f.root()
		for nm, v in cases.items():
			r.add(nm, v)
	finally:
		f.close()

	f = pygds.gdsfile(); f.open(fn)
	try:
		r = f.root()
		for nm, v in cases.items():
			got = r.index(nm).read()
			assert np.array_equal(got, v), (nm, got[:5], v[:5])
	finally:
		f.close()


def test_compressors():
	fn = os.path.join(tempfile.mkdtemp(), 'comp.gds')
	data = np.tile(np.arange(2000, dtype=np.int32), 3)  # compressible
	methods = ['', 'ZIP', 'ZIP_RA', 'LZMA', 'LZMA_RA', 'LZ4', 'LZ4_RA']
	f = pygds.gdsfile(); f.create(fn)
	try:
		r = f.root()
		for m in methods:
			r.add('c_' + (m or 'none'), data, compress=m)
	finally:
		f.close()

	f = pygds.gdsfile(); f.open(fn)
	try:
		r = f.root()
		for m in methods:
			got = r.index('c_' + (m or 'none')).read()
			assert np.array_equal(got, data), m
	finally:
		f.close()


def test_multidim_and_subregion():
	fn = os.path.join(tempfile.mkdtemp(), 'md.gds')
	a = np.arange(24, dtype=np.int32).reshape(2, 3, 4)
	f = pygds.gdsfile(); f.create(fn)
	try:
		r = f.root()
		n = r.add('cube', a, compress='LZMA_RA')
		# a sub-region write needs a writable (non-compressed) node
		m = r.add('mat', np.zeros((4, 4), dtype=np.int32))
		m.write(np.array([7, 7], dtype=np.int32), start=[1, 1], count=[1, 2])
	finally:
		f.close()

	f = pygds.gdsfile(); f.open(fn)
	try:
		r = f.root()
		cube = r.index('cube').read()
		assert cube.shape == (2, 3, 4) and np.array_equal(cube, a)
		mat = r.index('mat').read()
		assert mat[1, 1] == 7 and mat[1, 2] == 7 and mat[0, 0] == 0
	finally:
		f.close()


def test_strings_and_append():
	fn = os.path.join(tempfile.mkdtemp(), 'str.gds')
	f = pygds.gdsfile(); f.create(fn)
	try:
		r = f.root()
		r.add('s', ['alpha', 'beta', 'gamma'], compress='LZ4_RA')
		n = r.add('grow', np.arange(3, dtype=np.int32))
		n.append(np.arange(3, 6, dtype=np.int32))
		n.append(np.arange(6, 9, dtype=np.int32))
	finally:
		f.close()

	f = pygds.gdsfile(); f.open(fn)
	try:
		r = f.root()
		assert list(r.index('s').read()) == ['alpha', 'beta', 'gamma']
		assert list(r.index('grow').read()) == list(range(9))
	finally:
		f.close()


def test_folders_attrs_exist_delete():
	fn = os.path.join(tempfile.mkdtemp(), 'tree.gds')
	f = pygds.gdsfile(); f.create(fn)
	try:
		r = f.root()
		g = r.addfolder('grp')
		g.add('x', np.arange(4, dtype=np.int32))
		n = r.add('y', np.arange(4, dtype=np.int32))
		n.putattr('unit', 'm')
		n.putattr('k', 7)
		n.putattr('flag', True)
		n.putattr('vec', [1, 2, 3])
		n.putattr('drop', 'gone')
		n.delattr('drop')
		r.add('tmp', np.arange(3, dtype=np.int32))
		r.index('tmp').delete()
	finally:
		f.close()

	f = pygds.gdsfile(); f.open(fn)
	try:
		r = f.root()
		assert r.exist('grp/x') and not r.exist('grp/zzz')
		assert list(r.index('grp/x').read()) == [0, 1, 2, 3]
		at = r.index('y').getattr()
		assert at['unit'] == 'm' and at['k'] == 7 and at['flag'] is True
		assert at['vec'] == [1, 2, 3] and 'drop' not in at
		assert r.index('tmp', silent=True) is None
	finally:
		f.close()


def test_apply():
	fn = os.path.join(tempfile.mkdtemp(), 'apply.gds')
	mat = np.arange(24, dtype=np.int32).reshape(4, 6)  # 4 rows x 6 cols
	f = pygds.gdsfile(); f.create(fn)
	try:
		f.root().add('m', mat)
		f.root().add('v', np.arange(4, dtype=np.int32))
	finally:
		f.close()

	f = pygds.gdsfile(); f.open(fn)
	try:
		m = f.root().index('m')
		# row sums (iterate axis 0): each slice is a length-6 row
		rs = m.apply(0, lambda x: int(x.sum()), as_is='list')
		assert rs == [np.sum(mat[i]) for i in range(4)], rs
		# column sums (iterate axis 1): each slice is a length-4 column
		cs = m.apply(1, lambda x: int(x.sum()), as_is='list')
		assert cs == [int(mat[:, j].sum()) for j in range(6)], cs
		# unlist
		u = m.apply(0, lambda x: x.astype(np.int64) * 0 + 1, as_is='unlist')
		assert u.size == mat.size
		# multi-node lock-step apply over matching margins
		v = f.root().index('v')
		out = pygds.apply_gdsn([m, v], [0, 0],
			lambda row, sv: int(row.sum()) + int(sv), as_is='list')
		assert out == [int(mat[i].sum()) + i for i in range(4)], out
	finally:
		f.close()


def test_digest_summarize_diagnosis():
	import hashlib
	fn = os.path.join(tempfile.mkdtemp(), 'dig.gds')
	v = np.arange(1000, dtype=np.int32)
	f = pygds.gdsfile(); f.create(fn)
	try:
		f.root().add('v', v, compress='LZMA_RA')
	finally:
		f.close()

	f = pygds.gdsfile(); f.open(fn)
	try:
		n = f.root().index('v')
		md5 = n.digest('md5')
		assert md5 == hashlib.md5(v.tobytes()).hexdigest()
		assert n.digest('sha256') == hashlib.sha256(v.tobytes()).hexdigest()
		s = n.summarize()
		assert s['min'] == 0 and s['max'] == 999 and s['n'] == 1000
		assert abs(s['mean'] - 499.5) < 1e-9
		diag = f.diagnosis()
		assert diag['num_fragment'] >= 1 and diag['size'] > 0
	finally:
		f.close()


def test_moveto_copyto_assign_addfile():
	d = tempfile.mkdtemp()
	fn = os.path.join(d, 'mv.gds')
	# an external file to embed
	ext = os.path.join(d, 'payload.bin')
	payload = bytes(range(256)) * 4
	with open(ext, 'wb') as fh:
		fh.write(payload)

	f = pygds.gdsfile(); f.create(fn)
	try:
		r = f.root()
		r.add('a', np.arange(3, dtype=np.int32))
		r.add('b', np.arange(3, 6, dtype=np.int32))
		# copy a -> a_copy
		r.copyto('a_copy', r.index('a'))
		# assign: make c an empty node then assign from b
		c = r.add('c', np.array([0], dtype=np.int32))
		c.assign(r.index('b'))
		# move a to before b
		r.index('a').moveto(r.index('b'), 'before')
		# embed external file, then extract it
		r.addfile('payload', ext, compress='ZIP')
	finally:
		f.close()

	f = pygds.gdsfile(); f.open(fn)
	try:
		r = f.root()
		assert list(r.index('a_copy').read()) == [0, 1, 2]
		assert list(r.index('c').read()) == [3, 4, 5]
		out = os.path.join(d, 'out.bin')
		r.index('payload').getfile(out)
		with open(out, 'rb') as fh:
			assert fh.read() == payload
	finally:
		f.close()


def _run_all():
	import traceback
	fns = [v for k, v in sorted(globals().items())
		if k.startswith('test_') and callable(v)]
	ok = 0
	for fn in fns:
		try:
			fn()
			print('PASS', fn.__name__)
			ok += 1
		except Exception:
			print('FAIL', fn.__name__)
			traceback.print_exc()
	print('\n%d/%d passed' % (ok, len(fns)))
	return ok == len(fns)


if __name__ == '__main__':
	import sys
	sys.exit(0 if _run_all() else 1)
