using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MTLTestApp
{
    internal class Elliptic
    {
        // maximum number of iterations of a series
        public const int SeriesMax = 250;

        // Series for complete elliptic integral of the first kind (A&S 17.3.11):
        //   K(k) = (pi/2) ( 1 + (1/2 k)^2 + (1/2 k 3/4 k)^2 + (1/2 k 3/4 k 5/6 k)^2 + ... )

        private static double EllipticK_Series(double k)
        {
            double z = 1.0;
            double f = 1.0;
            for (int n = 1; n < SeriesMax; n++)
            {
                double f_old = f;
                z = z * (2 * n - 1) / (2 * n) * k;
                f += z * z;
                if (f == f_old) return (Math.PI / 2.0 * f);
            }
            throw new NonconvergenceException();
        }

        // Asymptotic expansion of Elliptic integral of first kind (DLMF 19.21.1)
        //   K(k) = \sum_n [ (1/2)_n k' / n! ]^2 [ ln(1/k') + Psi(n+1) - Psi(n+1/2) ]
        // Since this is a power series in k', it is good close to k = 1
        // Converges in 8 terms at k' = 0.1, 12 terms at k' = 0.25, 23 terms at k' = 0.5, 53 terms at k' = 0.75
        // Accurate even at k' = 0.75; because all terms are the same sign, no cancelations occur even when terms do not decrease rapidly

        private static double EllipticK_Asymptotic(double k1)
        {
            double p = 1.0;
            double q = 2.0 * Math.Log(2.0) - Math.Log(k1);
            double f = q;
            for (int n = 1; n < SeriesMax; n++)
            {
                double f_old = f;
                p *= k1 * (n - 0.5) / n;
                q -= 1.0 / (n * (2 * n - 1));
                double df = p * p * q;
                f += df;
                if (f == f_old) return (f);
            }
            throw new NonconvergenceException();
        }

        // K(k) can be expressed as an arithmetic-geometric mean (A&S 17.6)
        //   K(k) = = \frac{\pi}{2 AGM(1-k, 1+k)} = \frac{\pi}{2 AGM(1,k')}
        // AGM(a,b) is determined by taking the arithmetic mean (a+b)/2 and the geometric mean sqrt(ab) and re-inserting them as
        // arguments until convergence is achieved.

        private static double EllipticK_AGM(double k)
        {

            double a = 1.0;
            double b = Math.Sqrt((1.0 - k) * (1.0 + k));

            // Starting from 1-k, 1+k, the first iteration will always take us to 1, k',
            // so although it looks prettier (more symmetric) to start with 1+k, 1-k,
            // it's computationally faster to start with 1, k'.

            return 0.5 * Math.PI / AGM(a, b);

        }

        private static double AGM(double x, double y)
        {

            // The basic technique of AGM is to compute
            //   x_{n+1} = (x_{n} + y_{n}) / 2
            //   y_{n+1} = \sqrt{ x_{n} y_{n} }
            // until the both coverge to same value.

            // A naively implemented convergence test (x == y) can cause endless
            // looping because x and y dance around final value, changing the last
            // few bits each time in a cycle. So we have to terminate when "close enough".

            // You can do a series development to show that
            //   agm(c-d, c+d) = c [ 1 - 1/4 r^2 - 5/64 r^4 - O(r^6) \right]
            // where r = d/c, and the coefficient of r^6 is << 1.
            // So we can stop when (d/c)^6 ~ \epsilon, or
            // d ~ c \epsilon^(1/6), and then use this series to avoid
            // taking last few roots. By simple algebra,
            //   c = (x+y)/2  d = (x-y)/2
            // and we would need to form the sum to take the next iteration anyway.

            for (int n = 1; n < SeriesMax; n++)
            {

                double s = x + y;
                double t = x - y;

                if (Math.Abs(t) < Math.Abs(s) * agmEpsilon)
                {
                    double rSquared = Math.Pow(t / s, 2);
                    return 0.5 * s * (1.0 - rSquared * (1.0 / 4.0 + 5.0 / 64.0 * rSquared));
                }

                // This can go wrong when x * y underflows or overflows. This won't
                // happen when called for EllipticE since we start with x=1. But
                // don't make this method public until we can handle whole domain.
                y = Math.Sqrt(x * y);
                x = 0.5 * s;

            }

            throw new NonconvergenceException();

        }

        private static readonly double agmEpsilon = Math.Pow(1.0E-16, 1.0 / 6.0);

        public static double EllipticK(double k)
        {
            if (k < 0.0)
            {
                throw new ArgumentOutOfRangeException(nameof(k));
            }
            else if (k < 0.25)
            {
                // For small k, use the series near k~0.
                return (EllipticK_Series(k));
            }
            else if (k < 0.875)
            {
                // For intermediate k, use the AGM method.
                return (EllipticK_AGM(k));
            }
            else if (k < 1.0)
            {
                // For large k, use the asymptotic expansion. (k' = 0.484 at k=0.875)
                // Note Math.Sqrt((1.0-k)*(1.0+k)) is significantly more accurate for small 1-k
                // than Math.Sqrt(1.0-k*k), at the cost of one extra flop.
                // My testing indicates a rms error of 1E-17 vs 3E-14, max error of 1E-16 vs 2E-13.
                double k1 = Math.Sqrt((1.0 - k) * (1.0 + k));
                Debug.Assert(k1 > 0.0);
                return (EllipticK_Asymptotic(k1));
            }
            else if (k == 1.0)
            {
                return (Double.PositiveInfinity);
            }
            else if (k <= Double.PositiveInfinity)
            {
                throw new ArgumentOutOfRangeException(nameof(k));
            }
            else
            {
                Debug.Assert(Double.IsNaN(k));
                return (k);
            }
        }

        // Series for complete elliptic integral of the second kind
        // Converges in 7 terms for k = 0.1, 12 for k = 0.25, 22 for k = 0.5, 49 for k = 0.75; accurate for all these cases

        private static double EllipticE_Series(double k)
        {

            // (Pi/2) [ 1 - (1/2 k)^2 / 1 - (1/2 k 3/4 k)^2 / 3 - (1/2 k 3/4 k 5/6 k)^2 / 5 + ... ]

            double z = 1.0;
            double f = 1.0;
            for (int n = 1; n < SeriesMax; n++)
            {
                double f_old = f;
                z *= k * (n - 0.5) / n;
                f -= z * z / (2 * n - 1);
                if (f == f_old) return (Math.PI / 2.0 * f);
            }

            throw new NonconvergenceException();

        }

        // 7 terms at k'=0.1, 12 at k'=0.25, 23 at k'=0.5, 52 at k'=0.75

        private static double EllipticE_Asymptotic(double k1)
        {

            double k12 = k1 * k1;
            double p = k12 / 2.0;
            double q = 2.0 * Math.Log(2.0) - 0.5 - Math.Log(k1);
            double f = 1.0 + p * q;
            for (int m = 1; m < SeriesMax; m++)
            {
                double f_old = f;
                p *= (m - 0.5) * (m + 0.5) / m / (m + 1) * k12;
                q -= 1.0 / (2 * m - 1) / (2 * m) + 1.0 / (2 * m + 1) / (2 * m + 2);
                f += p * q;
                if (f == f_old) return (f);
            }
            throw new NonconvergenceException();

        }

        /// <summary>
        /// Computes the complete elliptic integral of the second kind.
        /// </summary>
        /// <param name="k">The elliptic modulus, which must lie between zero and one.</param>
        /// <returns>The value of the Legendre integral E(k).</returns>
        /// <exception cref="ArgumentOutOfRangeException"><paramref name="k"/> lies outside [0, 1].</exception>
        /// <remarks>
        /// <para>E(k) is defined as the complete elliptic integral:</para>
        /// <img src="../images/EllipticEIntegral.png" />
        /// <para>It appears in the Legendre reduction of integrals of rational funtions.</para>
        /// <para>The perimeter of an ellipse with major axis a and eccentricity e is 4 a E(e).</para>
        /// <para>Be aware that some authors use the the parameter m = k<sup>2</sup> instead of the modulus k.</para>
        /// </remarks>
        /// <seealso cref="EllipticE(double,double)"/>
        /// <seealso href="http://en.wikipedia.org/wiki/Elliptic_integral"/>
        /// <seealso href="http://mathworld.wolfram.com/CompleteEllipticIntegraloftheSecondKind.html"/>
        public static double EllipticE(double k)
        {
            // These expansions are accurate in the intermediate region, but require many terms.
            // It would be good to use a faster approach there, like we do for K.
            if (k < 0.0)
            {
                throw new ArgumentOutOfRangeException(nameof(k));
            }
            else if (k < 0.71)
            {
                return (EllipticE_Series(k));
            }
            else if (k < 1.0)
            {
                double k1 = Math.Sqrt((1.0 - k) * (1.0 + k));
                Debug.Assert(k1 > 0.0);
                return (EllipticE_Asymptotic(k1));
            }
            else if (k == 1.0)
            {
                return (1.0);
            }
            else if (k <= Double.PositiveInfinity)
            {
                throw new ArgumentOutOfRangeException(nameof(k));
            }
            else
            {
                Debug.Assert(Double.IsNaN(k));
                return (k);
            }
        }

        public class NonconvergenceException : Exception
        {

            /// <summary>
            /// Initializes a new non-convergence exception.
            /// </summary>
            public NonconvergenceException() : base("The algorithm did not converge within the allowed number of iterations.") { }

            /// <summary>
            /// Initializes a new non-convergence exception with the given exception message.
            /// </summary>
            /// <param name="message">The exception message.</param>
            public NonconvergenceException(String message) : base(message) { }

            /// <summary>
            /// Initializes a new non-convergence exception with the given exception message and inner exception.
            /// </summary>
            /// <param name="message">The exception message.</param>
            /// <param name="innerException">The inner exception.</param>
            public NonconvergenceException(String message, Exception innerException) : base(message, innerException) { }

        }
    }
}
