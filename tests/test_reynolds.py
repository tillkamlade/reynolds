import pytest

import numpy as np

from reynolds import Reynolds

@pytest.fixture
def reynolds_kwargs() -> dict[str, float | int]:
    return {
        'D': 0.5,
        'B': 0.25,
        's': 0.1e-3,
        'F': 20000,
        'f': 5,
        'eta': 100,
        'p_amb': 0,
        'n_theta': 100,
        'n_z': 100
    }

def test_Reynolds(reynolds_kwargs):
    rey = Reynolds(**reynolds_kwargs)

    assert hasattr(rey, "R")
    assert hasattr(rey, "s")
    assert hasattr(rey, "F")
    assert hasattr(rey, "omega")
    assert hasattr(rey, "eta")
    assert hasattr(rey, "p_amb")
    assert hasattr(rey, "n_theta")
    assert hasattr(rey, "n_z")
    assert hasattr(rey, "n")
    assert hasattr(rey, "dtheta")
    assert hasattr(rey, "dz")

    assert rey.R == pytest.approx(0.25)
    assert rey.s == pytest.approx(0.1e-3)
    assert rey.F == pytest.approx(20000)
    assert rey.omega == pytest.approx(2 * np.pi * 5)
    assert rey.eta == pytest.approx(100)
    assert rey.p_amb == pytest.approx(0)

    assert isinstance(rey.n_theta, int)
    assert rey.n_theta == 100
    assert isinstance(rey.n_z, int)
    assert rey.n_z == 100
    assert isinstance(rey.n, int)
    assert rey.n == 100 * 100
    
    assert rey.dtheta == pytest.approx(2 * np.pi / 100)
    assert rey.dz == pytest.approx(0.25 / 99)

def test_Reynolds_READONLY(reynolds_kwargs):
    rey = Reynolds(**reynolds_kwargs)

    with pytest.raises(expected_exception = AttributeError, match = 'readonly attribute'):
        rey.R = 0
    with pytest.raises(expected_exception = AttributeError, match = 'readonly attribute'):
        rey.s = 0
    with pytest.raises(expected_exception = AttributeError, match = 'readonly attribute'):
        rey.F = 0
    with pytest.raises(expected_exception = AttributeError, match = 'readonly attribute'):
        rey.omega = 0
    with pytest.raises(expected_exception = AttributeError, match = 'readonly attribute'):
        rey.eta = 0
    with pytest.raises(expected_exception = AttributeError, match = 'readonly attribute'):
        rey.p_amb = 0
    with pytest.raises(expected_exception = AttributeError, match = 'readonly attribute'):
        rey.n_theta = 10
    with pytest.raises(expected_exception = AttributeError, match = 'readonly attribute'):
        rey.n_z = 10

def test_Reynolds_negative_values(reynolds_kwargs):
    reynolds_kwargs['n_theta'] = -1
    with pytest.raises(expected_exception = ValueError, match = 'n_theta must be positive'):
        Reynolds(**reynolds_kwargs)

def test_Reynolds_not_implemented(reynolds_kwargs):
    rey = Reynolds(**reynolds_kwargs)
    with pytest.raises(expected_exception = NotImplementedError, match = 'Reynolds.A is not implemented yet'):
        rey.A(0, 0)

def test_Reynolds_theta(reynolds_kwargs):
    rey = Reynolds(**reynolds_kwargs)
    theta = rey.theta
    assert theta.shape[0] == rey.n_theta
    assert np.allclose(theta, np.linspace(0, 2 * np.pi, 100, endpoint = False))

def test_Reynolds_z(reynolds_kwargs):
    rey = Reynolds(**reynolds_kwargs)
    z = rey.z
    assert z.shape[0] == rey.n_z
    assert np.allclose(z, np.linspace(-0.125, 0.125, 100, endpoint = True))