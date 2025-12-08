/* XMRig
 * Copyright (c) 2018-2021 SChernykh   <https://github.com/SChernykh>
 * Copyright (c) 2016-2021 XMRig       <https://github.com/xmrig>, <support@xmrig.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include "net/JobResult.h"
#include "base/net/stratum/Job.h"
#include "base/tools/Cvt.h"


#include <cstdio>


xmrig::JobResult::JobResult(int64_t id, const char *jobId, const char *nonce, const char *result, const xmrig::Algorithm &algorithm, const char* sig, const char* sig_data, uint8_t view_tag, int64_t extra_nonce) :
    algorithm(algorithm),
    nonce(nonce),
    result(result),
    sig(sig),
    sig_data(sig_data),
    view_tag(view_tag),
    id(id),
    extra_nonce(extra_nonce),
    jobId(jobId)
{
    if (result && strlen(result) == 64) {
        uint64_t target = 0;
        Cvt::fromHex(reinterpret_cast<uint8_t *>(&target), sizeof(target), result + 48, 16);

        if (target > 0) {
            m_actualDiff = Job::toDiff(target);
        }
    }
}


bool xmrig::JobResult::isCompatible(uint8_t fixedByte) const
{
    const size_t nonceLen = strlen(nonce);

    if (nonceLen == 64) {
        // Junocash 32-byte nonce: check byte 31 (hex chars 62-63)
        uint8_t n;
        if (!Cvt::fromHex(&n, 1, nonce + 62, 2)) {
            return false;
        }
        return n == fixedByte;
    }

    // Standard 4-byte nonce: check byte 3 (MSB)
    uint8_t n[4];
    if (!Cvt::fromHex(n, sizeof(n), nonce, 8)) {
        return false;
    }

    return n[3] == fixedByte;
}


bool xmrig::JobResult::isValid() const
{
    if (!nonce || m_actualDiff == 0) {
        return false;
    }

    const size_t nonceLen = strlen(nonce);
    // Allow 8-char (standard 4-byte) or 64-char (Junocash 32-byte) nonces
    if (nonceLen != 8 && nonceLen != 64) {
        return false;
    }

    return !jobId.isNull();
}
