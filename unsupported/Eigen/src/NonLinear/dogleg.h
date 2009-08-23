
template <typename Scalar>
void ei_dogleg(
        Matrix< Scalar, Dynamic, 1 >  &r__,
        const Matrix< Scalar, Dynamic, 1 >  &diag,
        const Matrix< Scalar, Dynamic, 1 >  &qtb,
        Scalar delta,
        Matrix< Scalar, Dynamic, 1 >  &x)
{
    /* Local variables */
    int i, j, k, l, jj, jp1;
    Scalar sum, temp, alpha, bnorm;
    Scalar gnorm, qnorm;
    Scalar sgnorm;

    /* Function Body */
    const Scalar epsmch = epsilon<Scalar>();
    const int n = diag.size();
    Matrix< Scalar, Dynamic, 1 >  wa1(n), wa2(n);
    assert(n==qtb.size());
    assert(n==x.size());

    /*     first, calculate the gauss-newton direction. */

    jj = n * (n + 1) / 2;
    for (k = 0; k < n; ++k) {
        j = n - k - 1;
        jp1 = j + 1;
        jj -= k+1;
        l = jj + 1;
        sum = 0.;
        if (n < jp1) {
            goto L20;
        }
        for (i = jp1; i < n; ++i) {
            sum += r__[l] * x[i];
            ++l;
            /* L10: */
        }
L20:
        temp = r__[jj];
        if (temp != 0.) {
            goto L40;
        }
        l = j;
        for (i = 0; i <= j; ++i) {
            /* Computing MAX */
            temp = std::max(temp,ei_abs(r__[l]));
            l = l + n - i;
            /* L30: */
        }
        temp = epsmch * temp;
        if (temp == 0.) {
            temp = epsmch;
        }
L40:
        x[j] = (qtb[j] - sum) / temp;
        /* L50: */
    }

    /*     test whether the gauss-newton direction is acceptable. */

    for (j = 0; j < n; ++j) {
        wa1[j] = 0.;
        wa2[j] = diag[j] * x[j];
        /* L60: */
    }
    qnorm = wa2.stableNorm();
    if (qnorm <= delta) {
        /* goto L140; */
        return;
    }

    /*     the gauss-newton direction is not acceptable. */
    /*     next, calculate the scaled gradient direction. */

    l = 0;
    for (j = 0; j < n; ++j) {
        temp = qtb[j];
        for (i = j; i < n; ++i) {
            wa1[i] += r__[l] * temp;
            ++l;
            /* L70: */
        }
        wa1[j] /= diag[j];
        /* L80: */
    }

    /*     calculate the norm of the scaled gradient and test for */
    /*     the special case in which the scaled gradient is zero. */

    gnorm = wa1.stableNorm();
    sgnorm = 0.;
    alpha = delta / qnorm;
    if (gnorm == 0.) {
        goto L120;
    }

    /*     calculate the point along the scaled gradient */
    /*     at which the quadratic is minimized. */

    for (j = 0; j < n; ++j) {
        wa1[j] = wa1[j] / gnorm / diag[j];
        /* L90: */
    }
    l = 0;
    for (j = 0; j < n; ++j) {
        sum = 0.;
        for (i = j; i < n; ++i) {
            sum += r__[l] * wa1[i];
            ++l;
            /* L100: */
        }
        wa2[j] = sum;
        /* L110: */
    }
    temp = wa2.stableNorm();
    sgnorm = gnorm / temp / temp;

    /*     test whether the scaled gradient direction is acceptable. */

    alpha = 0.;
    if (sgnorm >= delta) {
        goto L120;
    }

    /*     the scaled gradient direction is not acceptable. */
    /*     finally, calculate the point along the dogleg */
    /*     at which the quadratic is minimized. */

    bnorm = qtb.stableNorm();
    temp = bnorm / gnorm * (bnorm / qnorm) * (sgnorm / delta);
    /* Computing 2nd power */
    temp = temp - delta / qnorm * ei_abs2(sgnorm / delta) + ei_sqrt(ei_abs2(temp - delta / qnorm) + (1.-ei_abs2(delta / qnorm)) * (1.-ei_abs2(sgnorm / delta)));
    /* Computing 2nd power */
    alpha = delta / qnorm * (1. - ei_abs2(sgnorm / delta)) / temp;
L120:

    /*     form appropriate convex combination of the gauss-newton */
    /*     direction and the scaled gradient direction. */

    temp = (1. - alpha) * std::min(sgnorm,delta);
    for (j = 0; j < n; ++j) {
        x[j] = temp * wa1[j] + alpha * x[j];
    }
    return;

}
