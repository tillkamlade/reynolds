import numpy as np

class Reynolds:
    D: float
    B: float
    s: float
    F: float
    f: float
    eta: float
    p_amb: float
    n_theta: int
    n_z: int
    theta_min: float
    theta_max: float
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