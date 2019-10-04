#include "tommath_private.h"
#ifdef S_MP_DIV_SMALL_C
/* LibTomMath, multiple-precision integer library -- Tom St Denis */
/* SPDX-License-Identifier: Unlicense */

/* slower bit-bang division... also smaller */
mp_err s_mp_div_small(const mp_int *a, const mp_int *b, mp_int *c, mp_int *d)
{
   mp_int ta, tb, tq, q;
   int n;
   mp_sign sign;
   mp_err err;

   /* init our temps */
   if ((err = mp_init_multi(&ta, &tb, &tq, &q, NULL)) != MP_OKAY) {
      return err;
   }

   mp_set(&tq, 1uL);
   n = mp_count_bits(a) - mp_count_bits(b);
   if ((err = mp_abs(a, &ta)) != MP_OKAY)                         goto LBL_ERR;
   if ((err = mp_abs(b, &tb)) != MP_OKAY)                         goto LBL_ERR;
   if ((err = mp_mul_2d(&tb, n, &tb)) != MP_OKAY)                 goto LBL_ERR;
   if ((err = mp_mul_2d(&tq, n, &tq)) != MP_OKAY)                 goto LBL_ERR;

   while (n-- >= 0) {
      if (mp_cmp(&tb, &ta) != MP_GT) {
         if ((err = mp_sub(&ta, &tb, &ta)) != MP_OKAY)            goto LBL_ERR;
         if ((err = mp_add(&q, &tq, &q)) != MP_OKAY)              goto LBL_ERR;
      }
      if ((err = mp_div_2d(&tb, 1, &tb, NULL)) != MP_OKAY)        goto LBL_ERR;
      if ((err = mp_div_2d(&tq, 1, &tq, NULL)) != MP_OKAY)        goto LBL_ERR;
   }

   /* now q == quotient and ta == remainder */

   sign = (a->sign == b->sign) ? MP_ZPOS : MP_NEG;
   if (c != NULL) {
      mp_exch(c, &q);
      c->sign  = MP_IS_ZERO(c) ? MP_ZPOS : sign;
   }
   if (d != NULL) {
      mp_exch(d, &ta);
      d->sign = MP_IS_ZERO(d) ? MP_ZPOS : a->sign;
   }
LBL_ERR:
   mp_clear_multi(&ta, &tb, &tq, &q, NULL);
   return err;
}

#endif
