/* SPDX-License-Identifier: LGPL-2.1+ */
/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * Copyright (C) 2017 kitsunyan <kitsunyan@inbox.ru>
 */

#include <efi.h>
#include <efilib.h>

#include "sha512.h"

static const UINT64 K[80] = {
        0x428a2f98d728ae22, 0x7137449123ef65cd,
        0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
        0x3956c25bf348b538, 0x59f111f1b605d019,
        0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
        0xd807aa98a3030242, 0x12835b0145706fbe,
        0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
        0x72be5d74f27b896f, 0x80deb1fe3b1696b1,
        0x9bdc06a725c71235, 0xc19bf174cf692694,
        0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
        0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
        0x2de92c6f592b0275, 0x4a7484aa6ea6e483,
        0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
        0x983e5152ee66dfab, 0xa831c66d2db43210,
        0xb00327c898fb213f, 0xbf597fc7beef0ee4,
        0xc6e00bf33da88fc2, 0xd5a79147930aa725,
        0x06ca6351e003826f, 0x142929670a0e6e70,
        0x27b70a8546d22ffc, 0x2e1b21385c26c926,
        0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
        0x650a73548baf63de, 0x766a0abb3c77b2a8,
        0x81c2c92e47edaee6, 0x92722c851482353b,
        0xa2bfe8a14cf10364, 0xa81a664bbc423001,
        0xc24b8b70d0f89791, 0xc76c51a30654be30,
        0xd192e819d6ef5218, 0xd69906245565a910,
        0xf40e35855771202a, 0x106aa07032bbd1b8,
        0x19a4c116b8d2d0c8, 0x1e376c085141ab53,
        0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
        0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
        0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
        0x748f82ee5defb2fc, 0x78a5636f43172f60,
        0x84c87814a1f0ab72, 0x8cc702081a6439ec,
        0x90befffa23631e28, 0xa4506cebde82bde9,
        0xbef9a3f7b2c67915, 0xc67178f2e372532b,
        0xca273eceea26619c, 0xd186b8c721c0c207,
        0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
        0x06f067aa72176fba, 0x0a637dc5a2c898a6,
        0x113f9804bef90dae, 0x1b710b35131c471b,
        0x28db77f523047d84, 0x32caab7b40c72493,
        0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
        0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
        0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

#define be_swap(var) { \
        (var) = (UINT64) ((UINT8 *) &(var))[0] << 56 | \
                (UINT64) ((UINT8 *) &(var))[1] << 48 | \
                (UINT64) ((UINT8 *) &(var))[2] << 40 | \
                (UINT64) ((UINT8 *) &(var))[3] << 32 | \
                (UINT64) ((UINT8 *) &(var))[4] << 24 | \
                (UINT64) ((UINT8 *) &(var))[5] << 16 | \
                (UINT64) ((UINT8 *) &(var))[6] << 8 | \
                (UINT64) ((UINT8 *) &(var))[7]; \
}

#define rotr(x, n) (((x) >> (n)) | ((x) << (64 - (n))))
#define ch(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define maj(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define sum0(x) (rotr(x, 28) ^ rotr(x, 34) ^ rotr(x, 39))
#define sum1(x) (rotr(x, 14) ^ rotr(x, 18) ^ rotr(x, 41))
#define s0(x) (rotr(x, 1) ^ rotr(x, 8) ^ (x >> 7))
#define s1(x) (rotr(x, 19) ^ rotr(x, 61) ^ (x >> 6))

static VOID sha512_block(Sha512Context *context) {
        UINTN i;
        UINT64 t1, t2;
        UINT64 a, b, c, d, e, f, g, h;
        UINT64 *w;

        a = context->h[0];
        b = context->h[1];
        c = context->h[2];
        d = context->h[3];
        e = context->h[4];
        f = context->h[5];
        g = context->h[6];
        h = context->h[7];

        w = context->w;
        for (i = 0; i < 80; i++) {
                if (i >= 16)
                        w[i&0xf] = s1(w[(i-2)&0xf]) + w[(i-7)&0xf] + s0(w[(i-15)&0xf]) + w[(i-16)&0xf];
                else
                        be_swap(w[i]);
                t1 = h + sum1(e) + ch(e, f, g) + K[i] + w[i&0xf];
                t2 = sum0(a) + maj(a, b, c);

                h = g;
                g = f;
                f = e;
                e = d + t1;
                d = c;
                c = b;
                b = a;
                a = t1 + t2;
        }

        context->h[0] += a;
        context->h[1] += b;
        context->h[2] += c;
        context->h[3] += d;
        context->h[4] += e;
        context->h[5] += f;
        context->h[6] += g;
        context->h[7] += h;
}

VOID sha512_create(Sha512Context *context) {
        context->h[0] = 0x6a09e667f3bcc908;
        context->h[1] = 0xbb67ae8584caa73b;
        context->h[2] = 0x3c6ef372fe94f82b;
        context->h[3] = 0xa54ff53a5f1d36f1;
        context->h[4] = 0x510e527fade682d1;
        context->h[5] = 0x9b05688c2b3e6c1f;
        context->h[6] = 0x1f83d9abfb41bd6b;
        context->h[7] = 0x5be0cd19137e2179;
        context->size = 0;
        context->total = 0;
}

VOID sha512_update(Sha512Context *context, VOID *data, UINTN length) {
        UINTN count, size;
        VOID *ptr;

        context->total += length;
        size = context->size;
        while (length > 0) {
                count = 128 - size;
                count = length < count ? length : count;

                ptr = (VOID *) context->w + size;
                if (data) {
                        CopyMem(ptr, data, count);
                        data += count;
                } else
                        SetMem(ptr, count, 0);

                size += count;
                length -= count;

                if (size == 128) {
                        sha512_block(context);
                        size = 0;
                }
        }
        context->size = size;
}

VOID sha512_finish(Sha512Context *context, UINT8 *output) {
        UINTN i, total, padding;
        UINT8 end;

        total = context->total;
        padding = (context->size >= 112 ? 128 : 0) + 112 - context->size;
        end = 0x80;
        sha512_update(context, &end, 1);
        sha512_update(context, NULL, padding-1);

        context->w[14] = 0;
        context->w[15] = total << 3;
        be_swap(context->w[15]);
        sha512_block(context);

        for (i = 0; i < 8; i++)
                be_swap(context->h[i]);
        CopyMem(output, context->h, 64);
}

VOID sha512_compute(VOID *data, UINTN length, UINT8 *output) {
        Sha512Context context;
        sha512_create(&context);
        sha512_update(&context, data, length);
        sha512_finish(&context, output);
}
