#include "reynolds.hpp"

Reynolds::Reynolds() :
            R(0),
            s(0),
            F(0),
            omega(0),
            eta(0),
            p_amb(0),
            n_theta(0),
            n_z(0),
            n(0),
            dtheta(0),
            dz(0),
            theta(Eigen::ArrayXd::Zero()),
            z(Eigen::ArrayXd::Zero()) {}

Reynolds::Reynolds(
            const double D,
            const double B,
            const double s,
            const double F,
            const double f,
            const double eta,
            const double p_amb,
            const unsigned int n_theta,
            const unsigned int n_z,
            const double theta_min,
            const double theta_max) :
            R(D / 2),
            s(s),
            F(F),
            omega(2 * M_PI * f),
            eta(eta),
            p_amb(p_amb),
            n_theta(n_theta),
            n_z(n_z),
            n(n_theta * n_z),
            dtheta(2 * M_PI / n_theta),
            dz(B / (n_z - 1)),
            theta(Eigen::ArrayXd::LinSpaced(n_theta, theta_min, theta_max - dtheta)),
            z(Eigen::ArrayXd::LinSpaced(n_z, -B/2, B/2)) {}

std::vector<double> Reynolds::h(const double &epsilon, const double &beta) {
    std::vector<double> h;
    h.reserve(this->n_theta);
    const double &s = this->s;

    for (const double &theta : this->theta) {
        h.push_back(s * (1 - epsilon * cos(theta - beta)));
    }

    return h;
}

unsigned int Reynolds::idx(const unsigned int &i, const unsigned int &j) {
    return i * this->n_z + j;
}

Eigen::SparseMatrix<double> Reynolds::A(const double &epsilon, const double &beta) {
    Eigen::SparseMatrix<double> A(this->n, this->n);
    std::vector<double> h = this->h(epsilon, beta);
    
    std::vector<Eigen::Triplet<double>> triplets;
    triplets.reserve(5 * this->n);

    for (unsigned int i = 0; i < this->n_theta; i++) {
        for (unsigned int j = 0; j < this->n_z; j++) {
            if ((j == 0) || (j == this->n_z - 1)) {
                triplets.push_back(Eigen::Triplet<double>(this->idx(i,j), this->idx(i,j), 1));
                continue;
            } else {
                triplets.push_back(Eigen::Triplet<double>(this->idx(i,j), this->idx(i,j+1),    pow(h[i], 3)/pow(this->dz, 2)));
                triplets.push_back(Eigen::Triplet<double>(this->idx(i,j), this->idx(i,j-1),    pow(h[i], 3)/pow(this->dz, 2)));
                triplets.push_back(Eigen::Triplet<double>(this->idx(i,j), this->idx(i,j  ), -2*pow(h[i], 3)/pow(this->dz, 2) - 2*pow(h[i], 3)/(pow(this->R, 2)*pow(this->dtheta, 2))));
                triplets.push_back(Eigen::Triplet<double>(this->idx(i,j), this->idx((i+1)%this->n_theta,j), 3*h[(i+1)%this->n_theta]*pow(h[i], 2)/(4*pow(this->R, 2)*pow(this->dtheta, 2)) - 3*h[(i-1)%this->n_theta]*pow(h[i], 2)/(4*pow(this->R, 2)*pow(this->dtheta, 2)) + pow(h[i], 3)/(pow(this->R, 2)*pow(this->dtheta, 2))));
                triplets.push_back(Eigen::Triplet<double>(this->idx(i,j), this->idx((i-1)%this->n_theta,j), -3*h[(i+1)%this->n_theta]*pow(h[i], 2)/(4*pow(this->R, 2)*pow(this->dtheta, 2)) + 3*h[(i-1)%this->n_theta]*pow(h[i], 2)/(4*pow(this->R, 2)*pow(this->dtheta, 2)) + pow(h[i], 3)/(pow(this->R, 2)*pow(this->dtheta, 2))));
            }
        }
    }

    A.setFromTriplets(triplets.begin(), triplets.end());

    return A;
}

Eigen::VectorXd Reynolds::b(const double &epsilon, const double &beta) {
    Eigen::VectorXd b = Eigen::VectorXd::Zero(this->n);
    std::vector<double> h = this->h(epsilon, beta);

    for (unsigned int i = 0; i < this->n_theta; i++) {
        for (unsigned int j = 0; j < this->n_z; j++) {
            if ((j == 0) || (j == this->n_z - 1)) {
                b(this->idx(i,j)) = this->p_amb;
                continue;
            } else {
                b(this->idx(i,j)) = 3*this->eta*this->omega*(h[(i+1)%this->n_theta] - h[(i-1)%this->n_theta])/this->dtheta;
            }
        }
    }

    return b;
}

Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> Reynolds::p(const double &epsilon, const double &beta) {
    Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>> solver;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> p(this->n_theta, this->n_z);
    Eigen::Map<Eigen::VectorXd> p_vec(p.data(), this->n);

    Eigen::SparseMatrix<double> A = this->A(epsilon, beta);
    Eigen::VectorXd b = this->b(epsilon, beta);

    solver.compute(A);

    p_vec = solver.solve(b);

    for (unsigned int i = 0; i < this->n_theta; i++) {
        for (unsigned int j = 0; j < this->n_z; j++) {
            if (p(i,j) < this->p_amb) p(i,j) = this->p_amb;
        }
    }

    return p;
}

Eigen::Vector<double, 2> Reynolds::L(const double &epsilon, const double &beta) {
    Eigen::Vector<double, 2> L;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> p = this->p(epsilon, beta);
    double dA = this->R * this->dtheta * this->dz;

    for (unsigned int i = 0; i < this->n_theta; i++) {
        for (unsigned int j = 0; j < this->n_z; j++) {
            L(0) += -p(i,j) * sin(this->theta[i]) * dA;
            L(1) +=  p(i,j) * cos(this->theta[i]) * dA;
        }
    }

    return L;
}

Eigen::Vector2d Reynolds::residual(const double &epsilon, const double &beta) {
    Eigen::Vector<double, 2> L = this->L(epsilon, beta);
    Eigen::Vector<double, 2> residual = {L[0], L[1] - this->F};

    return residual;
}

Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> Reynolds::tau(const double &epsilon, const double &beta) {
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> tau(this->n_theta, this->n_z);

    std::vector<double> h = this->h(epsilon, beta);
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> p = this->p(epsilon, beta);

    for (unsigned int i = 0; i < this->n_theta; i++) {
        for (unsigned int j = 0; j < this->n_z; j++) {
            double dp_dtheta = (p((i+1)%this->n_theta,j) - p((i-1)%this->n_theta,j)) / (2 * this->dtheta);
            tau(i,j) = this->eta * ((this->omega * this->R) / h[i] - h[i] / (2 * this->R) * dp_dtheta);
        }
    }

    return tau;
}

double Reynolds::P_f(const double &epsilon, double const &beta) {
    double P_f = 0;
    const double dA = this->R * this->dtheta * this->dz;

    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> tau = this->tau(epsilon, beta);
    
    for (unsigned int i = 0; i < this->n_theta; i++) {
        for (unsigned int j = 0; j < this->n_z; j++) {
            P_f += this->omega * this->R * tau(i,j) * dA;
        }
    }

    return P_f;
}