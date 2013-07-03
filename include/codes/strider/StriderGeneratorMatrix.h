/*
 * Copyright (c) 2012 Jonathan Perry
 * This code is released under the MIT license (see LICENSE file).
 */
#pragma once

#include <complex>

/**
 * \ingroup strider
 * \brief Loads strider's 27x33 generator matrix.
 *
 * @param outMatrixG: where values will be written
 * @param rowsG: must be 27
 * @param colsG: must be 33
 */
void getStriderGeneratorMatrix(std::complex<double>* outMatrixG,
                               int rowsG,
                               int colsG);
