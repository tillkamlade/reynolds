import pytest

from reynolds import Reynolds

@pytest.fixture
def rey() -> Reynolds:
    return Reynolds('c==3')

def test_Reynolds(rey):
    assert rey.name == 'c==3'