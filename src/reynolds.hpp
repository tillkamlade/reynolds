#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <Eigen/SparseCore>
#include <Eigen/SparseCholesky>

class Reynolds {
    public:
        Reynolds();
        
        Reynolds(
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
            const double theta_max);

        std::vector<double> h(const double &epsilon, const double &beta);
   
        unsigned int idx(const unsigned int &i, const unsigned int &j);

        Eigen::SparseMatrix<double> A(const double &epsilon, const double &beta);

        Eigen::VectorXd b(const double &epsilon, const double &beta);

        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> p(const double &epsilon, const double &beta);

        Eigen::Vector<double, 2> L(const double &epsilon, const double &beta);

        Eigen::Vector2d residual(const double &epsilon, const double &beta);

        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> tau(const double &epsilon, const double &beta);

        double P_f(const double &epsilon, double const &beta);

        const double R;
        const double s;
        const double F;
        const double omega;
        const double eta;
        const double p_amb;

        const unsigned int n_theta;
        const unsigned int n_z;
        
        const unsigned int n;
        const double dtheta;
        const double dz;
        const Eigen::ArrayXd theta;
        const Eigen::ArrayXd z;
};