/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include "../puncturing/StridedPuncturingSchedule.h"
#include "protocols/SequentialProtocol.h"


// Puncturing
typedef SequentialProtocol<StridedPuncturingSchedule> StridedSequentialProtocol;

