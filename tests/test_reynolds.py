import pytest

from math import pi

from reynolds import Reynolds

@pytest.fixture
def rey() -> Reynolds:
    return Reynolds(D = 0.5,
                    B = 0.25,
                    s = 0.1e-3,
                    F = 20000,
                    f = 5,
                    eta = 100,
                    p_amb = 0,
                    n_theta = 100,
                    n_z = 100)

def test_Reynolds(rey):
    assert hasattr(rey, "D")
    assert hasattr(rey, "B")
    assert hasattr(rey, "s")
    assert hasattr(rey, "F")
    assert hasattr(rey, "f")
    assert hasattr(rey, "eta")
    assert hasattr(rey, "p_amb")
    assert hasattr(rey, "n_theta")
    assert hasattr(rey, "n_z")
    assert hasattr(rey, "theta_min")
    assert hasattr(rey, "theta_max")

    assert rey.D == pytest.approx(0.5)
    assert rey.B == pytest.approx(0.25)
    assert rey.s == pytest.approx(0.1e-3)
    assert rey.F == pytest.approx(20000)
    assert rey.f == pytest.approx(5)
    assert rey.eta == pytest.approx(100)
    assert rey.p_amb == pytest.approx(0)

    assert isinstance(rey.n_theta, int)
    assert rey.n_theta == 100
    assert isinstance(rey.n_z, int)
    assert rey.n_z == 100

    assert rey.theta_min == pytest.approx(0)
    assert rey.theta_max == pytest.approx(2 * pi)