from typing import Final
import numpy as np

class Reynolds:
    R: Final[float]
    s: Final[float]
    F: Final[float]
    omega: Final[float]
    eta: Final[float]
    p_amb: Final[float]
    n_theta: Final[int]
    n_z: Final[int]
    n: Final[int]
    dtheta: Final[int]
    dz: Final[int]
    
    def __init__(self,
                 D: float,
                 B: float,
                 s: float,
                 F: float,
                 f: float,
                 eta: float,
                 p_amb: float,
                 n_theta: int,
                 n_z: int,
                 theta_min: float = 0,
                 theta_max: float = 2 * np.pi) -> None: ...

    def A(self, epsilon: float, beta: float) -> np.ndarray[tuple[int, int], np.dtype[np.float64]]: ...

    @property
    def theta(self) -> np.ndarray[tuple[int], np.dtype[np.float64]]: ...