from typing import Final
import numpy as np

class Reynolds:
    D: Final[float]
    B: Final[float]
    s: Final[float]
    F: Final[float]
    f: Final[float]
    eta: Final[float]
    p_amb: Final[float]
    n_theta: Final[int]
    n_z: Final[int]
    theta_min: Final[float]
    theta_max: Final[float]
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